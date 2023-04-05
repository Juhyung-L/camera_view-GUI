#ifndef CALIBRATIONWIDGET_H
#define CALIBRATIONWIDGET_H

#include <QPushButton>
#include <vector>
#include "videoWidget.h"
#include "videoWindow.h"
#include "settings.h"
#include "cameraStream.h"

class CalibrationWindow : public VideoWindow
{
private:
    void init_ui();
    void reset_vars();
    void set_next_frame() override;
    void enable_close();
    void closeEvent(QCloseEvent *evnet) override;
    bool close_flag;
    VideoWidget *calibration_view;
    QPushButton *close_button;
    CameraStream stream;
    cv::Mat frame;
    cv::Mat gray;
    cv::Mat frame_resized;
    std::string distortion_file;

    // variables for calibration
    cv::Size CHESSBOARD_SIZE;
    std::vector<std::vector<cv::Point3f>> objpoints;
    std::vector<std::vector<cv::Point2f> > imgpoints;
    std::vector<cv::Point3f> objp;
    std::vector<cv::Point2f> corner_pts;
public:
    CalibrationWindow(QWidget *parent=nullptr);
    void start(const Camera& camera);
};

#endif // CALIBRATIONWIDGET_H
