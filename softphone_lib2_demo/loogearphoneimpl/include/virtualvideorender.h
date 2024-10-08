#ifndef VirtualVideoRender_H
#define VirtualVideoRender_H
#include <QVideoFrame>
class VirtualVideoRender {
public:
    virtual void present(const QVideoFrame &frame)=0;
    virtual void start(unsigned w,unsigned h,QVideoFrame::PixelFormat format)=0;
    virtual void put_frame(unsigned char *data, unsigned int len, int width, int height, int bytesPerLine, int format)=0;
    virtual bool isActive()=0;
    virtual bool formatLater()=0;
};
#endif
