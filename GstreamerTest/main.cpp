#include "mainwindow.h"
#include <QApplication>
#include <gst/gst.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    gst_init(&argc, &argv);

    w.show();

    return a.exec();
}
