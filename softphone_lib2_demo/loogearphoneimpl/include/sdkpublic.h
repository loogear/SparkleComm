
#ifndef SDKPUBLIC_H
#define SDKPUBLIC_H
#include <string>
#include <vector>
#include <list>
#ifdef USING_QT_ENV
    #include <QString>
    #include <QVariant>
#endif

#define ENABEL_SERVICE_MOD

#ifdef _WIN32
#define HAS_SCRENCAPTURE 1
#endif
#ifdef __APPLE__
#include "TargetConditionals.h"
#if  TARGET_OS_IPHONE
#elif  TARGET_OS_MAC
#define HAS_SCRENCAPTURE 1
#endif
#endif

#define SLEEP_TIMEOUT 10

#ifdef _WIN32

 #if defined(BUID_SOFTPHONE)
         #define SOFTPHONE_API__declspec(dllimport)
    #else
        #define SOFTPHONE_API __declspec(dllexport)

   #endif
#else
   #define SOFTPHONE_API
#endif

#ifndef __PRETTY_FUNCTION__
    #define  __PRETTY_FUNCTION__ __func__
#endif

#define INVALID_long -2

#ifdef _WIN32
    #define   typeof decltype
#endif
    #ifdef __linux__
    #define  typeof __typeof__
#endif

//#define  ENDL "["<<__PRETTY_FUNCTION__<<"]"
#define  UNIMPLEMNT qCritical()<<"ERROR！！ You are calling unimplemnt function:";


#ifdef USING_QT_ENV
     typedef  QString LGR_STRING;
    #define L_C(str) str.toStdString().c_str()
    #define  C_L(str) QString::fromStdString(str.toStdString())
#else
    #define LGR_STRING std::string
    #define L_C(str) LString::fromStdString(str)
    #define  C_L(str) str.toStdString()
#endif


bool  serial_string(char **buffer_p,unsigned &len,std::string &var);
bool  de_serial_string(char **buffer_p,unsigned &len,std::string &var);
std::string getStringValue(std::string str,std::string key);
#define STRING_SER_LEN(var) sizeof(unsigned)+var.length()
    typedef enum{
        UDP,
        TCP,
        TLS
    }SIP_Transport;

typedef enum{
    RECORD_BOTH_SINGLE,
    RECORD_BOTH_SEPERATE,
    RECORD_BOTH_ALL,
    RECORD_ICOMING_ONLY,
    RECORD_OUT_ONLY
}Record_type;
/*!
 * \brief The MediaInfo 类定义类呼叫过程中的媒体信息呢
 */
class MediaInfo{
public:
    /*!
     * \brief The MediaType enum 媒体类型,应用程序需依据此类型做相应的处理，如有视频时打开视频显示窗口
     */
    enum MediaType{
        /** 未知类型,初始化值*/
        UNKNOWN,
        /** 媒体类型为只有音频*/
        ONLY_VOICE,
        /** 媒体类型为只有视频*/
        ONLY_VIDEO,
        /** 媒体类型为同时包含音频和视频*/
        VIDEO_VOICE
    };
    /*!
     * \brief The MessageType enum 消息类型
     */
    enum MessageType
    {
      Chat           =  1,        /**< A chat message. */
      Error          =  2,        /**< An error message. */
      Groupchat      =  4,        /**< A groupchat message. */
      Headline       =  8,        /**< A headline message. */
      Normal         = 16,        /**< A normal message. */
      Invalid        = 32         /**< The message stanza is invalid. */
    };
    /*!
     * \brief The MediaTtypeSEP enum SDP协商的媒体类型定义
     */
    enum MediaTtypeSEP
    {
        /** Type is not specified. */
        PJMEDIA_TYPE_NONE,

        /** The media is audio */
        PJMEDIA_TYPE_AUDIO,

        /** The media is video. */
        PJMEDIA_TYPE_VIDEO,

        /** The media is application. */
        PJMEDIA_TYPE_APPLICATION,

        /** The media type is unknown or unsupported. */
        PJMEDIA_TYPE_UNKNOWN

    } ;
    /*!
     * \brief The MediaStatus enum 媒体状态定义
     */
    enum MediaStatus
    {
        /**
        * Call currently has no media, or the media is not used.
        */
        MEDIA_NONE,

        /**
        * The media is active
        */
        MEDIA_ACTIVE,

        /**
        * The media is currently put on hold by local endpoint
        */
        MEDIA_LOCAL_HOLD,

        /**
        * The media is currently put on hold by remote endpoint
        */
        MEDIA_REMOTE_HOLD,

        /**
        * The media has reported error (e.g. ICE negotiation)
        */
        MEDIA_ERROR

    };
    /*!
     * \brief type 媒体协商类型
     */
    MediaTtypeSEP type;
    /*!
     * \brief status 媒体状态
     */
    MediaStatus status;
#ifdef ENABEL_SERVICE_MOD
    unsigned calacSize();
    bool  tobuffer(char * buffer,unsigned len);
    bool  fromBuffer(char * buffer, unsigned len);
#endif
};
typedef std::vector<MediaInfo> MediaInfoList;
typedef struct {
    int deviceId;/*!< 设备ID*/
    std::string name;/*!< 设备名称*/
}CameraDevice;

typedef struct {
    int deviceId;/*!< 设备ID*/
    std::string name;/*!< 设备名称*/
}RenderDevice;
typedef struct {
    int deviceId;/*!< 设备ID*/
    std::string name;/*!< 设备名称*/
    std::string driver;/*!< 驱动名称*/
    int inputCount;
    int outputCount;
    bool active;
}SoundDevice;
typedef enum
{
    CALL_INV_STATE_NULL,	    /**< Before INVITE is sent or received  */
    CALL_INV_STATE_CALLING,	    /**< After INVITE is sent		    */
    CALL_INV_STATE_INCOMING,	    /**< After INVITE is received.	    */
    CALL_INV_STATE_EARLY,	    /**< After response with To tag.	    */
    CALL_INV_STATE_CONNECTING,	    /**< After 2xx is sent/received.	    */
    CALL_INV_STATE_CONFIRMED,	    /**< After ACK is sent/received.	    */
    CALL_INV_STATE_DISCONNECTED,   /**< Session is terminated.		    */
} SDKCallState;
typedef enum{
CALL_ROLE_UAC,	/**< Role is UAC. */
CALL_ROLE_UAS,	/**< Role is UAS. */
}CallRole;


/**
  * @brief The AviableCallInfo class
  */
 class AviableCallInfo{
   public:
    int callId;
    std::string peerUrl;
    long accountId;
    SDKCallState state;
    CallRole role;
    MediaInfoList mediaInfoList;
    long long totalTime;
#ifdef ENABEL_SERVICE_MOD
    unsigned calacSize();
    bool  tobuffer(char * buffer,unsigned len);
    bool  fromBuffer(char * buffer, unsigned len);
#endif
};

typedef std::vector<AviableCallInfo > AviableCallInfoList;

class AviableCallInfoS{
public:
    AviableCallInfoS(AviableCallInfoList list_pra);
    AviableCallInfoS(char * buffer_pra,unsigned size_pra);
    ~AviableCallInfoS();

public:
    unsigned getSize() const;

    char *getBuffer() const;

    bool isValid();

    AviableCallInfoList &getList();

private:
    AviableCallInfoList list;
    #ifdef ENABEL_SERVICE_MOD
    bool toBuffer();
    bool fromBuffer();
   #endif
    char * buffer;
    unsigned size;
    bool valid;
    bool needDelete=false;
};
typedef std::vector<CameraDevice > CameraDeviceList;
typedef std::vector<RenderDevice> RenderDeviceList;

typedef std::vector<SoundDevice> SoundDeviceList;

typedef enum{
    RES_QVGA,//QVGA（320 x 240）
    RES_CIF,// CIF(352 x 288)
    RES_VGA,//VGA (640 x 480)
    RES_4CIF,//4CIF (704 x 576)
    RES_720p,//720p (1280 x 720)
    RES_1080p,//1080p (1920 x 1080)
    RES_MAX
}VIDEO_RESOLUTION;  //Video resolution

typedef struct {
    /**
 * Codec unique identification.
 */
    std::string		codecId;

    /**
 * Codec priority (integer 0-255).
 */
    unsigned		priority;

    /**
 * Codec description.
 */
    std::string		desc;
}CodecInfomation;

typedef std::vector<CodecInfomation> CodecList;

struct Group{
    std::string id ;
    std::string name;
#ifdef WITH_IM_SERVER

  const  inline bool operator==(const Group &b){
        return (this->id==b.id)&&(this->name==b.name);
    }
#endif
};
struct Contact{
public:
    Contact(): from_frend(0), to_frend(0)
    {
    }
    std::string groupid;
    std::string id;
    std::string name;
    std::string phone;
    std::string sex;
    std::string photoPath;
    std::string vcardJson;
    std::string requestMsg;
    std::string address;
    unsigned char from_frend;
    unsigned char to_frend;

    std::list<std::string>  groupList;
#ifdef WITH_IM_SERVER
    inline bool operator==(Contact b){
        return (this->groupid==b.groupid)&&(this->id==b.id)
                &&(this->name==b.name);
    }
#endif
};


#endif // PUBLIC_H



