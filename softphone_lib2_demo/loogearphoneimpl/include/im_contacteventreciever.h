#ifndef IMCONTACTEVENTRECIEVER_H
#define IMCONTACTEVENTRECIEVER_H
#include <QStringList>
#include <QImage>
/*!
 \brief

 \class ImContactEventReciever im_contacteventreciever.h "im_contacteventreciever.h"
*/
class ImContactEventReciever
{
public:
    virtual ~ImContactEventReciever(){

    }
    /*!
     \brief

     \fn delUser
     \param name
     \param id
    */
    virtual   void delUser (const QString& name, const QString& id)=0;
    /*!
     \brief

     \fn presenceChanged
     \param from
     \param connected
    */
    virtual  void presenceChanged (const QString& from, const bool &connected)=0;
    /*!
     \brief

     \fn newUser
     \param name
     \param id
     \param image
    */
    virtual  void newUser (const QString& name, const QString& id, const QImage& image)=0;
    /*!
     \brief

     \fn groupsRevieved
    */
    virtual void groupsRevieved()=0;
    /*!
     \brief

     \fn presenceReceived
     \param uid
     \param name
     \param resource
     \param status
     \param msg
    */
    virtual void presenceReceived(QString uid,QString name,QString resource,int status,QString msg)=0;

    /*!
     \brief

     \fn subScribeRecieved
     \param uid
     \param vcardStr
     \param msg
    */
    virtual void subScribeRecieved(QString uid, QString vcardStr,QString msg)=0;

    /*!
     \brief

     \fn contactInfoIncoming
     \param uid
     \param vcardStr
     \param from_friend
     \param to_friend
     \param groupList
    */
    virtual void contactInfoIncoming(QString uid, QString vcardStr,unsigned char from_friend,unsigned char to_friend,QStringList groupList)=0;

    /*!
     \brief

     \fn owner_vCardInforevieved
     \param vacrdinfo
    */
    virtual void owner_vCardInforevieved(QString vacrdinfo)=0;

};

#endif // IMCONTACTEVENTRECIEVER_H
