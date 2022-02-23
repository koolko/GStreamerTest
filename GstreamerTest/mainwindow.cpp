#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _mediaFlow(new MediaFlow(this))
{
    _ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::on_toolButton_clicked()
{
    _fileName = QFileDialog::getOpenFileName(this,
        tr("Open Video"), "/home/koolko", tr("Image Files (*.mp4)"));

    if (!_fileName.isEmpty())
    {
        _ui->lePath->setText(_fileName);
    }
}

void MainWindow::on_btnPlay_clicked()
{
    if (!_fileName.isEmpty() && QFile(_fileName).exists())
    {
        _mediaFlow->Play(&_fileName, (guintptr)this->_ui->frameMedia->winId());
    }
}
