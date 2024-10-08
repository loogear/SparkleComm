#ifndef SipEventReciever__H
#define SipEventReciever__H
//#include <QString>
#include "sdkpublic.h"
/*!
 * \brief SipEventReciever  为接口类为底层的回调类，当SDK在有任何的通知信息时，
 * 会通过此类的接口函数回调。如来电提示 @ref callIsIncoming .
 * SIP服务器连接建立成功 @ref sipConnected .
 * 应用开发者需要定义一个扩展自 SipEventReciever 的类
 */
class SipEventReciever
{

public:
    virtual ~SipEventReciever(){

    }
    /*!
     * \brief sipConnected SIP服务器连接建立成功
     * \param accid 账户编号,默认值为0，请参考 @ref multi_account ,@ref accid
     */
    virtual void sipConnected( long accid=0)=0;
    /*!
     * \brief sipDisconnected SIP服务器连接建立断开
     * \param accid 账户编号,默认值为0，请参考 @ref multi_account ,@ref accid
     */
    virtual  void sipDisconnected( long accid=0)=0;
    /*!
     * \brief sipConnectionQualityReport 信号强度报告
     * \param Strength 信号强度值0~5,数值越低说明信号强度越差，0为断开
     * \param accid 账户编号,默认值为0，请参考 @ref multi_account ,@ref accid
     */
    virtual void sipConnectionQualityReport(int Strength,long accid=0){
    }
    /*!
     * \brief callIsAnswserd 呼叫被应答
     * \param callID 呼叫编号，请参考 @ref multi_call ,@ref callid
     * \param accid 账户编号,默认值为 0,请参考 @ref multi_account ,@ref accid
     */
    virtual void callIsAnswserd(LGR_STRING,int callID, long accid=0)=0;
    /*!
     * \brief callIsReleased 呼叫已释放
     * \param callID 呼叫编号，请参考 @ref multi_call ,@ref callid
     * \param causeCode 原因值代码值,10~20,为保留值,为本地服务返回的代码，如账号为空，尚未注册，
     * 其余的值为RFC3261的规范值：如404 为号码是空号。参考值如下
     * 100试呼叫（Trying）
     * 180振铃（Ringing）
     * 181呼叫正在前转（Call is Being Forwarded）
     * 200成功响应（OK）
     * 302临时迁移（Moved Temporarily）
     * 400错误请求（Bad Request）
     * 401未授权（Unauthorized）
     * 403禁止（Forbidden）
     * 404用户不存在（Not Found）
     * 408请求超时（Request Timeout）
     * 480暂时无人接听（Temporarily Unavailable）
     * 486线路忙（Busy Here）
     * 504服务器超时（Server Time-out）
     * 600全忙（Busy Everywhere）
     * \param reasonText 原因的文本描述：如404对应的文本为： Not Found，注：次文本为服务器返回的文本，不通的服务器描述会有差异.
     *
     * \param accid 账户编号,默认值为 0,请参考 @ref multi_account ,@ref accid
     */
    virtual void callIsReleased(LGR_STRING,int callID,int causeCode,LGR_STRING reasonText, long accid=0)=0;
    /*!
     * \brief callIsIncoming 来电通知
     * \param peerAdder 对方的号码/地址
     * \param callID 呼叫ID号，请参考 @ref multi_call ,@ref callid
     * \param accid 账户编号，请参考 @ref multi_account ,@ref accid
     */
    virtual void callIsIncoming(LGR_STRING peerAdder, int callID,long accid=0)=0;

    /*!
     * \brief callMediaAnnounce 媒体协商通知，在呼叫建立后，媒体协商成功后会调用此函数。
     * 注意，媒体协商依赖于网络和对端的响应情况，此函数调用会有延迟。如果协商不成功不会调用此函数。
     * 呼叫会直接挂断
     *\param  peerAddr 对方的地址如电话号码或者URL
     * \param mediaType 媒体类型 参考 @ref MediaType
     * \param callID 呼叫ID号，请参考 @ref multi_call ,@ref callid
     * \param accid 账户编号，请参考 @ref multi_account ,@ref accid
     */
    virtual void callMediaAnnounce(LGR_STRING peerAddr,MediaInfo::MediaType mediaType, int callID,long accid=0)=0;

    /*!
     * \brief callMediaIsRunning 媒体流已经已经建立的通知函数,区别于 @ref callMediaAnnounce 此函数仅仅告知
     * 媒体流已经开始处理运作，应用层面可以据此做界面的操作。一般用户提示用户呼叫已经接通。
     *\param  peerAddr 对方的地址如电话号码或者URL
    * \param callID 呼叫ID号，请参考 @ref multi_call ,@ref callid
     * \param accid 账户编号，请参考 @ref multi_account ,@ref accid
     */
    virtual void callMediaIsRunning(LGR_STRING peerAddr,int callID, long accid=0)=0;

    /*!
     * \brief sipIMMessageInComing 有SIP消息到达
     * \param peerAddr 对方的地址如电话号码或者URL
     * \param msgText SIP消息内容
     * \param callID  呼叫ID号，请参考 @ref multi_call ,@ref callid
     */
    virtual void sipIMMessageInComing(LGR_STRING peerAddr,LGR_STRING msgText,int callID=-1,long=0)=0;

    /*!
     * \brief callIsBeingTransfered 呼叫将要被转移，指对方要将此呼叫转移到另外的地址。
     * \param peerAddr  对方的地址如电话号码或者URL
     * \param destAddr  被转移的新的地址如电话号码或者URL
     * \param callID 呼叫ID号，请参考 @ref multi_call ,@ref callid
     */
    virtual void callIsBeingTransfered(LGR_STRING peerAddr,LGR_STRING destAddr,int callID=-1,long=0)=0;

    /*!
     * \brief sipInfoInComing 通话过程中的SIP INFO 消息
     * \param type SIP INFO type 如对应的SIP消息:Content-Type: application/msml+xml
     * 其type值为application，
     * \param subType SIP INFO subType 如对应的SIP消息:Content-Type: application/msml+xml
     * 其subType值为msml+xml
     * \param body SIP INFO body 消息体
     * \param callID callID 呼叫ID号，请参考 @ref multi_call ,@ref callid
     * \param accID 账户编号，请参考 @ref multi_account ,@ref accid
     */
    virtual void sipInfoInComing(LGR_STRING type,LGR_STRING subType,LGR_STRING body,int callID=-1,long accID=0)=0;
    /*!
     * \brief DTMFInComing 通话过程中的DTMF按键消息
     * \param digit 按键值0～9* #等
     * \param duration 按键时长
     * \param callID callID 呼叫ID号，请参考 @ref multi_call ,@ref callid
     * \param accID 账户编号，请参考 @ref multi_account ,@ref accid
     */
  virtual void DTMFInComing(LGR_STRING digit, unsigned duration, int callID=-1, long accID=0)=0;    /*!
     * \brief onBuddyState 报告好友的状态，如离线
     * \param peerAddr 对方的地址如电话号码或者URL
     * \param stateid 状态ID
     * \param accid 账户编号，请参考 @ref multi_account ,@ref accid
     */
    virtual void onBuddyState(LGR_STRING peerAddr, int stateid,long accid=0)=0;

};

#endif // EVENTRECIEVER_H
