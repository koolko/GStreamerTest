#include "pipelinecontainer.h"

PipelineContainer::PipelineContainer(QObject *parent) :
    QObject(parent),
    pipeline(NULL), source(NULL), demuxer(NULL), decoder(NULL), converter(NULL), sink(NULL)
{
}
