#include "videoWidget.h"

/* This class is the widget on which the frames from the camera is continuously drawn to.
 * In set_next_frame() of CameraViewWindow or CalibrationWindow, which gets called at a specific frequency,
 * set_frame() and update() is called, which draws the image inputted into set_frame().
 * The drawing of the frames is GPU-accelerated because this class is a derived class of QOpenGLWidget.
 */

VideoWidget::VideoWidget(QWidget *parent)
    : QOpenGLWidget(parent),
    painter(QPainter(this)),
    can_draw_flag(false)
{
}

void VideoWidget::set_frame(const cv::Mat &frame)
{
    q_frame = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888).rgbSwapped();
    can_draw_flag = true;
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    if (can_draw_flag)
    {
        painter.begin(this);
        painter.drawImage(0, 0, q_frame);
        painter.end();
    }
}


