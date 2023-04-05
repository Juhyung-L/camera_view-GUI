#ifndef SETTINGS_H
#define SETTINGS_H

#include <opencv2/core/types.hpp>
#include <filesystem>
#include <vector>
#include <string>

namespace directories
{
    // TODO: change YAML_DIR when this code is moved to the Jetson Nano
    inline const std::filesystem::path YAML_DIR = "/home/kevin/Desktop/qt_creator_test/test/yaml";
    inline const std::filesystem::path LEFT_CAMERA_DIR = "/dev/video90";
    inline const std::filesystem::path RIGHT_CAMERA_DIR = "/dev/video91";
    inline const std::filesystem::path LEFT_YAML_FILE = YAML_DIR / "distortion_left.yaml";
    inline const std::filesystem::path RIGHT_YAML_FILE = YAML_DIR / "distortion_right.yaml";
    inline void make_directory()
    {
        if (!std::filesystem::exists(YAML_DIR))
        {
            std::filesystem::create_directory(YAML_DIR);
        }
    }
}

class Camera
{
public:
    std::string name;
    std::filesystem::path dir;
    Camera()
    {}
    Camera(std::string name, std::filesystem::path dir)
        :name(name), dir(dir)
    {}
    Camera(const Camera& camera) // copy constructor
        : name(camera.name),
        dir(camera.dir)
    {}
};

namespace cameras
{
    inline const Camera LEFT_CAMERA = Camera("left", directories::LEFT_CAMERA_DIR);
    inline const Camera RIGHT_CAMERA = Camera("right", directories::RIGHT_CAMERA_DIR);
}

namespace resolutions
{
    inline const cv::Size CAPTURE_RESOLUTION(800, 600);
    inline const cv::Size SCREEN_RESOLUTION(1280, 720);
    inline const cv::Size CAMERA_VIEW_DISPLAY_RESOLUTION(640, 700);
    inline const cv::Size CALIBRATION_DISPLAY_RESOLUTION(1280, 700);
}
#endif // SETTINGS_H
