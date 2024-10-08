#ifndef SOFTPHONE_LIB_INTERFACE_H
#define SOFTPHONE_LIB_INTERFACE_H
#include "sip_eventreciever.h"
#include "mediacallback.h"
#include "softphoneapi.h"
#include "videodisplay.h"
#include "sdkpublic.h"


class  SoftphoneLibInterface
{

public:
    /**
     * @brief Softphone Lib的主构造函数，构造函有三个可选的传入三个参数。分别用于：
     * 即时消息、联系人、SIP事件的接收
     *
     * @param msgEventRecver 是即时消息的回调函数类的实例，默认值NULL。
     * @param contactReciever 是联系人的回调函数类的实例，默认值NULL。
     * @param sipEventRecver 是SIP事件的回调函数类的实例，默认值NULL。
     * @param enableAddVidDec 是否启用附加的video设备(使用video 是用Qt Surface 是需要设置为true)，默认值false
     */
#ifdef WITH_IM_SERVER
    explicit   SoftphoneLibInterface(ImEventReciever *msgEventRecver=NULL, \
                             ImContactEventReciever *contactReciever=NULL, \
                             SipEventReciever *sipEventRecver=NULL,SIP_Transport sipTransport=UDP, bool enableAddVidDec=false, bool isIpV6=false)=0;
#else
        SoftphoneLibInterface(void *msgEventRecver=NULL,\
                             void *contactReciever=NULL,\
                              SipEventReciever *sipEventRecver=NULL,SIP_Transport sipTransport=UDP,bool enableAddVidDec=false, bool isIpV6=false){};
 #endif
    /**
     * @brief
     *
     */
      virtual  ~SoftphoneLibInterface(){};


    /**
     * @brief unRegister 注销
     * @param accountID  当前账号
     * @param eventReciverOK 指消息接受回调的指针是否还可用(SipEventReciever ImContactEventReciever ImEventReciever)
     * 默认是可用。当eventReciverOK为false回调指针不可以用，系统不再回调这这函数。为false应用场景为应用程序退出时，这些指针已经被销毁无法
     * 再用
     * @return true为成功 false失败
     */
  virtual  bool unRegister(long accountID=0, bool eventReciverOK=true,  AcccConfig::REG_OPTION option=AcccConfig::BOTH)=0;
    /**
     * @brief makeVoiceCall 发起语音呼叫
     * @param calleeAdd  对方号码
     * @param accountID 呼叫帐号编号(用于多账户注册)
     * @return 返回Call ID>=0为成功  -1失败
     */
    virtual  int  makeVoiceCall(LGR_STRING calleeAdd,long accountID=0)=0;
    virtual  int  makeVoiceCall2(LGR_STRING calleeAdd,LGR_STRING extraData,long accountID=0)=0;

    virtual  bool  startMcastSender(std::string remote, int port,  std::string codec_id, std::string playFile)=0;
    virtual  bool  startMcastReciever(std::string remote, int port, std::string codec_id)=0;
    virtual  bool  stopMcaster(std::string remote, int port, bool isReciever)=0;

    virtual  std::string getInvitedCallSipHead(std::string headName,int callid,int accid)=0;

    /**
     * @brief answerCall 接听电话
     * @param accountID 呼叫帐号编号 默认为0
     * @param callID    呼叫编号 默认为-1，默认时，寻找当前的呼叫(最后进来或者发起的呼叫)进行操作，
     * 如果默认呼叫无效返回失败。默认值仅建议在单账号操作时使用（避免频繁传递参数），多账号时无法保证
     * 当前呼叫的有效性。answerCall，releaseCall，setOnHold等等操作等均如此
     * @return  true为成功 false失败
     */
    virtual  bool answerCall(long accountID=0,int callID=-1)=0;
    /**
     * @brief releaseCall 挂断电话
     * @param accountID 呼叫帐号编号 默认为0
     * @param callID    呼叫编号 默认为-1，默认时，寻找当前的呼叫(最后进来或者发起的呼叫)进行操作，
     * 如果默认呼叫无效返回失败。默认值仅建议在单账号操作时使用（避免频繁传递参数），多账号时无法保证
     * 当前呼叫的有效性
     * @return  true为成功 false失败
     */
    virtual  bool releaseCall(long accountID=0,int callID=-1)=0;

    virtual  bool setOnHold(long accountID=0,int callID=-1)=0;

    virtual  int startRecord(LGR_STRING filename,long accountID=0,int callID=-1)=0;
   virtual   bool stopRecord(int recordId,long accountID=0,int callID=-1)=0;

    /**
     * @brief
     *
     * @param accountID
     * @param callID
     * @return bool
     */
    virtual  bool releaseHold(long accountID=0, int callID=-1)=0;



    /**
     * @brief referCall 转接电话
     * @param targetAddr 目标地址
     * @param accountID 呼叫帐号编号 默认为0
     * @return true为成功 false失败
     */
    virtual  bool redirectCall(LGR_STRING targetAddr, long accountID=0,int callID=-1)=0;
    virtual  bool xferCall(LGR_STRING targetAddr, long accountID=0,int callID=-1)=0;

    virtual  bool developeTestapi(int option, LGR_STRING targetAddr)=0;



    /*!
     * \brief sendDtmf 发送dtmf
     * \param digits dtmf串
     * \param type 0--inband,1--SIPINFO,2--rfc2833
     * \return true为成功 false失败
     */
    virtual  bool  sendDtmf(LGR_STRING digits, int type=0,int callID=-1)=0;

    /*!
     * \brief sendSipInfo 发送SIP INFO 消息
     * \param type SIP INFO type 如type=application/msml+xml
     *  对应的SIP消息:Content-Type: application/msml+xml
     * \param body SIP INFO body
     * \return true为成功 false失败
     */
    virtual  bool sendSipInfo(LGR_STRING type,LGR_STRING body,int callID=-1)=0;

    /**
     * @brief isSpeakerOn 扬声器状态查询
     * @return true为打开 false关闭
     */
    virtual  bool  isSpeakerOn()=0;
    /**
     * @brief setSpeakerOn  打开／关闭扬声器
     * @param setOn true为打开 false关闭
     * @return true为成功 false失败
     */
    virtual  bool  setSpeakerOn(bool setOn)=0;

    virtual  bool isMicphoneMuted(int callID=-1)=0;
    /**
     * @brief
     *
     * @param setOn
     * @return bool
     */
   virtual   bool setMicphoneMuted(bool setOn,int callID=-1)=0;
     /**
     * @brief
     *
     * @return unsigned
     */
    virtual  unsigned getOutputVolume()=0;
    /**
     * @brief
     *
     * @param volume
     * @param keep
     */
   virtual   void setOutputVolume(unsigned volume, bool keep=true)=0;
    /**
     * @brief
     *
     * @return unsigned
     */
   virtual   unsigned getInputVolume() const =0;
    /**
     * @brief
     *
     * @param volume
     * @param keep
     */
    virtual  void setInputVolume(unsigned volume, bool keep=true)=0;

    /**
     * @brief
     *
     * @return bool
     */
    virtual  bool getAutoAnswer() const=0;
    /**
     * @brief
     *
     * @param value
     */
   virtual   void setAutoAnswer(bool value)=0;

    /**
     * @brief
     *
     * @param filenname
     * @param toCallId
     * @param accountID
     */
    virtual  void playFileToCall( LGR_STRING filenname,int toCallId=-1,long accountID=0)=0;



    /**
     * @brief
     *
     * @param accountID
     * @param fromCallId
     * @param toCallId
     */
   virtual   void putCallInConferece(long accountID=0, int fromCallId=-1, int toCallId=-1)=0;

   virtual   bool  addNewCallInConferece(LGR_STRING targetUrl,long accountID=0,int toCallId=-1)=0;

    /**
     * @brief
     *
     * @return AviableCallInfoList
     */
   virtual   AviableCallInfoList getAviableCalls()=0;

   virtual    void  enbaleSoudDevice(bool enable)=0;


    /**
     * @brief
     *
     */
    virtual  void keepAlive()=0;


    /**
     * @brief
     *
     * @param destAddr
     * @param TextMessage
     * @return bool
     */
   virtual  bool sendTextMessageTo(LGR_STRING destAddr, LGR_STRING TextMessage,bool grouptalk=false)=0;

    /**
     * @brief
     *
     * @param destAddr
     * @param accountID
     * @return bool
     */
   virtual  bool subscribeSipPrensece(LGR_STRING destAddr,long accountID=0)=0;

    /**
     * @brief
     *
     * @param mediaCallBack
     */
   virtual  void setMediaCallback(MediaCallBack * mediaCallBack)=0;
    /*!
     * \brief videoCodecInit 初始化视频参数
     * \param resolution 参见 \a VIDEO_RESOLUTION
     * RES_QVGA,//QVGA（320 x 240）
     * RES_CIF,// CIF(352 x 288)
     * RES_VGA,//VGA (640 x 480)
     * RES_4CIF,//4CIF (704 x 576)
     * RES_720p,//720p (1280 x 720)
     * RES_1080p//1080p (1920 x 1080)
     * \param fps fps 默认值是15
     */
   virtual  void videoCodecInit(VIDEO_RESOLUTION resolution,int fps=15)=0;

    /*!
     * \brief getCodecs 获取语音编码列表
     * \return CodecList \a CodecList
     */
   virtual  CodecList getCodecs()=0;
    /*!
     * \brief setCodecPriority 设置语音编码优先级参数。设置方式：首先使用\a getCodecs，获取当前
     * 支持的编码列表，然后设置不同编码的参数。
     * \param codecId 编码id如"PCMU/8000/1"
     * \param pr  优先级参数从0-255,数值越大优先级越高。
     * \return 成功或者失败
     */
   virtual  bool setCodecPriority(std::string codecId, unsigned int pr)=0;


    /**
     * @brief
     *
     * @return unsigned long
     */
    virtual unsigned long accountCounts()=0;


    /**
     * @brief
     *
     * @param acc
     * @param accountID
     * @return bool
     */
   virtual  bool registerToServer(AcccConfig acc, long &accountID)=0;
  virtual   bool threadInitCheck(const char * threadName)=0;

   virtual  bool registerToSipServer(LGR_STRING serverAddr,
                                         int port,
                                         LGR_STRING userID,
                                         LGR_STRING userPWD, long accountID,
                                         LGR_STRING domain, bool authWithDomain,
                                      LGR_STRING idUri, bool useIMS3GPP,
                              LGR_STRING extraHeads=LGR_STRING())=0;

  virtual  int getAccountRegSipCode(long accountID=0)=0;
    /**
     * @brief
     *
     * @param uid
     * @param accountID
     * @return bool
     */
  #ifdef JAVA_NATIVE
   virtual  long getSipAccountByUid(const LGR_STRING uid)=0;
  #else
   virtual  bool getSipAccountByUid(const LGR_STRING &uid,long &accountID)=0;
  #endif
    /**
     * @brief
     *
     * @param uid
     * @param accountID
     * @return bool
     */
#ifdef JAVA_NATIVE
 virtual  LGR_STRING getSipAccountById( const long accountID)=0;
#else
  virtual   bool getSipAccountById( LGR_STRING &uid,const long &accountID)=0;
#endif
    /**
     * @brief
     *
     * @param serverAddr
     * @param port
     * @param userID
     * @param userPWD
     * @param accountID
     * @param domain
     * @param idUri
     * @param cellphone
     * @param useIMS3GPP
     * @return bool
     */

  virtual   bool registerToServer(LGR_STRING serverAddr,
                          int port,
                          LGR_STRING userID,
                          LGR_STRING userPWD, long accountID,
                          LGR_STRING domain="",
                          bool authWithDomain=false,
                          LGR_STRING idUri="",
                          LGR_STRING cellphone="",
                          bool useIMS3GPP=false,
                          LGR_STRING extraHeads=LGR_STRING()
            )=0;

    /*!
     * \brief setVideoDisplays 设置视频窗口句柄,不同操作系统设置方式分为两类情况
     * 1.Qt(不是QuckTime的QT)开发环境下,操作系统支持：Android/macOS/IOS/WINDOWS/Linux,传递QAbstractVideoSurface
     * 2.Android为SurfaceView的对象,具体参见java接口，macOS/IOS/WINDOWS/Linux的本地窗口，传递其对应本地窗口句柄:
     * 举例如下：
     *   A:macOS/IOS:
     *   @code   NSWindow *prewin = [(NSView*)preViewWin->winId() window];
     *      NSWindow *incomingwin = [(NSView*)callWin->winId() window];
     *      VideoDisplay prVwin;   prVwin.window=prewin;
     *      VideoDisplay inComingVwin;inComingVwin.window=incomingwin;
     *      phoneLib->setVideoDisplays(prVwin,inComingVwin)=0;
     *    @endcode
     * \param prewviewWin 本地预览窗口的句柄，此句柄设置后会保存。设置后方可调用\a showVideoPrevieWindow()
     * \param incomingWin 来电窗口的句柄，此句柄设置后会保存。
     * \return
     */
   virtual  bool setVideoDisplays(VideoDisplay &prewviewWin, VideoDisplay &incomingWin)=0;
    /*!
     * \brief setPrewviewDisplays
     * \param prewviewWin
     * \return
     */
   virtual  bool setPrewviewDisplays(VideoDisplay &prewviewWin)=0;

    /**
     * @brief
     *
     * @param incomingWin
     * @return bool
     */
  virtual   bool setIncomingDisplays(VideoDisplay &incomingWin)=0;

    /**
     * @brief
     *
     * @return bool
     */
  virtual   bool showOutgoingWindow()=0;
    /**
     * @brief
     *
     * @return bool
     */
   virtual  bool showIncominWindow()=0;

    /**
     * @brief
     *
     * @return bool
     */
   virtual  bool hideOutgoingWindow()=0;
    /**
     * @brief
     *
     * @return bool
     */
   virtual  bool hideIncominWindow()=0;

    /**
     * @brief
     *
     * @return bool
     */
   virtual  bool removeIncominVideo()=0;
    /**
     * @brief
     *
     * @return bool
     */
  virtual   bool addIncominVideo()=0;

   virtual  bool changeVideoDeviceToCall(int callID, int deviceid)=0;


    /**
     * @brief
     *
     * @param isEnable
     * @return bool
     */
   virtual  bool enableVideo(bool isEnable)=0;
    /*!
     * \brief showVideoPrevieWindow 显示预览窗口,仅在还没有通话的时候显示用
     * \return 成功/失败,如果没有启用视频会返回失败.其他异常也会返回失败
     */
virtual     bool showVideoPrevieWindow()=0;
 virtual    std::string callStaticInfo(long accountID=0)=0;

    /**
     * @brief
     *
     * @return int
     */
   virtual  int getDefalutCaptureDevice()=0;
    /**
     * @brief
     *
     * @param deviceID
     * @return bool
     */
  virtual   bool setDefaultCameraDevice(int deviceID=-1)=0;
    /**
     * @brief getDefaultCameraDevice
     * @return
     */
  virtual   int  getDefaultCameraDevice()=0;
    /**
     * @brief
     *
     * @param degree
     * @param deviceID
     * @return bool
     */
  virtual   bool setVideoCaptureOrient(int degree,int deviceID=-1)=0;


    /**
     * @brief
     *
     * @return CameraDeviceList
     */
   virtual  CameraDeviceList getCameraDevice()=0;
    /**
     * @brief
     *
     * @return RenderDeviceList
     */
  virtual   RenderDeviceList getRenderDevice()=0;

    /*!
     * \brief getSounDevice
     * \return
     */
   virtual  SoundDeviceList  getSounDevice()=0;

    /*!
     * \brief setSoundInputDev
     * \param device_id
     * \return
     */
  virtual   bool setSoundInputDev(int device_id)=0;
    /*!
     * \brief setSoundOutputDev
     * \param device_id
     * \return
     */
  virtual  bool setSoundOutputDev(int device_id)=0;
  virtual   void  adjustMicTxLevel(float level)=0;
  virtual   void  adjustPlayerRxLevel(float level)=0;
    /**
     * @brief
     *
     * @return bool
     */
   virtual  bool stopPreview()=0;

};

#endif // SOFTPHONE_LIB_INTERFACE_H
