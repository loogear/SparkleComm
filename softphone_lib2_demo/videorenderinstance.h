#ifndef VIDEORENDERINSTANCE_H
#define VIDEORENDERINSTANCE_H
#include "videorender.h"
class VideoRenderInstance:public QObject
{
    Q_OBJECT
public:
    explicit VideoRenderInstance(Softphone_lib * sdk,QObject *parent = 0);

    VideoRender *videoRenderIn() const;

    VideoRender *videoRenderOut() const;
public slots:
private:
    VideoRender *m_videoRenderIn{nullptr};
    VideoRender *m_videoRenderOut{nullptr};
    Softphone_lib *m_phoneSdk;
    bool enableForwardVideo=false;
};
#endif // VIDEORENDERINSTANCE_H
