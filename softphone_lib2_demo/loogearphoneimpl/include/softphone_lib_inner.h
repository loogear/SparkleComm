#ifndef SOFTPHONE_LIB_INNER_H
#define SOFTPHONE_LIB_INNER_H
#include "include/softphone_lib_interface.h"
#ifdef WITH_IM_SERVER
#include "im_eventreciever.h"
#include "im_contacteventreciever.h"
#include "im_muceventreciever.h"
#endif
#include "sip_eventreciever.h"
#include "mediacallback.h"
#include "softphoneapi.h"
#include "videodisplay.h"
#include "sdkpublic.h"
class MessagePrivate;
class SipPrivate;
/*!
*@mainpage Softphone_lib API 文档
 *
 * @section contents 目录
 * @ref intro_sec <br>
 * @ref SIP_sec <br>
 * @ref voice_sec <br>
 * @ref video_sec <br>
 * @ref im_sec <br>
 * <br>
*@section intro_sec 简介
* Softphone_lib 是劳格科技开发的一套支持VoIP库，次库的目的是对VoIP应用开发者提供一套清晰简单的开发接口。
*@section SIP_sec SIP API
* SIP API提供音频SIP操作相关的api。
*
* @section voice_sec 音频API部分
*  音频API提供音频操作相关的api。
*
*@section video_sec 视频API部分
* 视频API提供视频操作相关的api。
*
*@section im_sec 即时消息API部分
* 即时消息API提供即时消息操作相关的api。
*
*/
/*!
 * \brief The Softphone_lib 类
 *
 *
 */
class  SoftphoneLibInner:public  SoftphoneLibInterface
{

public:

    explicit   SoftphoneLibInner(void *msgEventRecver=NULL,\
                             void *contactReciever=NULL,\
                             SipEventReciever *sipEventRecver=NULL,SIP_Transport sipTransport=UDP,bool enableAddVidDec=false, bool isIpV6=false);

    ~SoftphoneLibInner();
private:
#ifdef WITH_IM_SERVER
    MessagePrivate *messagePrivate;
#endif
    SipPrivate  *sipPrivate; /**< TODO: describe */
public :
    bool unRegister(long accountID, bool eventReciverOK, AcccConfig::REG_OPTION option) override;
    int makeVoiceCall(LGR_STRING calleeAdd, long accountID) override;
    int makeVoiceCall2(LGR_STRING calleeAdd, LGR_STRING extraData, long accountID) override;
    bool startMcastSender(std::string remote, int port, std::string codec_id, std::string playFile) override;
    bool startMcastReciever(std::string remote, int port, std::string codec_id) override;
    bool stopMcaster(std::string remote, int port, bool isReciever) override;
    std::string getInvitedCallSipHead(std::string headName, int callid, int accid) override;
    bool answerCall(long accountID, int callID) override;
    bool releaseCall(long accountID, int callID) override;
    bool setOnHold(long accountID, int callID) override;
    int startRecord(LGR_STRING filename, long accountID, int callID) override;
    bool stopRecord(int recordId, long accountID, int callID) override;
    bool releaseHold(long accountID, int callID) override;
    bool redirectCall(LGR_STRING targetAddr, long accountID, int callID) override;
    bool xferCall(LGR_STRING targetAddr, long accountID, int callID) override;
    bool developeTestapi(int option, LGR_STRING targetAddr) override;
    bool sendDtmf(LGR_STRING digits, int type, int callID) override;
    bool sendSipInfo(LGR_STRING type, LGR_STRING body, int callID) override;
    bool isSpeakerOn() override;
    bool setSpeakerOn(bool setOn) override;
    bool isMicphoneMuted(int callID) override;
    bool setMicphoneMuted(bool setOn, int callID) override;
    unsigned getOutputVolume() override;
    void setOutputVolume(unsigned volume, bool keep) override;
    unsigned getInputVolume() const override;
    void setInputVolume(unsigned volume, bool keep) override;
    bool getAutoAnswer() const override;
    void setAutoAnswer(bool value) override;
    void playFileToCall(LGR_STRING filenname, int toCallId, long accountID) override;
    void putCallInConferece(long accountID, int fromCallId, int toCallId) override;
    bool addNewCallInConferece(LGR_STRING targetUrl, long accountID, int toCallId) override;
    AviableCallInfoList getAviableCalls() override;
    void enbaleSoudDevice(bool enable) override;
    void keepAlive() override;
    bool sendTextMessageTo(LGR_STRING destAddr, LGR_STRING TextMessage, bool grouptalk) override;
    bool subscribeSipPrensece(LGR_STRING destAddr, long accountID) override;
    void setMediaCallback(MediaCallBack *mediaCallBack) override;
    void videoCodecInit(VIDEO_RESOLUTION resolution, int fps) override;
    CodecList getCodecs() override;
    bool setCodecPriority(std::string codecId, unsigned int pr) override;
    unsigned long accountCounts() override;
    bool registerToServer(AcccConfig acc, long &accountID) override;
    bool threadInitCheck(const char *threadName) override;
    bool registerToSipServer(LGR_STRING serverAddr, int port, LGR_STRING userID, LGR_STRING userPWD, long accountID, LGR_STRING domain, bool authWithDomain, LGR_STRING idUri, bool useIMS3GPP, LGR_STRING extraHeads) override;
    int getAccountRegSipCode(long accountID) override;
    bool getSipAccountByUid(const LGR_STRING &uid, long &accountID) override;
    bool getSipAccountById(LGR_STRING &uid, const long &accountID) override;
    bool registerToServer(LGR_STRING serverAddr, int port, LGR_STRING userID, LGR_STRING userPWD, long accountID, LGR_STRING domain, bool authWithDomain, LGR_STRING idUri, LGR_STRING cellphone, bool useIMS3GPP, LGR_STRING extraHeads) override;
    bool setVideoDisplays(VideoDisplay &prewviewWin, VideoDisplay &incomingWin) override;
    bool setPrewviewDisplays(VideoDisplay &prewviewWin) override;
    bool setIncomingDisplays(VideoDisplay &incomingWin) override;
    bool showOutgoingWindow() override;
    bool showIncominWindow() override;
    bool hideOutgoingWindow() override;
    bool hideIncominWindow() override;
    bool removeIncominVideo() override;
    bool addIncominVideo() override;
    bool changeVideoDeviceToCall(int callID, int deviceid) override;
    bool enableVideo(bool isEnable) override;
    bool showVideoPrevieWindow() override;
    std::string callStaticInfo(long accountID) override;
    int getDefalutCaptureDevice() override;
    bool setDefaultCameraDevice(int deviceID) override;
    int getDefaultCameraDevice() override;
    bool setVideoCaptureOrient(int degree, int deviceID) override;
    CameraDeviceList getCameraDevice() override;
    RenderDeviceList getRenderDevice() override;
    SoundDeviceList getSounDevice() override;
    bool setSoundInputDev(int device_id) override;
    bool setSoundOutputDev(int device_id) override;
    void adjustMicTxLevel(float level) override;
    void adjustPlayerRxLevel(float level) override;
    bool stopPreview() override;
};

#endif // SOFTPHONE_LIB_INNER_H
