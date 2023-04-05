#ifndef CAMERASTREAM_H
#define CAMERASTREAM_H

#include <opencv2/videoio.hpp>
#include <opencv2/core/types.hpp>
#include "settings.h"

class CameraStream
{
private:
    cv::Size resolution;
public:
    CameraStream();
    cv::VideoCapture capture;
    void connect_camera(const Camera& camera) noexcept(false); // indicate that the function can throw an exception
    void disconnect_camera();
};

#endif // CAMERASTREAM_H
