#include <QVBoxLayout>
#include <experimental/filesystem>
#include "mainWindow.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
    stacked_widget(new QStackedWidget(this)),
    tab_widget(new QTabWidget(this)),
    camera_view_tab(new CameraViewTab(this)),
    calibration_tab(new CalibrationTab(this)),
    camera_missing_widget(new CameraMissingWidget(this)),
    close_button(new QPushButton("Close", this))
{   
    init_ui();
}

void MainWindow::init_ui()
{
    this->setWindowTitle("Camera-view GUI");
    this->showFullScreen();

    connect(close_button, &QPushButton::clicked, this, &MainWindow::close);

    tab_widget->addTab(camera_view_tab, "Camera View");
    tab_widget->addTab(calibration_tab, "Calibration");

    stacked_widget->addWidget(tab_widget);
    stacked_widget->addWidget(camera_missing_widget);

    QVBoxLayout *VBL = new QVBoxLayout(this);
    VBL->addWidget(stacked_widget);
    VBL->addWidget(close_button);

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
	start_camera_view(); // automatically start the camera view
    }
}

bool MainWindow::check_camera_connections()
{
    if (std::experimental::filesystem::exists(directories::LEFT_CAMERA_DIR) &&
        std::experimental::filesystem::exists(directories::RIGHT_CAMERA_DIR))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::start_camera_view()
{
	camera_view_tab->start_camera_view();
}





