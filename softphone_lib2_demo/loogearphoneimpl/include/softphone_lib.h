#ifndef SOFTPHONE_LIB_H
#define SOFTPHONE_LIB_H
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
class SoftphoneLibInterface;


/*!
*@mainpage Softphone_lib API 文档
 *
 * @section contents 目录
 * @ref intro_sec <br>
 * @ref SIP_sec <br>
 * @ref voice_sec <br>
 * @ref video_sec <br>
 * @ref im_sec <br>
 * @ref tips <br>
 * @ref change_log <br>
 * <br>
*@section intro_sec 简介
* SparkleComm 是劳格科技开发的一套支持VoIP库，次库的目的是对VoIP应用开发者提供一套清晰简单的开发接口。
* 提供基于网络的点对点视频通话和语音通话功能，还提供多人视频和音频会议功能，
* 支持通话中音视频设备控制和实时音视频模式切换，支持音视频采集数据回调以实 现美颜和变声等自定义功能。实现音视频通话功能需要用到登录认证服务和实时语 音视频通话服务。音视频通话系统版本最低要求 Android 4.4。
* 核心的两个类 @ref Softphone_lib  以及 @ref SipEventReciever 。
* 其中 @ref Softphone_lib 为应用层 主动发起操作的 API 接口，如发起呼叫。
*  @ref SipEventReciever 为接口类为底层的回调 类，如来电提示
*
* @section 功能介绍
SparkleComm 源于劳格科技多年的即时通讯的实时音视频通话功能的技术积累， 提供稳定流畅、高品质、全平台、高实时的点对点和多人实时音视频通话服务。延 时小于 100ms，网络动态优化，抗 800ms 网络抖动，在网络丢包 30%时仍能正常 通话。采样率最高支持 48KHz，支持全频带编解码，智能语音前处理算法，针对 音乐场景的特殊优化，保证经过网络传输的音乐仍能保持 CD 音质。
  音视频服务适用于各种实时音视频场景，比如社交行业的视频聊天、视频交友、教
  育行业的小班化教学、点对点视频教学、企业内部的多人会议、远程医疗、游戏语
  音等等。
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
*@section   refs 术语说明
*
* @subsection multi_account 多账号管理
* 多账号管理指的是在同一个实例中可以同时存在多个账号，每一个账号可以单独的操作。
* 如：同时将11001\@loogear.com,和11002\@loogear.com两个账号注册到服务器上。
* 可以接听分别来自于此两个账号的来电，在呼出时也可以指定从哪个账号呼出。
* 参考 @ref accid 。
*
*@subsection accid 账号编号accid
*SparkleComm Client suite 支持多账号登陆,accid为注册到服务器后返回的账号编号，在
* 多账号模式下，的后续操作均与此编号相关，类型为整数，默认值为0,在只注册一个账户是此值永远为0。
* 参考 @ref multi_account 。
*
* @subsection multi_call 多呼叫管理
* 多呼叫管理指的是在单个或者多个账号的情况下,同时存在几个呼叫的管理。
* 如登陆后发起呼叫到10086，在尚未挂断的情况下再吃呼叫10010,
* 此时针对这个两个呼叫会有两个不同的 @ref callid 。
*
* @subsection callid 呼叫编号callid
* 呼叫编号callid是用来识别不同呼叫的编号，如登陆后发起呼叫到10086，在尚未挂断的情况下再吃呼叫10010,
* 此时针对这个两个呼叫会有两个不同的 @ref callid 。后续的针对这个两个不同的呼叫的操作，
* 比如挂断，需要依据 @ref callid 来识别。参考 @ref multi_call
*@subsection 关于截屏同能及接口的介绍1
* 参考@ref ScreenCapture
*@section tips	注意事项：
*1.	Softphone_lib的部分操作会触发网络数据发送，故商用业务不要在主线程中执行。 <br>
*2.	Softphone_lib的语音操作上述描述已经足够，应用无需关心音频的录制及播放。只需要通过回调函数关心呼叫状态即可。涉及视频操作尚需要窗口的初始化操作，本文档未描述。 <br>
*3.	完整的应用详细描述请参考demo。 <br>
*
*@section change_log 变更记录
*
*@subsection  change_log1 变更1(2018年12月27日)
*  新增接口组播接口，包括发送及接收；增加获取呼叫消息头的接口. <br>
*    @ref Softphone_lib#startMcastReciever 发送组播接口 <br>
*    @ref Softphone_lib#startMcastSender 接收组播接口 <br>
*    @ref Softphone_lib#stopMcaster 停止接收发送 <br>
*    @ref Softphone_lib#getInvitedCallSipHead 获取SIP呼叫消息头部 <br>
*/
typedef enum {
   BUILDIN,
   SEPERATE_CLIENT,
   SEPERATE_SERVER
}PROCCESS_TYPE;
/*!
 * \brief The Softphone_lib 类 Softphone_lib 为应用层主动发起操作的核心API接口，如发起呼叫，挂断等等。
 * 注意事项:
* Softphone_lib 的部分操作会触发网络数据发送，故商用业务不要在主 线程中执行。
* Softphone_lib 的语音操作上述描述已经足够，应用无需关心音频的录 制及播放。只需要通过回调函数关心呼叫状态即可。涉及视频操作尚 需要窗口的初始化操作，本文档未描述。 完整的应用详细描述请参考 demo。
 *
 *
 */
 class SOFTPHONE_API Softphone_lib
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
    explicit   Softphone_lib(ImEventReciever *msgEventRecver=NULL, \
                             ImContactEventReciever *contactReciever=NULL, \
                             SipEventReciever *sipEventRecver=NULL, SIP_Transport sipTransport=UDP, bool enableAddVidDec=false, bool isIpV6=false, PROCCESS_TYPE process_type=BUILDIN);
#else
    explicit   Softphone_lib(void *msgEventRecver=NULL, \
                             void *contactReciever=NULL, \
                             SipEventReciever *sipEventRecver=NULL, SIP_Transport sipTransport=UDP, bool enableAddVidDec=false, bool isIpV6=false, PROCCESS_TYPE process_type=BUILDIN);
#endif
      bool waitTerminate();
    /**
     * @brief
     *
     */
    ~Softphone_lib();
private:

    SoftphoneLibInterface  *sipInterface;
    PROCCESS_TYPE m_proccess_type;



public :
    /**
     * @brief unRegister 注销
     * @param accountID  当前账号
     * @param eventReciverOK 指消息接受回调的指针是否还可用(SipEventReciever ImContactEventReciever ImEventReciever)
     * 默认是可用。当eventReciverOK为false回调指针不可以用，系统不再回调这这函数。为false应用场景为应用程序退出时，这些指针已经被销毁无法
     * 再用
     * @param option 注册选项
     * @return true为成功 false失败
     */
    bool unRegister(long accountID=0, bool eventReciverOK=true,  AcccConfig::REG_OPTION option=AcccConfig::BOTH);
    /**
     * @brief makeVoiceCall 发起语音呼叫
     * @param calleeAdd  对方号码
     * @param accountID 呼叫帐号编号(用于多账户注册)
     * @return 返回Call ID>=0为成功  -1失败
     */
    int  makeVoiceCall(LGR_STRING calleeAdd,long accountID=0);
    /**
     * @brief makeVoiceCall 发起带扩展消息的语音呼叫
     * @param calleeAdd  对方号码
     * @param extraData 扩展消息格式标准为"X-HEAD_NAME1:head1 value;X-HEAD_NAME2:head2 value",头部消息间用";"分隔:
     * 如:"X-SparkleComm-Call-DEVICE-ID: EF32D-98AF-9E01-882E;X-SparkleComm-Call-MSG2:this is other message"
     * @param accountID 呼叫帐号编号(用于多账户注册)
     * @return 返回Call ID>=0为成功  -1失败
     */
    int  makeVoiceCall2(LGR_STRING calleeAdd,LGR_STRING extraData,long accountID=0);

    /**
     * @brief startMcastSender 发起语音组播,在组播的地址和端口范围内发起语音组播。参考 @ref startMcastReciever 以及  @ref stopMcaster
     * @param remote 组播的地址 如如"224.0.0.251"或"224.0.1.111" 组播的地址是否能够被成功，尤其是在wifi环境下与路由器/AP设置有关，
     * "224.0.0.251"在目前大多数路由或AP都是支持的，
     * @param port  组播的端口 如"4001"
     * @param codec_id 组播的语音编码 如"AMR/8000/1",可以传空值"",空置将由系统自动选取
     * @param playFile 是否是播放文件，播放文件目前支持.wav格式PCM，要播放的文件名,WAV文件必须格式化为16位PCM单声道.
     * @return 返回是否成功值
     */
    bool  startMcastSender(std::string remote, int port, std::string codec_id, std::string playFile);
    /**
     * @brief stopMcaster 停止接受/发送组播
     * @param remote 组播的地址 如"224.0.0.251"或"224.0.1.111" 组播的地址是否能够被成功，尤其是在wifi环境下与路由器/AP设置有关，
     * "224.0.0.251"在目前大多数路由或AP都是支持的， 需要与发送或者接收时的参数一致 参考 @ref startMcastReciever 以及 @ref startMcastSender
     * @param port   组播的端口 如"4001" 需要与发送或者接收时的参数一致 参考 @ref startMcastReciever 以及 @ref startMcastSender
     * @param isReciever 是否是组播的接收端，false为发送，true为接收
     * @return 返回是否成功值
     */
    bool  stopMcaster(std::string remote, int port, bool isReciever);

    /**
     * @brief startMcastReciever 开始接受语音的组播，接受组播需要指定组播的地址。 参考 @ref startMcastSender 以及  @ref stopMcaster
     * @param remote 组播的地址 如"224.0.1.111"，需要与发送方的组播地址一致
     * @param port 组播的端口 如"4001"，需要与发送方的组播端口一致
     * @param codec_id 组播的语音编码 如"AMR/8000/1",可以传空值"",空值将由系统自动选取，需要与发送方的组播端口一致
     * @return 返回是否成功值
     */
    bool  startMcastReciever(std::string remote, int port,std::string codec_id);

    /**
     * @brief getInvitedCallSipHead 获取来电呼叫的SIP消息头的值
     * @param headName SIP消息头名字如"X-SparkleComm-Call-DEVICE-ID"
     * @param callid 呼叫id @ref callid 来识别。参考 @ref multi_call
     * @param accid 账户编号，请参考 @ref multi_account ,@ref accid
     * @return 返回值，如果不存在则返回为空置""
     */
    std::string getInvitedCallSipHead(std::string headName,int callid,int accid);

    /**
     * @brief answerCall 接听电话
     * @param accountID 呼叫帐号编号 默认为0
     * @param callID    呼叫编号 默认为-1，默认时，寻找当前的呼叫(最后进来或者发起的呼叫)进行操作，
     * 如果默认呼叫无效返回失败。默认值仅建议在单账号操作时使用（避免频繁传递参数），多账号时无法保证
     * 当前呼叫的有效性。answerCall，releaseCall，setOnHold等等操作等均如此
     * @return  true为成功 false失败
     */
    bool answerCall(long accountID=0,int callID=-1);
    /**
     * @brief startRecord 开始录音
     * @param filename  录音文件名称(绝对路径)
     * @param accountID 呼叫帐号编号 默认为0
     * @param callID 呼叫编号 默认为-1
     * @return 返回录音的ID号
     */
    int startRecord(LGR_STRING filename,long accountID=0,int callID=-1);
    /**
     * @brief startRecord 停止录音
     * @param recordId  录音编号
     * @param accountID 呼叫帐号编号 默认为0
     * @param callID 呼叫编号 默认为-1
     * @return 操作是否成功
     */
    bool stopRecord(int recordId,long accountID=0,int callID=-1);

    /**
     * @brief releaseCall 挂断电话
     * @param accountID 呼叫帐号编号 默认为0
     * @param callID    呼叫编号 默认为-1，默认时，寻找当前的呼叫(最后进来或者发起的呼叫)进行操作，
     * 如果默认呼叫无效返回失败。默认值仅建议在单账号操作时使用（避免频繁传递参数），多账号时无法保证
     * 当前呼叫的有效性
     * @return  true为成功 false失败
     */
    bool releaseCall(long accountID=0,int callID=-1);

    /*!
     * \brief setOnHold 保持呼叫
     * \param accountID 账户编号，请参考 @ref multi_account ,@ref accid
     * \param callID 呼叫编号 请参考 @ref multi_call ,@ref callid
     * \return true为成功 false失败
     */
    bool setOnHold(long accountID=0,int callID=-1);

    /**
     * @brief
     *
     * @param accountID 取消保持呼叫
     * \param accountID 账户编号，请参考 @ref multi_account ,@ref accid
     * \param callID 呼叫编号 请参考 @ref multi_call ,@ref callid
     * \return true为成功 false失败
     */
    bool releaseHold(long accountID=0, int callID=-1);



    /**
     * @brief referCall 转接电话 区别于 @ref xferCall
     * @param targetAddr 目标地址
     * @param accountID 呼叫帐号编号 默认为0，账户编号，请参考 @ref multi_account ,@ref accid
     * @return true为成功 false失败
     */
    bool redirectCall(LGR_STRING targetAddr, long accountID=0,int callID=-1);

    /**
     * @brief xferCall 电话偏转 区别于 @ref referCall
     * @param targetAddr 目标地址
     * @param accountID 呼叫帐号编号 默认为0，账户编号，请参考 @ref multi_account ,@ref accid
     * @return true为成功 false失败
     */
    bool xferCall(LGR_STRING targetAddr, long accountID=0,int callID=-1);

    bool developeTestapi(int option, LGR_STRING targetAddr);



    /*!
     * \brief sendDtmf 发送dtmf
     * \param digits dtmf串
     * \param type 0--inband,1--SIPINFO,2--rfc2833
     * \return true为成功 false失败
     */
    bool  sendDtmf(LGR_STRING digits, int type=0,int callID=-1);

    /*!
     * \brief sendSipInfo 发送SIP INFO 消息
     * \param type SIP INFO type 如type=application/msml+xml
     *  对应的SIP消息:Content-Type: application/msml+xml
     * \param body SIP INFO body
     * \return true为成功 false失败
     */
    bool sendSipInfo(LGR_STRING type,LGR_STRING body,int callID=-1);

    /**
     * @brief isSpeakerOn 扬声器状态查询
     * @return true为打开 false关闭
     */
    bool  isSpeakerOn();
    /**
     * @brief setSpeakerOn  打开／关闭扬声器
     * @param setOn true为打开 false关闭
     * @return true为成功 false失败
     */
    bool  setSpeakerOn(bool setOn);

    /*!
     * \brief isMicphoneMuted MIC是否打开
     * \return rue为打开 false关闭
     */
    bool isMicphoneMuted(int callID=-1);
    /**
     * @brief setMicphoneMuted 设置MIC静音
     *
     * @param setOn rue为打开 false关闭
     * @return bool true为成功 false失败
     */
    bool setMicphoneMuted(bool setOn,int callID=-1);

    /**
     * @brief getOutputVolume 获取音量大小数值
     *
     * @return unsigned 值为0~100的数字
     */
    unsigned getOutputVolume();
    /**
     * @brief setOutputVolume 设置音量大小数值
     *
     * @param volume 值为0~100的数字
     * @param keep 是否保持，即此参数是否保留到下一个呼叫
     */
    void setOutputVolume(unsigned volume, bool keep=true) ;
    /**
     * @brief getInputVolume 获取MIC音量大小数值
     *
     * @return unsigned  值为0~100的数字
     */
    unsigned getInputVolume() const ;
    /**
     * @brief setInputVolume 设置MIC音量大小数值
     *
     * @param volume 值为0~100的数字
     * @param keep 是否保持，即此参数是否保留到下一个呼叫
     */
    void setInputVolume(unsigned volume, bool keep=true);

    /**
     * @brief getAutoAnswer 当前是否为自动应答状态
     *
     * @return bool true为打开 false关闭
     */
    bool getAutoAnswer() const;
    /**
     * @brief setAutoAnswer 设置为自动应答状态
     *
     * @param value true为打开 false关闭
     */
    void setAutoAnswer(bool value);

    /**
     * @brief playFileToCall 给当前的呼叫播放文件
     *
     * @param filenname 播放的文件名
     * @param toCallId 呼叫编号 请参考 @ref multi_call ,@ref callid
     * @param accountID 呼叫帐号编号 默认为0，账户编号，请参考 @ref multi_account ,@ref accid
     */
    void playFileToCall( LGR_STRING filenname,int toCallId=-1,long accountID=0);

    /**
     * @brief putCallInConferece 将所有的呼叫放置到会议中
     *
     * @param accountID 呼叫帐号编号 默认为0，账户编号，请参考 @ref multi_account ,@ref accid
     * @param fromCallId (保留,未使用)呼叫编号 请参考 @ref multi_call ,@ref callid
     * @param toCallId (保留,未使用)呼叫编号 请参考 @ref multi_call ,@ref callid
     */
    void putCallInConferece(long accountID=0, int fromCallId=-1, int toCallId=-1);

    /**
     * @brief addNewCallInConferece 加入一路呼叫到会议中
     *
     * @param accountID 呼叫帐号编号 默认为0，账户编号，请参考 @ref multi_account ,@ref accid
     * @param targetUrl 对方的电话号码
     * @param toCallId呼叫编号 请参考 @ref multi_call ,@ref callid
     */
    bool  addNewCallInConferece(LGR_STRING targetUrl,long accountID=0,int toCallId=-1);

    /**
     * @brief getAviableCalls 获取当前活跃的呼叫数
     *
     * @return AviableCallInfoList
     */
    AviableCallInfoList getAviableCalls();

    void  enbaleSoudDevice(bool enable);

#ifdef WITH_IM_SERVER
    /**
     * @brief acceptSubScribe 接受/拒绝邀请加入好友(接受/拒绝订阅)
     * @param uid  对方的ID
     * @param accept Bool型参数，true 为接受，false为拒绝
     * @return 返回成功与失败
     */
    bool acceptSubScribe(LGR_STRING uid, bool accept=true);
    /**
     * @brief subScribe 邀请对方为好友(订阅对方的状态信息)
     * @param uid 对方的ID
     * @param msg 附加发送的消息
     * @return 返回成功与失败
     */
    bool subScribe(LGR_STRING uid, LGR_STRING sub_msg);

    /**
     * @brief unSubScribe 删除好友(取消订阅对方的状态信息)
     * @param uid 对方的ID
     * @param msg 附加发送的消息
     * @return 返回成功与失败
     */
    bool unSubScribe(LGR_STRING uid,LGR_STRING msg);
    void uploadVCardInfo(LGR_STRING vcardInfo);

    bool createMucRoom(LGR_STRING roomName, LGR_STRING myNickname="", LGR_STRING invites="");
    bool destoryRoom(LGR_STRING roomName);
    bool inviteToRoom(LGR_STRING roomName, LGR_STRING invites);
    bool kickFromRoom(LGR_STRING roomName, LGR_STRING memberID);

    void setRoomHumanName(LGR_STRING roomName, LGR_STRING newName);

    // SoftphoneAPI interface
public:
    QList<Group> getGroupList();
    QList<Contact> getContactList(LGR_STRING groupID="");
#endif
public:
    /**
     * @brief
     *
     */
    void keepAlive();
    // SoftphoneAPI interface
public:
    /**
     * @brief sendTextMessageTo 发送即时消息
     *
     * @param destAddr 目标地址
     * @param TextMessage 消息文本
     * @param grouptalk 是否为群聊
     * @return bool 发送是否成功
     */
    bool sendTextMessageTo(LGR_STRING destAddr, LGR_STRING TextMessage,bool grouptalk=false);

    /**
     * @brief subscribeSipPrensece 订阅好友状态
     *
     * @param destAddr 目标地址
     * @param accountID 呼叫帐号编号 默认为0，账户编号，请参考 @ref multi_account ,@ref accid
     * @return bool 是否成功
     */
    bool subscribeSipPrensece(LGR_STRING destAddr,long accountID=0);

    /**
     * @brief setMediaCallback 特殊的媒体回调函数
     *
     * @param mediaCallBack
     */
    void setMediaCallback(MediaCallBack * mediaCallBack);
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
    void videoCodecInit(VIDEO_RESOLUTION resolution,int fps=15);

    /*!
     * \brief getCodecs 获取语音编码列表
     * \return CodecList \a CodecList
     */
    CodecList getCodecs();
    /*!
     * \brief setCodecPriority 设置语音编码优先级参数。设置方式：首先使用\a getCodecs，获取当前
     * 支持的编码列表，然后设置不同编码的参数。
     * \param codecId 编码id如"PCMU/8000/1"
     * \param pr  优先级参数从0-255,数值越大优先级越高。
     * \return 成功或者失败
     */
    bool setCodecPriority(std::string codecId, unsigned int pr);

    // SoftphoneAPI interface
public:
    /**
     * @brief accountCounts 获取当前的账号总数
     *
     * @return unsigned long
     */
    unsigned long accountCounts();

    // SoftphoneAPI interface
public:
    /**
     * @brief registerToServer
     *
     * @param acc 账号结构体 @ref AcccConfig
     * @param accountID 帐号编号 默认为0，账户编号，请参考 @ref multi_account ,@ref accid
     * @return bool 成功与否
     */
    bool registerToServer(AcccConfig acc, long &accountID);
    /*!
     * \brief registerToSipServer 注册到SIP服务器
     * \param serverAddr 服务器地址IP地址或者域名
     * \param port SIP服务器端口号
     * \param userID 用户ID如10011\@loogear.com或者10011
     * \param userPWD 注册密码
     * \param accountID 账号编号此处传入0
     * \param domain 域名如loogear.com，参考服务器的分配域名可能与serverAddr相同，也可能不同
     * \param authWithDomain 是否带域名认证，默认为false。参考服务器的要求。
     * \param idUri 参考服务器的注册要求，默认为空
     * \param useIMS3GPP 是否采用IMS的3gpp认证，参考服务器的注册要求
     * @param extraHeads 扩展消息格式标准为"X-HEAD_NAME1:head1 value;X-HEAD_NAME2:head2 value",头部消息间用";"分隔:
     * 如:"X-SparkleComm-Call-DEVICE-ID: EF32D-98AF-9E01-882E;X-SparkleComm-Call-MSG2:this is other message"
     * \return 返回成功与与失败，注意，并非注册成功与否而是函数调用是否成功。
     */
    bool registerToSipServer(LGR_STRING serverAddr,
                                         int port,
                                         LGR_STRING userID,
                                         LGR_STRING userPWD, long accountID,
                                         LGR_STRING domain,
                                         bool authWithDomain,
                                          LGR_STRING idUri,
                                          bool useIMS3GPP,
                                         LGR_STRING extraHeads=LGR_STRING());
    /*!
     * \brief registerToImServer  注册到即时消息服务器
     * \param serverAddr 服务器地址IP地址或者域名
     * \param port 服务器端口号
     * \param userID 用户ID如10011\@loogear.com或者10011
     * \param userPWD 注册密码
     * \param domain 域名如loogear.com，参考服务器的分配域名可能与serverAddr相同，也可能不同
     * \param cellphone 手机号码，此参数为扩展参数，默认填写userID相同值
     * \return 返回成功与与失败，注意，并非注册成功与否而是函数调用是否成功。
     */
    bool registerToImServer(LGR_STRING serverAddr,
                                         int port,
                                         LGR_STRING userID,
                                         LGR_STRING userPWD,
                                         LGR_STRING domain, LGR_STRING cellphone);

    /*!
     * \brief threadInitCheck 线程检查
     * \param threadName 线程名字
     * \return 返回成功与与失败
     */
    bool threadInitCheck(const char * threadName);

    /*!
     * \brief getAccountRegSipCode 获取SIP注册返回代码
     * \param accountID   帐号编号 默认为0，账户编号，请参考 @ref multi_account ,@ref accid
     * \return 整数的代码值如200,404.
     */
    int getAccountRegSipCode(long accountID=0);
    /**
     * @brief getSipAccountByUid 根据UID获取账户编号
     *
     * @param uid 字符串的uid如1011\@loogear.com
     * @param accountID 账户编号，请参考 @ref multi_account ,@ref accid
     * @return bool 返回成功与与失败
     */
  #ifdef JAVA_NATIVE
    long getSipAccountByUid(const LGR_STRING uid);
  #else
    bool getSipAccountByUid(const LGR_STRING &uid,long &accountID);
  #endif
    /**
     * @brief getSipAccountById  根据账户编号获取UID
     *
     * @param uid uid 字符串的uid如1011\@loogear.com
     * @param accountID 账户编号，请参考 @ref multi_account ,@ref accid
     * @return bool 返回成功与与失败
     */
#ifdef JAVA_NATIVE
  LGR_STRING getSipAccountById( const long accountID);
#else
    bool getSipAccountById( LGR_STRING &uid,const long &accountID);
#endif
    /**
     * @brief registerToServer 此函数为混合注册函数即即时消息与SIP,内部使用
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
#ifdef WITH_IM_SERVER
    bool registerToServer(LGR_STRING serverAddr,
                          int port,
                          LGR_STRING userID,
                          LGR_STRING userPWD, long accountID,
                          LGR_STRING domain="",
                          bool authWithDomain=true,
                          LGR_STRING idUri="",
                          LGR_STRING cellphone="",
                          bool useIMS3GPP=false,
                          LGR_STRING extraHeads=LGR_STRING()
            );
#else
    bool registerToServer(LGR_STRING serverAddr,
                          int port,
                          LGR_STRING userID,
                          LGR_STRING userPWD, long accountID,
                          LGR_STRING domain="",
                          bool authWithDomain=false,
                          LGR_STRING idUri="",
                          LGR_STRING cellphone="",
                          bool useIMS3GPP=false,
                          LGR_STRING extraHeads=LGR_STRING()
            );
#endif


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
     *      phoneLib->setVideoDisplays(prVwin,inComingVwin);
     *    @endcode
     * \param prewviewWin 本地预览窗口的句柄，此句柄设置后会保存。设置后方可调用\a showVideoPrevieWindow()
     * \param incomingWin 来电窗口的句柄，此句柄设置后会保存。
     * \return
     */
    bool setVideoDisplays(VideoDisplay &prewviewWin, VideoDisplay &incomingWin);
    /*!
     * \brief setPrewviewDisplays 设置预览视频显示窗口句柄
     * \param prewviewWin 窗口对象，请参考 @ref VideoDisplay
     * \return 成功与否
     */
    bool setPrewviewDisplays(VideoDisplay &prewviewWin);

    /**
     * @brief setIncomingDisplays 设置来电视频显示窗口句柄
     *
     * @param incomingWin 窗口对象，请参考 @ref VideoDisplay
     * @return bool  成功与否
     */
    bool setIncomingDisplays(VideoDisplay &incomingWin);

    /**
     * @brief showOutgoingWindow 显示发出的视频
     *
     * @return bool  成功与否
     */
    bool showOutgoingWindow();

    /**
     * @brief showIncominWindow 显示接入的视频
     *
     * @return bool  成功与否
     */
    bool showIncominWindow();

    /**
     * @brief hideOutgoingWindow 隐藏发出的视频
     *
     * @return bool 成功与否
     */
    bool hideOutgoingWindow();
    /**
     * @brief hideIncominWindow 隐藏接入的视频
     *
     * @return bool 成功与否
     */
    bool hideIncominWindow();

    /**
     * @brief removeIncominVideo 删除接入的视频
     *
     * @return bool 成功与否
     */
    bool removeIncominVideo();
    /**
     * @brief addIncominVideo 添加接入的视频
     *
     * @return bool 成功与否
     */
    bool addIncominVideo();

    bool changeVideoDeviceToCall(int callID, int deviceid);

    /**
     * @brief enableVideo 是否启用视频
     *
     * @param isEnable true为启用，false为关闭
     * @return bool 成功与否
     */
    bool enableVideo(bool isEnable);
    /*!
     * \brief showVideoPrevieWindow 显示预览窗口,仅在还没有通话的时候显示用
     * \return 成功/失败,如果没有启用视频会返回失败.其他异常也会返回失败
     */
    bool showVideoPrevieWindow();
    /*!
     * \brief callStaticInfo 获取呼叫状态文本，调试用接口
     * \param accountID 呼叫帐号编号(用于多账户注册)
     * \return 返回的文本串
     */
    std::string callStaticInfo(long accountID=0);

    /**
     * @brief getDefalutCaptureDevice 获取当前默认的音频MIC设备编号
     *
     * @return int 设备编号
     */
    int getDefalutCaptureDevice();
    /**
     * @brief setDefaultCameraDevice 设置当前摄像头设备编号
     *
     * @param deviceID 设备编号
     * @return bool 操作成功与否
     */
    bool setDefaultCameraDevice(int deviceID=-1);

    /**
     * @brief getDefaultCameraDevice 获取当前摄像头设备编号
     *
     *
     * @return bool deviceID 设备编号
     */
    int  getDefaultCameraDevice();

    /**
     * @brief setVideoCaptureOrient 设置视频摄像头的旋转方向
     *
     * @param degree 旋转角度0~360
     * @param deviceID 设备编号
     * @return bool 操作成功与否
     */
    bool setVideoCaptureOrient(int degree,int deviceID=-1);


    /**
     * @brief getCameraDevice 获取摄像头设备列表
     *
     * @return CameraDeviceList 设备列表参考 @ref CameraDeviceList
     */
    CameraDeviceList getCameraDevice();
    /**
     * @brief RenderDeviceList 获取视频显示设备列表
     *
     * @return RenderDeviceList 设备列表参考 @ref RenderDeviceList
     */
    RenderDeviceList getRenderDevice();

    /*!
     * \brief getSounDevice 获取音频设备列表
     * \return  SoundDeviceList 设备列表参考 @ref SoundDeviceList
     */
    SoundDeviceList  getSounDevice();

    /*!
     * \brief setSoundInputDev 设置MIC设备
     * \param device_id 设备编号
     * \return 操作成功与否
     */
    bool setSoundInputDev(int device_id);
    /*!
     * \brief setSoundOutputDev 设置音频输出设备
     * \param device_id 设备编号
     * \return 操作成功与否
     */
    bool setSoundOutputDev(int device_id);
    /**
     * @brief stopPreview 关闭自视窗口
     *
     * @return bool 操作成功与否
     */
    bool stopPreview();

    void  adjustMicTxLevel(float level);
    void  adjustPlayerRxLevel(float level);

#ifdef WITH_IM_SERVER
    /*!
     * \brief setMucEventreciever 设置群聊接受回调类
     * \param reciver 对象句柄 参考 @ref ImMUCEventReciever
     */
    void setMucEventreciever(ImMUCEventReciever * reciver);
#endif
};

#endif // SOFTPHONE_LIB_H
