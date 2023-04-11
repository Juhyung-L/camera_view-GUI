#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudaarithm.hpp>
#include "cameraViewWindow.h"
#include "settings.h"

/* This is a dervied class of VideoWindow. It displays the two camera views on a widget. It uses OpenCV's
 * CUDA functionalities to speed up image matrix manipulations.
 */

using namespace resolutions;

CameraViewWindow::CameraViewWindow(QWidget *parent)
    : VideoWindow(parent),
    show_undistort_flag(false),
    close_flag(false),
    close_button(new QPushButton("Close", this)),
    left_view(new VideoWidget(this)),
    right_view(new VideoWidget(this)),
    // CPU matrices
    left_frame(CAPTURE_RESOLUTION.height, CAPTURE_RESOLUTION.width, CV_8UC3),
    right_frame(CAPTURE_RESOLUTION.height, CAPTURE_RESOLUTION.width, CV_8UC3),
    left_frame_resized(CAMERA_VIEW_DISPLAY_RESOLUTION.height, CAMERA_VIEW_DISPLAY_RESOLUTION.width, CV_8UC3),
    right_frame_resized(CAMERA_VIEW_DISPLAY_RESOLUTION.height, CAMERA_VIEW_DISPLAY_RESOLUTION.width, CV_8UC3),
    // GPU matrices
    left_gpu_frame(CAPTURE_RESOLUTION.height, CAPTURE_RESOLUTION.width, CV_8UC3),
    right_gpu_frame(CAPTURE_RESOLUTION.height, CAPTURE_RESOLUTION.width, CV_8UC3),
    left_gpu_frame_undistorted(CAPTURE_RESOLUTION.height, CAPTURE_RESOLUTION.width, CV_8UC3),
    right_gpu_frame_undistorted(CAPTURE_RESOLUTION.height, CAPTURE_RESOLUTION.width, CV_8UC3),
    left_gpu_frame_resized(CAMERA_VIEW_DISPLAY_RESOLUTION.height, CAMERA_VIEW_DISPLAY_RESOLUTION.width, CV_8UC3),
    right_gpu_frame_resized(CAMERA_VIEW_DISPLAY_RESOLUTION.height, CAMERA_VIEW_DISPLAY_RESOLUTION.width, CV_8UC3)
{
    init_ui();
}

void CameraViewWindow::init_ui()
{
    this->setWindowTitle("Camera-View");

    // left_view->setFixedSize(CAMERA_VIEW_DISPLAY_RESOLUTION.width, CAMERA_VIEW_DISPLAY_RESOLUTION.height);
    // right_view->setFixedSize(CAMERA_VIEW_DISPLAY_RESOLUTION.width, CAMERA_VIEW_DISPLAY_RESOLUTION.height);
    QHBoxLayout *HBL = new QHBoxLayout();
    HBL->addWidget(left_view);
    HBL->addWidget(right_view);
    HBL->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addLayout(HBL);
    VBL->addWidget(close_button);
    VBL->setContentsMargins(0, 0, 0, 0);
    this->setLayout(VBL);

    connect(close_button, &QPushButton::clicked, this, &CameraViewWindow::enable_close);
}

void CameraViewWindow::load_camera_models()
{
    left_model.load_data(cameras::LEFT_CAMERA);
    right_model.load_data(cameras::RIGHT_CAMERA);
}

void CameraViewWindow::set_next_frame()
{
    // read the frames
    bool left_ret = left_stream.capture.read(left_frame);
    bool right_ret = right_stream.capture.read(right_frame);

    if (left_ret && right_ret)
    {
        // upload to GPU
        left_gpu_frame.upload(left_frame);
        right_gpu_frame.upload(right_frame);

        if (show_undistort_flag)
        {
            left_model.undistort(left_gpu_frame, left_gpu_frame_undistorted);
            right_model.undistort(right_gpu_frame, right_gpu_frame_undistorted);
            cv::cuda::flip(left_gpu_frame_undistorted, left_gpu_frame_undistorted, 1);
            cv::cuda::flip(right_gpu_frame_undistorted, right_gpu_frame_undistorted, 1);
            cv::cuda::resize(left_gpu_frame_undistorted, left_gpu_frame_resized, CAMERA_VIEW_DISPLAY_RESOLUTION, cv::INTER_LINEAR);
            cv::cuda::resize(right_gpu_frame_undistorted, right_gpu_frame_resized, CAMERA_VIEW_DISPLAY_RESOLUTION, cv::INTER_LINEAR);
        }
        else
        {
            cv::cuda::flip(left_gpu_frame, left_gpu_frame, 1);
            cv::cuda::flip(right_gpu_frame, right_gpu_frame, 1);
            cv::cuda::resize(left_gpu_frame, left_gpu_frame_resized, CAMERA_VIEW_DISPLAY_RESOLUTION, cv::INTER_LINEAR);
            cv::cuda::resize(right_gpu_frame, right_gpu_frame_resized, CAMERA_VIEW_DISPLAY_RESOLUTION, cv::INTER_LINEAR);
        }
        //download frames to CPU
        left_gpu_frame_resized.download(left_frame_resized);
        right_gpu_frame_resized.download(right_frame_resized);

        // paint the frames
        left_view->set_frame(left_frame_resized);
        right_view->set_frame(right_frame_resized);
        left_view->update();
        right_view->update();
    }

    if (close_flag)
    {
        close_flag = false;
        this->close();
    }
}

void CameraViewWindow::enable_close()
{
    close_flag = true;
}

void CameraViewWindow::start(bool show_undistort)
{
    try
    {
        left_stream.connect_camera(cameras::LEFT_CAMERA);
        right_stream.connect_camera(cameras::RIGHT_CAMERA);
    }
    catch (std::runtime_error e)
    {
        left_stream.disconnect_camera();
        right_stream.disconnect_camera();
        throw std::runtime_error(e);
    }

    show_undistort_flag = show_undistort;
    if (show_undistort_flag)
    {
        load_camera_models();
    }
    VideoWindow::start_timer();
}

void CameraViewWindow::closeEvent(QCloseEvent *event)
{
    VideoWindow::stop_timer();
    left_stream.disconnect_camera();
    right_stream.disconnect_camera();
}
