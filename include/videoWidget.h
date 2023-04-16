#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/core/mat.hpp>

class VideoWidget : public QOpenGLWidget
{
private:
    QPainter painter;
    QImage q_frame;
    bool can_draw_flag;
    void paintEvent(QPaintEvent *event) override;
public:
    VideoWidget(QWidget *parent=nullptr);
    void set_frame(const cv::Mat& frame);
};

#endif // VIDEOWIDGET_H
