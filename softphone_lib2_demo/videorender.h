#ifndef VIDEORENDER_H
#define VIDEORENDER_H

#include <QObject>
#include "virtualvideorender.h"
#include <QVideoFrame>
class Softphone_lib;
class VideoRender : public QObject,public VirtualVideoRender
{
    Q_OBJECT
public:
    explicit VideoRender(QObject *parent = 0);

    // VirtualVideoRender interface
    void present(const QVideoFrame &frame);
    void start(unsigned w, unsigned h, QVideoFrame::PixelFormat format);
    bool isActive();    

    void put_frame(unsigned char *data, unsigned int len, int width, int height, int bytesPerLine, int format);
    bool formatLater(){ return !formatNow;}
    unsigned width() const;
    unsigned height() const;
    QVideoFrame::PixelFormat videoFormat() const;
    bool getFormatNow() const;
    void setFormatNow(bool value);

signals:
    void presentSignal(const QVideoFrame &frame);
    void startSignal(unsigned w, unsigned h, QVideoFrame::PixelFormat format);

private:
    unsigned m_width;
    unsigned m_height;
    QVideoFrame::PixelFormat m_videoFormat;
    bool formatNow;
};



#endif // VIDEORENDER_H
