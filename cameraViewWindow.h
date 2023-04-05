#ifndef CAMERAVIEWWINDOW_H
#define CAMERAVIEWWINDOW_H

#include <QPushButton>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/cuda.hpp>
#include "videoWindow.h"
#include "videoWidget.h"
#include "cameraStream.h"
#include "fisheyeCameraModel.h"

class CameraViewWindow : public VideoWindow
{
private:
    void init_ui();
    void load_camera_models();
    void set_next_frame() override;
    void enable_close();
    bool show_undistort_flag;
    bool close_flag;
    void closeEvent(QCloseEvent *event) override;
    QPushButton *close_button;
    CameraStream left_stream;
    CameraStream right_stream;
    VideoWidget *left_view;
    VideoWidget *right_view;
    FisheyeCameraModel left_model;
    FisheyeCameraModel right_model;
    cv::Mat left_frame;
    cv::Mat right_frame;
    cv::Mat left_frame_resized;
    cv::Mat right_frame_resized;
    cv::cuda::GpuMat left_gpu_frame;
    cv::cuda::GpuMat right_gpu_frame;
    cv::cuda::GpuMat left_gpu_frame_undistorted;
    cv::cuda::GpuMat right_gpu_frame_undistorted;
    cv::cuda::GpuMat left_gpu_frame_resized;
    cv::cuda::GpuMat right_gpu_frame_resized;
public:
    CameraViewWindow(QWidget *parent=nullptr);
    void start(bool show_undistort) noexcept(false);
};

#endif // CAMERAVIEWWINDOW_H
