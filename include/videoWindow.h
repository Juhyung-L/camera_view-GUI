#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QTimer>

class VideoWindow : public QWidget
{
private:
    QTimer *timer;
    std::vector<QWidget*> observers;
    int rate;
    virtual void set_next_frame(); // this function is overriden in its child classes
    void notify_start();
    void notify_end();
public:
    VideoWindow(QWidget *parent=nullptr);
    void set_frame_rate(int rate);
    void start_timer();
    void stop_timer();
    void attach(QWidget *observer);
};

#endif // VIDEOWINDOW_H
