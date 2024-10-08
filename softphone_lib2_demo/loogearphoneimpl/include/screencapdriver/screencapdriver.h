#ifndef SCREENCAPDRIVER_H
#define SCREENCAPDRIVER_H
#include "screencapdriver_global.h"
class PrivateCapturerDriver;
class SCREENCAPDRIVER_EXPORT ScreenCaptureDriver
{
public:
    ScreenCaptureDriver();
    ~ScreenCaptureDriver();
    CAPTURE_TYPE getCaptureType() const;
    void setCaptureType(const CAPTURE_TYPE &value);
    CAPTURE_FORMAT getCaptureFormat() const;
    void setCaptureFormat(const CAPTURE_FORMAT &value);

    bool Start();
    void Stop();

    void *getUser_data() const;
    void setUser_data(void *value);

    size_t getWidth() const;
    void setWidth(const size_t &value);

    size_t getHeight() const;
    void setHeight(const size_t &value);

    size_t getFps() const;
    void setFps(const size_t &value);

    CaptueConfigure getConf() const;
    void setConf(const CaptueConfigure &value);

    sreen_capture_op getCapture_op() const;
    void setCapture_op(const sreen_capture_op &value);
private:
PrivateCapturerDriver *p;
};

#endif // SCREENCAPDRIVER_H
