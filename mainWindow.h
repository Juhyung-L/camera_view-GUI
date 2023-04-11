#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "tabs.h"

class MainWindow : public QWidget
{
private:
    QStackedWidget *stacked_widget;
    QTabWidget* tab_widget;
    CameraViewTab* camera_view_tab;
    CalibrationTab *calibration_tab;
    CameraMissingWidget *camera_missing_widget;
    QPushButton *close_button;
    void init_ui();
    void init_vars();
    bool check_camera_connections();
    void start_camera_view();
public:
    MainWindow(QWidget *parent = nullptr);
};
#endif // MAINWINDOW_H
