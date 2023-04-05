#include <filesystem>
#include "mainWindow.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    stacked_widget(new QStackedWidget(this)),
    tab_widget(new QTabWidget(this)),
    camera_view_tab(new CameraViewTab(this)),
    calibration_tab(new CalibrationTab(this)),
    camera_missing_widget(new CameraMissingWidget(this))
{   
    init_ui();
}

void MainWindow::init_ui()
{
    this->setWindowTitle("Camera-view GUI");
    this->setFixedSize(400, 250);

    tab_widget->addTab(camera_view_tab, "Camera View");
    tab_widget->addTab(calibration_tab, "Calibration");

    stacked_widget->addWidget(tab_widget);
    stacked_widget->addWidget(camera_missing_widget);

    directories::make_directory(); // makes the yaml folder if it doesn't already exist

    // check if both cameras are properly connected
    // if not, display the camera_missing_widget, which shows an error message
    if (!check_camera_connections())
    {
        stacked_widget->setCurrentWidget(camera_missing_widget);
    }
    else
    {
        stacked_widget->setCurrentWidget(tab_widget);
    }

    this->setCentralWidget(stacked_widget);
}

bool MainWindow::check_camera_connections()
{
    if (std::filesystem::exists(directories::LEFT_CAMERA_DIR) &&
        std::filesystem::exists(directories::RIGHT_CAMERA_DIR))
    {
        return true;
    }
    else
    {
        return false;
    }
}





