#include "include/cameraStream.h"

/* This class is a basic wrapper around cv::VideoCapture.
 * It opens the camera with the specified resolution in MJPG mode.
 */

CameraStream::CameraStream()
    : resolution(resolutions::CAPTURE_RESOLUTION),
    capture(cv::VideoCapture())
{
}

void CameraStream::connect_camera(const Camera& camera)
{
    capture.open(camera.dir, cv::CAP_V4L);
    capture.set(cv::CAP_PROP_FRAME_WIDTH, resolution.width);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.height);
    capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));

    // throw exception if cv::VideoCapture is not opened
    // there are a lot of runtime problems that can occur if the camera is not opened, so catch it here
    if (!capture.isOpened())
    {
        throw std::runtime_error("Camera [" + camera.name + "] could not be opened");
    }
}

void CameraStream::disconnect_camera()
{
    if (capture.isOpened())
    {
        capture.release();
    }
}
