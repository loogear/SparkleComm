#ifndef EVENTRECIEVER_H
#define EVENTRECIEVER_H

#include <QString>
#include <QImage>
class EventReciever
{

public:
    virtual void connected()=0;
    virtual  void disconnected()=0;
    virtual   void delUser (const QString& name, const QString& id)=0;
    virtual  void newMessage (const QString& from, const QString& message)=0;
    virtual  void presenceChanged (const QString& from, const bool &connected)=0;
    virtual  void newUser (const QString& name, const QString& id, const QImage& image)=0;

    virtual void groupsRevieved()=0;
    virtual void presenceReceived(QString uid,QString name,QString resource,int status,QString msg)=0;
};

#endif // EVENTRECIEVER_H
