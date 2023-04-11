#include "videoWindow.h"

/* This class is the base class for both CameraViewWindow and CalibrationWindow.
 * It is a parentless window that pops up to show the view of the camera.
 * A QTimer calls the function set_next_frame() at a specified frequency (eg. 30times/sec = 30fps).
 * set_next_frame() must be overriden in the derived classes to draw the frame retrieved from a camera.
 * While this window is opened, the main window is disabled.
 */


VideoWindow::VideoWindow(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer(this))
{
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, &QTimer::timeout, this, &VideoWindow::set_next_frame);
}

void VideoWindow::set_frame_rate(int frame_rate)
{
    rate = 1000 / frame_rate;
}

void VideoWindow::start_timer()
{
    notify_start();
    timer->start(rate);
    this->activateWindow();
    this->showFullScreen();
}

void VideoWindow::stop_timer()
{
    timer->stop();
    notify_end();
}

void VideoWindow::attach(QWidget *observer)
{
    observers.push_back(observer);
}

void VideoWindow::notify_start()
{
    for (QWidget* observer : observers)
    {
        observer->setEnabled(false);
    }
}

void VideoWindow::notify_end()
{
    for (QWidget* observer : observers)
    {
        observer->setEnabled(true);
    }
}

void  VideoWindow::set_next_frame()
{
}



