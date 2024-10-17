#include "videosurfacehandle.h"
#include "videorender.h"
#include "videorenderinstance.h"

#include <QVideoSurfaceFormat>
#include <QDebug>
VideoSurfaceHandle::VideoSurfaceHandle(DISP_OBJECT *parent):    DISP_OBJECT(parent)
{
    qRegisterMetaType<QVideoFrame>("QVideoFrame");
    backGroundImage= new  QImage (":/pc/images/PC/logo1.png");
    connect(this,&VideoSurfaceHandle::fristFrame,this,&VideoSurfaceHandle::timeout,Qt::QueuedConnection);

    m_timer.setInterval(1000);
    connect(&m_timer,&QTimer::timeout,this,&VideoSurfaceHandle::timeout,Qt::QueuedConnection);
    connect(this,&VideoSurfaceHandle::startSignal,this,&VideoSurfaceHandle::startSlot,Qt::QueuedConnection);
    this->m_timer.start();
}

void VideoSurfaceHandle::timeout()
{
    QVideoFrame frame(*backGroundImage);
    if(videoSurface)
        this->videoSurface->present(frame);
}

QAbstractVideoSurface *VideoSurfaceHandle::getVideoSurface() const
{
    return videoSurface;
}

void VideoSurfaceHandle::setVideoSurface(QAbstractVideoSurface *videoSurface)
{
    this->videoSurface = videoSurface;
    videoSurface->start(QVideoSurfaceFormat(
                            backGroundImage->size(),
                            QVideoFrame::pixelFormatFromImageFormat(backGroundImage->format())));
    emit this->fristFrame();
}

void VideoSurfaceHandle::presentSlot(const QVideoFrame &frame)
{
    if(m_timer.isActive()) m_timer.stop();
    if(videoSurface){
        if(frame.size()!=videoSurface->surfaceFormat().frameSize()||frame.pixelFormat()!=videoSurface->surfaceFormat().pixelFormat()){
            qDebug()<<"presentSlot video size changed";
            this->videoSurface->stop();
            videoSurface->start(QVideoSurfaceFormat(frame.size(),frame.pixelFormat()));
        }
        videoSurface->present(frame);
        qDebug()<<"presentSlot videoSurface ok";
    }else{
        qWarning()<<"presentSlot videoSurface not setted";
    }
}

void VideoSurfaceHandle::startSlot(unsigned w, unsigned h, QVideoFrame::PixelFormat format)
{
    if(videoSurface){
        this->videoSurface->stop();
        this->videoSurface->start(QVideoSurfaceFormat(QSize(w,h),format));
    }
}

VideoRenderInstance *VideoSurfaceHandle::renderInstace() const
{
    return m_renderInstace;
}

void VideoSurfaceHandle::setRenderInstace(VideoRenderInstance *newRenderInstace)
{
    if (m_renderInstace == newRenderInstace)
        return;
    m_renderInstace = newRenderInstace;
    emit renderInstaceChanged();
}

VideoSurfaceHandle::WindowRole VideoSurfaceHandle::getWindowRole() const
{
    return m_windowRole;
}

void VideoSurfaceHandle::setWindowRole(const WindowRole &windowRole)
{



    m_windowRole = windowRole;
    if(m_windowRole == WindowRole::INCOMING)
    {
        connect(m_renderInstace->videoRenderIn(),&VideoRender::presentSignal,
                this,&VideoSurfaceHandle::presentSlot,Qt::QueuedConnection);
        connect(m_renderInstace->videoRenderIn(),&VideoRender::startSignal,
                this,&VideoSurfaceHandle::startSlot,Qt::QueuedConnection);
    }
    else
    {
        connect(m_renderInstace->videoRenderOut(),&VideoRender::presentSignal,
                this,&VideoSurfaceHandle::presentSlot,Qt::QueuedConnection);
        connect(m_renderInstace->videoRenderOut(),&VideoRender::startSignal,
                this,&VideoSurfaceHandle::startSlot,Qt::QueuedConnection);
    }
}

void VideoSurfaceHandle::windowDeactivateEvent()
{
    qDebug()<<"windowDeactivateEvent..... ";
}

void VideoSurfaceHandle::componentComplete()
{

}
