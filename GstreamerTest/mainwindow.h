#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mediaflow.h"
#include <QPointer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_clicked();

    void on_btnPlay_clicked();

private:
    Ui::MainWindow *_ui;
    QPointer<MediaFlow> _mediaFlow;
    PipelineContainer* container;
    PipelineContainer _data;
    QString _fileName;
};

#endif // MAINWINDOW_H
