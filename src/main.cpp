#include <QApplication>
#include <QFile>
#include <QString>
#include "include/mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set the theme
//    QFile theme_file("/home/juhyung/Desktop/camera-view_GUI/theme/Gravira.qss");
//    theme_file.open(QFile::ReadOnly);
//    QString theme = QLatin1String(theme_file.readAll());
//    a.setStyleSheet(theme);

    MainWindow w;
    return a.exec();
}
