#ifndef TABS_H
#define TABS_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include "calibrationWindow.h"
#include "cameraViewWindow.h"
#include "settings.h"

class CameraViewTab : public QWidget
{
private:
    // add CameraViewWidget
    QPushButton *start_button;
    QCheckBox *undistort_checkbox;
    CameraViewWindow *camera_view_window;
    void init_ui();
    bool undistortion_file_check();
public:
    CameraViewTab(QWidget *parent);
    ~CameraViewTab();
    void start_camera_view();
};

class CalibrationTab : public QWidget
{
private:
    // add CalibrationWidget
    QPushButton *calibration_button;
    QComboBox *dropdown_menu;
    CalibrationWindow *calibration_window;
    void init_ui();
    void start_calibration();
public:
    CalibrationTab(QWidget *parent);
    ~CalibrationTab();
};

class CameraMissingWidget : public QWidget
{
private:
    QLabel *error_message;
public:
    CameraMissingWidget(QWidget *parent);
};

#endif // TABS_H
