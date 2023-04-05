#ifndef FISHEYECAMERAMODEL_H
#define FISHEYECAMERAMODEL_H

#include <opencv2/core/mat.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/types.hpp>
#include <string>
#include "settings.h"

class FisheyeCameraModel
{
private:
    std::string distortion_file;
    cv::Mat undistort_mapx;
    cv::Mat undistort_mapy;
    cv::cuda::GpuMat gpu_undistort_mapx;
    cv::cuda::GpuMat gpu_undistort_mapy;
    cv::cuda::GpuMat undistorted_frame;
    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;
    cv::Size resolution;
public:
    FisheyeCameraModel();
    FisheyeCameraModel(const FisheyeCameraModel& model);
    void load_data(const Camera& camera);
    void undistort(const cv::cuda::GpuMat& frame, cv::cuda::GpuMat& undistorted_frame);
    cv::cuda::GpuMat undistort(const cv::cuda::GpuMat& frame);
};

#endif // FISHEYECAMERAMODEL_H
