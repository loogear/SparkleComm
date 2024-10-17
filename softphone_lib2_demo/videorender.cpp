#include "videorender.h"


#include <QVideoSurfaceFormat>
#include <QDebug>
VideoRender::VideoRender(QObject *parent) : QObject(parent), formatNow(true)
{
}

void VideoRender::present(const QVideoFrame &frame)
{
   emit this->presentSignal(frame);
}

void VideoRender::start(unsigned w, unsigned h, QVideoFrame::PixelFormat format)
{
    emit this->startSignal(w,h,format);
    m_width = w;
    m_height = h;
    m_videoFormat = format;
}

bool VideoRender::isActive()
{
    return true;
}

void VideoRender::put_frame(unsigned char *data, unsigned int len, int width, int height, int bytesPerLine, int format)
{

}

unsigned VideoRender::width() const
{
    return m_width;
}

unsigned VideoRender::height() const
{
    return m_height;
}

QVideoFrame::PixelFormat VideoRender::videoFormat() const
{
    return m_videoFormat;
}

bool VideoRender::getFormatNow() const
{
    return formatNow;
}

void VideoRender::setFormatNow(bool value)
{
    formatNow = value;
}


