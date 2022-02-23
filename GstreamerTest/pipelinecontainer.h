#ifndef PIPELINECONTAINER_H
#define PIPELINECONTAINER_H

#include <gst/gstelement.h>
#include <QObject>

class PipelineContainer : public QObject
{
    Q_OBJECT

public:
    explicit PipelineContainer(QObject *parent = NULL);

    GstElement *pipeline;
    GstElement *source;
    GstElement *demuxer;
    GstElement *decoder;
    GstElement *converter;
    GstElement *sink;
    GstElement *videoqueue;
    GstElement *audioqueue;
    GstElement *audiosink;
    GstElement *audiodecoder;
};

#endif // PIPELINECONTAINER_H
