#ifndef PHONEASSTANT_H
#define PHONEASSTANT_H

#include <QString>
#include "softphoneapi.h"
#include "sdkpublic.h"
#define ASS_ACCOUNT_NAME "phoneassistant"
class Softphone_lib;
 class SOFTPHONE_API PhoneAsstant
{
public:
    PhoneAsstant();
    static QString formatRegisterMsg(AcccConfig &acc);
    static QString getRegisterJsonMsg(QString json);

    static QString formatOfflineMsg(QString &accuid);
    static QString getOfflineMsg(QString json);

   static QString formatIncomingCallMsg(AcccConfig &tmpAcc,QString callerID);
   static QString getIncomingCallJsonMsg(QString json);

    static QString getRegisterAssAccout(AcccConfig &acc);
    static bool checkOfflineMod(AcccConfig &acc,QByteArray &offlineData,QString serverAddr);

    static bool offlineReady(AcccConfig &acc, QString accuid,QString serverAddr);

    static bool offlineEnd(AcccConfig &acc, QString accuid,QString serverAddr);

    static bool registerToPhoneAsstantServer(AcccConfig &acc, QByteArray &offlineData, Softphone_lib *phoneLib);

};

#endif // PHONEASSTANT_H
