#include <QVBoxLayout>
#include <QString>
#include <QFont>
#include <experimental/filesystem>
#include <iostream>
#include "include/tabs.h"

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
    start_button->setFont(QFont("Arial", 50));
    undistort_checkbox->setFont(QFont("Arial", 50));
    undistort_checkbox->setStyleSheet("QCheckBox::indicator { width: 30px; height: 30px; }");

    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addWidget(start_button);
    VBL->addWidget(undistort_checkbox, 0, Qt::AlignHCenter);
    VBL->setContentsMargins(100, 100, 100, 100);

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
    if (std::experimental::filesystem::exists(directories::LEFT_YAML_FILE) &&
        std::experimental::filesystem::exists(directories::RIGHT_YAML_FILE))
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
    dropdown_menu(new QComboBox(this)),
    calibration_status_message(new QLabel(this))
{
    init_ui();
    calibration_window->attach(parent);
}

void CalibrationTab::init_ui()
{
    calibration_window->set_frame_rate(10);
    connect(calibration_button, &QPushButton::clicked, this, &CalibrationTab::start_calibration);
    connect(calibration_window, &CalibrationWindow::calibration_status, this, &CalibrationTab::set_calibration_status);

    calibration_status_message->setFont(QFont("Arial", 30));
    dropdown_menu->setFont(QFont("Arial", 50));
    calibration_button->setFont(QFont("Arial",50));

    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addWidget(calibration_status_message, 0, Qt::AlignCenter);
    VBL->addWidget(dropdown_menu);
    VBL->addWidget(calibration_button);
    VBL->setContentsMargins(100, 100, 100, 100);
    dropdown_menu->addItem(QString::fromStdString(cameras::LEFT_CAMERA.name));
    dropdown_menu->addItem(QString::fromStdString(cameras::RIGHT_CAMERA.name));
}

void CalibrationTab::set_calibration_status(bool status)
{
    if (status)
    {
        calibration_status_message->setText("Calibration Success");
        calibration_status_message->setStyleSheet("QLabel { color : green; }");
    }
    else
    {
        calibration_status_message->setText("Calibration Failed");
        calibration_status_message->setStyleSheet("QLabel { color : red; }");
    }
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
    error_message->setFont(QFont("Arial", 20));
    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addWidget(error_message, 0, Qt::AlignCenter);
}




