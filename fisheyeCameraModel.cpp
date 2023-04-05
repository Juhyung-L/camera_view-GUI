#include <opencv2/core/persistence.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/cudawarping.hpp>
#include <string>
#include "fisheyeCameraModel.h"

FisheyeCameraModel::FisheyeCameraModel()
{
}

void FisheyeCameraModel::load_data(const Camera& camera)
{
    distortion_file = (std::string)directories::YAML_DIR + "/distortion_" + camera.name + ".yaml";
    cv::FileStorage fs(distortion_file, cv::FileStorage::READ);
    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;
    fs["resolution"] >> resolution;
    cv::Mat R = cv::Mat::eye(cv::Size(3, 3), CV_32FC1);

    cv::fisheye::initUndistortRectifyMap(camera_matrix,
                                         dist_coeffs,
                                         R,
                                         camera_matrix,
                                         resolution,
                                         CV_32FC1,
                                         undistort_mapx,
                                         undistort_mapy);

    gpu_undistort_mapx.upload(undistort_mapx);
    gpu_undistort_mapy.upload(undistort_mapy);
    undistorted_frame = cv::cuda::GpuMat(resolution.height, resolution.width, CV_8UC3);
}

void FisheyeCameraModel::undistort(const cv::cuda::GpuMat& frame, cv::cuda::GpuMat& undistorted_frame)
{
    cv::cuda::remap(frame, undistorted_frame, gpu_undistort_mapx, gpu_undistort_mapy, cv::INTER_LINEAR, cv::BORDER_CONSTANT);
}

cv::cuda::GpuMat FisheyeCameraModel::undistort(const cv::cuda::GpuMat& frame)
{
    cv::cuda::remap(frame, undistorted_frame, gpu_undistort_mapx, gpu_undistort_mapy, cv::INTER_LINEAR, cv::BORDER_CONSTANT);
    return undistorted_frame.clone();
}

