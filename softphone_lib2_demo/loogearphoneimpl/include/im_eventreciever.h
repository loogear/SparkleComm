#ifndef IMEVENTRECIEVER_H
#define IMEVENTRECIEVER_H

#include <QString>
/*!
 * \brief The ImEventReciever class
 */
class ImEventReciever
{

public:
    virtual ~ImEventReciever(){

    }
    virtual void connected()=0;
    virtual  void disconnected()=0;
    virtual  void newMessage (const QString& from, const QString& message,int messagetype)=0;
};

#endif // IMEVENTRECIEVER_H
