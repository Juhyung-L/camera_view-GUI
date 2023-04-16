#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/persistence.hpp>
#include <QVBoxLayout>
#include "include/calibrationWindow.h"

CalibrationWindow::CalibrationWindow(QWidget *parent)
    : VideoWindow(parent),
    calibration_view(new VideoWidget(this)),
    close_button(new QPushButton("Close", this)),
    progress_bar(new QProgressBar(this)),
    close_flag(false),
    CHESSBOARD_SIZE(6, 9),
    frame(cv::Mat(resolutions::CAPTURE_RESOLUTION.height, resolutions::CAPTURE_RESOLUTION.width, CV_8UC3)),
    gray(cv::Mat(resolutions::CAPTURE_RESOLUTION.height, resolutions::CAPTURE_RESOLUTION.width, CV_8U)),
    frame_resized(cv::Mat(resolutions::CALIBRATION_DISPLAY_RESOLUTION.height, resolutions::CALIBRATION_DISPLAY_RESOLUTION.width, CV_8UC3)),
    num_frames(50)
{
    for (int i{0}; i<CHESSBOARD_SIZE.height; ++i)
    {
        for (int j{0}; j<CHESSBOARD_SIZE.width; ++j)
        {
            objp.push_back(cv::Point3f(j,i,0));
        }
    }
    init_ui();
}

void CalibrationWindow::init_ui()
{
    this->setWindowTitle("Calibration");

    progress_bar->setOrientation(Qt::Horizontal);
    progress_bar->setRange(0, num_frames);
    progress_bar->setValue(0);

    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addWidget(progress_bar);
    VBL->addWidget(calibration_view);
    VBL->addWidget(close_button);
    VBL->setContentsMargins(0, 0, 0, 0);

    connect(close_button, &QPushButton::clicked, this, &CalibrationWindow::enable_close);
}

void CalibrationWindow::reset_vars()
{
    objpoints.clear();
    imgpoints.clear();
    corner_pts.clear();
}

void CalibrationWindow::set_next_frame()
{
    if ((int)objpoints.size() <= num_frames)
    {
        progress_bar->setValue((int)objpoints.size());
        bool found = false;
        bool ret = stream.capture.read(frame);
        if (ret)
        {
           cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
           found = cv::findChessboardCorners(gray,
                                             CHESSBOARD_SIZE,
                                             corner_pts,
                                             cv::CALIB_CB_ADAPTIVE_THRESH |
                                             cv::CALIB_CB_FAST_CHECK |
                                             cv::CALIB_CB_NORMALIZE_IMAGE);
            if (found)
            {
                cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.1);
                cv::cornerSubPix(gray,corner_pts, cv::Size(3,3), cv::Size(-1,-1), criteria);
                cv::drawChessboardCorners(frame, CHESSBOARD_SIZE, corner_pts, found);
                objpoints.push_back(objp);
                imgpoints.push_back(corner_pts);
            }
            cv::resize(frame, frame_resized, resolutions::CALIBRATION_DISPLAY_RESOLUTION, cv::INTER_LINEAR);
            calibration_view->set_frame(frame_resized);
            calibration_view->update();
        }
    }
    else
    {
        cv::Mat camera_matrix, dist_coeffs;
        std::vector<cv::Mat> rvecs, tvecs;
        int flag = cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC | cv::fisheye::CALIB_FIX_SKEW;
        cv::TermCriteria criteria((cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER), 30, 1e-6);
        try
        {
            cv::fisheye::calibrate(objpoints,
                                   imgpoints,
                                   resolutions::CAPTURE_RESOLUTION,
                                   camera_matrix,
                                   dist_coeffs,
                                   rvecs,
                                   tvecs,
                                   flag,
                                   criteria);
        }
        catch (cv::Exception e)
        {
            emit calibration_status(false);
            this->close();
            return;
        }

        cv::FileStorage fs(distortion_file, cv::FileStorage::WRITE);
        fs << "camera_matrix" << camera_matrix;
        fs << "dist_coeffs" << dist_coeffs;
        fs << "resolution" << resolutions::CAPTURE_RESOLUTION;
        emit calibration_status(true);
        this->close();
    }

    if (close_flag)
    {
        close_flag = false;
        this->close();
    }
}

void CalibrationWindow::start(const Camera& camera)
{
    try
    {
        stream.connect_camera(camera);
    }
    catch (std::runtime_error e)
    {
        stream.disconnect_camera();
        throw std::runtime_error(e);
    }
    distortion_file = (std::string)directories::YAML_DIR + "/distortion_" + camera.name + ".yaml";
    reset_vars();
    VideoWindow::start_timer();
}

void CalibrationWindow::closeEvent(QCloseEvent *event)
{
    VideoWindow::stop_timer();
    stream.disconnect_camera();
}

void CalibrationWindow::enable_close()
{
    close_flag = true;
}
