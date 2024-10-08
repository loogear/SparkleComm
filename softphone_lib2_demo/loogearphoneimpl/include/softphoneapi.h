#ifndef SOFTPHONEAPI_H
#define SOFTPHONEAPI_H
#include "sdkpublic.h"
 //#pragma message "compile..........SOFTPHONEAPI_H.."
struct ServerConfigure {
    LGR_STRING serverAddr="";
    int port;
    LGR_STRING userID;
    LGR_STRING userPWD;
    LGR_STRING domain;
    bool authWithDomain;
    LGR_STRING idUr; //like "sip:account@serviceprovider".
    bool useIMS3GPP;
    SIP_Transport sipTransport;
    LGR_STRING extraHeads=LGR_STRING();
};
struct AcccConfig {
    typedef enum{OnlySIP, OnlyIM, BOTH} REG_OPTION;
    REG_OPTION  option;
    LGR_STRING cellPhone;
    ServerConfigure sip;
    ServerConfigure im;

};
/*
class SoftphoneAPI {
public:
    virtual  bool registerToServer(QString serverAddr,
                                   int port,
                                   QString userID,
                                   QString userPWD,
                                   long &accountID,
                                   QString domain="",
                                   QString cellPhone="")=0;

    virtual  bool registerToServer(AcccConfig &acc,long &accountID)=0;


    virtual unsigned long  accountCounts()=0;
    virtual   QList<Group> getGroupList()=0;
    virtual   QList<Contact> getContactList(QString groupID="")=0;
    virtual   bool sendTextMessageTo(QString destAddr, QString TextMessage)=0;

};
*/
#endif // SOFTPHONEAPI

