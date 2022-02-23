#include "mediaflow.h"
#include <QDebug>

MediaFlow::MediaFlow(QObject *parent) : QObject(parent)
{
}

void MediaFlow::Play(const QString *fileName, guintptr windowHandle)
{
    _container = new PipelineContainer();

    _container->pipeline = gst_pipeline_new("test_pipeline");
    _container->source = gst_element_factory_make("filesrc", "source");
    _container->demuxer = gst_element_factory_make("qtdemux", "demuxer");
    _container->decoder = gst_element_factory_make("avdec_h264", "decoder");
    _container->sink = gst_element_factory_make("glimagesink", "sink");

    _container->videoqueue = gst_element_factory_make("queue", "queuevideo");
    _container->audioqueue = gst_element_factory_make("queue", "queueaudio");
    _container->audiodecoder = gst_element_factory_make("faad", "audioconverter");
    _container->audiosink = gst_element_factory_make("alsasink", "audiosink");

    if (_container->pipeline == NULL ||
        _container->source == NULL ||
        _container->demuxer == NULL ||
        _container->decoder == NULL ||
        _container->sink == NULL)
    {
        qDebug() << "Nie można utworzyć elementu :/";
        return;
    }

    GstBus *bus = gst_element_get_bus (_container->pipeline);
    gst_bus_add_signal_watch(bus);
    g_signal_connect(bus, "message", G_CALLBACK(MediaFlow::cbMessage), _container);

    gst_bin_add_many(GST_BIN(_container->pipeline), _container->source, _container->demuxer, _container->decoder, _container->sink, _container->videoqueue, _container->audioqueue, _container->audiodecoder, _container->audiosink, NULL);

    if (!gst_element_link(_container->source, _container->demuxer))
    {
        qDebug() << "problem z linkowaniem (source - demuxer)";
        gst_object_unref(_container->pipeline);
        return;
    }

    g_signal_connect(_container->demuxer, "pad-added", G_CALLBACK(MediaFlow::padAddedHandler), _container);

    if (!gst_element_link(_container->decoder, _container->videoqueue))
    {
        qDebug() << "problem z linkowaniem (decoder - sink)";
        gst_object_unref(_container->pipeline);
        return;
    }

    if (!gst_element_link(_container->videoqueue, _container->sink))
    {
        qDebug() << "problem z linkowaniem (decoder - sink)";
        gst_object_unref(_container->pipeline);
        return;
    }

    if (!gst_element_link(_container->audiodecoder, _container->audioqueue))
    {
        qDebug() << "problem z linkowaniem (audiodecoder - audioqueue)";
        gst_object_unref(_container->pipeline);
        return;
    }

    if (!gst_element_link(_container->audioqueue, _container->audiosink))
    {
        qDebug() << "problem z linkowaniem (audioqueue - audiosink)";
        gst_object_unref(_container->pipeline);
        return;
    }

    g_object_set(_container->source, "location", fileName->toStdString().c_str(), NULL);

    GstStateChangeReturn gstState = gst_element_set_state(_container->pipeline, GST_STATE_PLAYING);

    if (gstState == GST_STATE_CHANGE_FAILURE)
    {
        qDebug() << "Problem ze startem odtwarzania";
        gst_element_set_state(_container->pipeline, GST_STATE_NULL);
        gst_object_unref(_container->pipeline);
    }

    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(_container->sink), windowHandle);
}

void MediaFlow::padAddedHandler(GstElement *src, GstPad *newPad, PipelineContainer *container)
{
    GstPad *decoderSinkPad = gst_element_get_static_pad(container->decoder, "sink");
    GstPad *audioDecoderSinkPad = gst_element_get_static_pad(container->audiodecoder, "sink");

    qDebug() << QString("Element: %1 --> Pad: %2").arg(gst_element_get_name(src)).arg(gst_pad_get_name(newPad));

    if (gst_pad_is_linked(decoderSinkPad) && gst_pad_is_linked(audioDecoderSinkPad))
    {
        qDebug() << "pad juz podlinkowany";
        return;
    }
    GstCaps *newPadCaps = gst_pad_get_pad_template_caps(newPad);
    QString newPadType = QString(gst_caps_to_string(newPadCaps));

    qDebug() << "Typ pada:" << newPadType;

    GstPadLinkReturn linkReturn;
    if (QString(gst_pad_get_name(newPad)).contains("video"))
    {
        linkReturn = gst_pad_link(newPad, decoderSinkPad);
    }
    else if (QString(gst_pad_get_name(newPad)).contains("audio"))
    {
        linkReturn = gst_pad_link(newPad, audioDecoderSinkPad);
    }

    if (GST_PAD_LINK_FAILED(linkReturn))
    {
        qDebug() << "nie udalo sie polaczyc pada";
    }

    if (newPadCaps != NULL)
    {
        gst_caps_unref(newPadCaps);
    }

    gst_object_unref(decoderSinkPad);
}

void MediaFlow::cbMessage(GstBus *, GstMessage *msg, PipelineContainer *data)
{
    switch (GST_MESSAGE_TYPE (msg))
    {
    case GST_MESSAGE_ERROR:
    {
        GError *err;
        gchar *debug;

        gst_message_parse_error (msg, &err, &debug);
        qDebug() << QString("Error: %1").arg(err->message);
//        g_error_free (err);
//        g_free (debug);

        gst_element_set_state (data->pipeline, GST_STATE_READY);
        break;
    }
    case GST_MESSAGE_EOS:
    {
        gst_element_set_state (data->pipeline, GST_STATE_READY);
        break;
    }
    case GST_MESSAGE_BUFFERING:
    {
        gint percent = 0;
        gst_message_parse_buffering (msg, &percent);
        qDebug() << "Buffering " << percent;

        if (percent < 100)
        {
            gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
        }
        else
        {
            gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
        }
        break;
    }
    case GST_MESSAGE_CLOCK_LOST:
    {
        gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
        gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
        break;
    }
    default:
        break;
    }
}
