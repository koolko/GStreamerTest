#ifndef MEDIAFLOW_H
#define MEDIAFLOW_H

#include "pipelinecontainer.h"
#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QString>
#include <QObject>
#include <QPointer>

class MediaFlow : public QObject
{
    Q_OBJECT

public:
    explicit MediaFlow(QObject *parent = NULL);

    QPointer<PipelineContainer> _container;

    void Play(const QString *fileName, guintptr windowHandle);
    static void padAddedHandler(GstElement *src, GstPad *newPad, PipelineContainer *container);
    static void cbMessage(GstBus *, GstMessage *msg, PipelineContainer *data);
};

#endif // MEDIAFLOW_H
