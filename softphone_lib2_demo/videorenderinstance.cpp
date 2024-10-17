#include "videorenderinstance.h"
#include "softphone_lib.h"
VideoRenderInstance::VideoRenderInstance(Softphone_lib * sdk,QObject *parent ):QObject(parent)
{
    this->m_phoneSdk=sdk;

    this->m_videoRenderIn= new VideoRender();
    this->m_videoRenderOut= new VideoRender();

    this->m_phoneSdk->enableVideo(true);


    //进来的帧
    VideoDisplay  windowIn;
    windowIn.window=dynamic_cast<VirtualVideoRender *>(this->m_videoRenderIn);
    this->m_phoneSdk->setIncomingDisplays(windowIn);
    this->m_phoneSdk->showIncominWindow();

    //出去的帧
    VideoDisplay  windowOut;
    windowOut.window=dynamic_cast<VirtualVideoRender *>(this->m_videoRenderOut);
    this->m_phoneSdk->setPrewviewDisplays(windowOut);
#ifndef TV_MEETING_VERSION
    this->m_phoneSdk->showOutgoingWindow();
#endif
}

VideoRender *VideoRenderInstance::videoRenderIn() const
{
    return m_videoRenderIn;
}

VideoRender *VideoRenderInstance::videoRenderOut() const
{
    return m_videoRenderOut;
}
