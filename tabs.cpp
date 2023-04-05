#include <QVBoxLayout>
#include <QString>
#include <filesystem>
#include <iostream>
#include "tabs.h"

/* This file has 3 classes: CameraViewTab, CalibrationTab, and CameraMissingWidget. CameraViewTab is for
 * starting the camera view and CalibrationTab is for starting the calibration. They are both tabs that
 * go into the QTabWidget in the main window. CameraMissingWidget is a widget that informs the user that
 * the cameras are not connected. It goes into the QStackedWidget along with the main widget. It is only
 * shown if at least one of the cameras are not connected.
 */


CameraViewTab::CameraViewTab(QWidget *parent):
        QWidget(parent),
        camera_view_window(new CameraViewWindow()),
        start_button(new QPushButton("Start", this)),
        undistort_checkbox(new QCheckBox("Show Undistorted View", this))
{
    init_ui();
    camera_view_window->attach(parent);
}

void CameraViewTab::init_ui()
{
    camera_view_window->set_frame_rate(30);
    connect(start_button, &QPushButton::clicked, this, &CameraViewTab::start_camera_view);

    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addWidget(start_button);
    VBL->addWidget(undistort_checkbox);
    this->setLayout(VBL);
    // disable the checkbox if not all the necessary distortion files are present
    if (!undistortion_file_check())
    {
        undistort_checkbox->setEnabled(false);
        undistort_checkbox->setChecked(false);
    }
    else
    {
        undistort_checkbox->setEnabled(true);
        undistort_checkbox->setChecked(true);
    }
    start_camera_view(); // automatically start camera view
}

void CameraViewTab::start_camera_view()
{
    try
    {
        camera_view_window->start(undistort_checkbox->checkState());
    }
    catch (std::runtime_error e) // catch runtime error from the cameras not being connected
    {
        std::cerr << e.what() << std::endl;
    }
}

bool CameraViewTab::undistortion_file_check()
{
    if (std::filesystem::exists(directories::LEFT_YAML_FILE) &&
        std::filesystem::exists(directories::RIGHT_YAML_FILE))
    {
        return true;
    }
    else
    {
        return false;
    }
}

CameraViewTab::~CameraViewTab()
{
    delete camera_view_window;
}

CalibrationTab::CalibrationTab(QWidget *parent)
    : QWidget(parent),
    calibration_window(new CalibrationWindow()),
    calibration_button(new QPushButton("Calibrate", this)),
    dropdown_menu(new QComboBox(this))
{
    init_ui();
    calibration_window->attach(parent);
}

void CalibrationTab::init_ui()
{
    calibration_window->set_frame_rate(5);
    connect(calibration_button, &QPushButton::clicked, this, &CalibrationTab::start_calibration);

    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addWidget(dropdown_menu);
    VBL->addWidget(calibration_button);
    dropdown_menu->addItem(QString::fromStdString(cameras::LEFT_CAMERA.name));
    dropdown_menu->addItem(QString::fromStdString(cameras::RIGHT_CAMERA.name));
    this->setLayout(VBL);
}

void CalibrationTab::start_calibration()
{
    Camera selected_camera;
    QString selected_camera_name = dropdown_menu->currentText();
    if (selected_camera_name == "left")
    {
        selected_camera = cameras::LEFT_CAMERA;
    }
    else if (selected_camera_name == "right")
    {
        selected_camera = cameras::RIGHT_CAMERA;
    }

    try
    {
        calibration_window->start(selected_camera);
    }
    catch (std::runtime_error e) // catch runtime error from the cameras not being connected
    {
        std::cerr << e.what() << std::endl;
    }
}

CalibrationTab::~CalibrationTab()
{
    delete calibration_window;
}

CameraMissingWidget::CameraMissingWidget(QWidget *parent)
    : QWidget(parent),
    error_message(new QLabel(this))
{
    error_message->setText("One of the cameras is not connected... \n Reconnect the camera and restart the program.");
    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addWidget(error_message);
    this->setLayout(VBL);
}




