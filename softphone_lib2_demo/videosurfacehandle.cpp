#include "videosurfacehandle.h"
#include <QTimer>
#include "widget.h"
VideoSurfaceHandle::VideoSurfaceHandle(QQuickItem *parent):    QQuickItem(parent)
{
    backGroundImage= new  QImage (":/pics/videobackgroud.png");
    size = QSize(640,480);

}
QSize VideoSurfaceHandle::getSize() const
{
    return size;
}

void VideoSurfaceHandle::setSize(const QSize &value)
{
    size = value;
}

QAbstractVideoSurface *VideoSurfaceHandle::getVideoSurface() const
{
    return videoSurface;
}


void VideoSurfaceHandle::setVideoSurface(QAbstractVideoSurface *videoSurface)
{
    this->videoSurface=videoSurface;
    QList<QVideoFrame::PixelFormat> list=  videoSurface->supportedPixelFormats();
    for(int i=0;i<list.count();i++){
        qDebug()<<"supportedPixelFormats:"<<list.at(i);
    }
    VideoDisplay  W;W.window=videoSurface;
    if(this->windowRole==VideoSurfaceHandle::OUTGOING){
        qDebug()<<"setPrewviewDisplays....";

    Widget::phoneLib->setPrewviewDisplays(W);
    }else{
        qDebug()<<"setIncomingDisplays....";

     Widget::phoneLib->setIncomingDisplays(W);
    }
}

VideoSurfaceHandle::WindowRole VideoSurfaceHandle::getWindowRole() const
{
    return windowRole;
}

void VideoSurfaceHandle::setWindowRole(const WindowRole &value)
{
    if(windowRole==value)return;
    emit windowRoleChanged();
    windowRole = value;
}



void VideoSurfaceHandle::componentComplete()
{

}
