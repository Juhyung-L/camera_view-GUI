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
    CameraViewTab(QWidget *parent=nullptr);
    ~CameraViewTab();
    void start_camera_view();
};

class CalibrationTab : public QWidget
{
    Q_OBJECT
private slots:
    void set_calibration_status(bool status);
private:
    QPushButton *calibration_button;
    QComboBox *dropdown_menu;
    CalibrationWindow *calibration_window;
    QLabel *calibration_status_message;
    void init_ui();
    void start_calibration();
public:
    CalibrationTab(QWidget *parent=nullptr);
    ~CalibrationTab();
};

class CameraMissingWidget : public QWidget
{
private:
    QLabel *error_message;
public:
    CameraMissingWidget(QWidget *parent=nullptr);
};

#endif // TABS_H
