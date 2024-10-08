#ifndef EXTRALIB_H
#define EXTRALIB_H
#include "sdkpublic.h"
#include "softphone_lib.h"
class SOFTPHONE_API ExtraLib
{
public:
    ExtraLib(Softphone_lib *lib);
    bool registerToServer(LGR_STRING exraUrl,
                          LGR_STRING serverAddr,
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
    bool unRegister(LGR_STRING exraUrl,
                    LGR_STRING userID,
                    long accountID=0,
                    bool eventReciverOK=true,
                    AcccConfig::REG_OPTION option=AcccConfig::BOTH);

private:
Softphone_lib *phoneLib;
};

#endif // EXTRALIB_H
