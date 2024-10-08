#ifndef IM_MUCEVENTRECIEVER_H
#define IM_MUCEVENTRECIEVER_H

#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QJsonDocument>
typedef struct Member{
    std::string jid;
    std::string name;
    QJsonObject  toJson(){
        QJsonObject obj;
        obj.insert("jid",jid.c_str());
        obj.insert("name",name.c_str());

        return obj;
    }

    void fromJson(QJsonObject obj){
      jid=obj.value("jid").toString().toStdString();
      name=obj.value("name").toString().toStdString();
    }
}Member;
typedef struct Room{
    std::string jid;
    std::string name;
    QMap<std::string,Member *> members;
 inline   void addMember(const char *jid,const char *name){
        Member * member=new Member();
        member->jid=jid;
         member->name=name;
         members[jid]=member;
    }
    inline std::string toString(){
        std::string retstr;
       QJsonObject obj= toJson();
       QJsonDocument doc(obj);
         retstr=doc.toJson(QJsonDocument::Compact).toStdString();
        return  retstr;
    };
  inline  QJsonObject  toJson(){
        QJsonObject obj;
        obj.insert("jid",jid.c_str());
        obj.insert("name",name.c_str());
        QJsonArray array;
        for(auto e:members){
            QJsonObject memberObj;
            memberObj.insert(e->jid.c_str(),e->toJson());
            array.append(memberObj);
        }
        obj.insert("members",array);
        return obj;
    }
    void fromJson(QJsonObject obj){
      jid=obj.value("jid").toString().toStdString();
      name=obj.value("name").toString().toStdString();
     QJsonArray array=obj.value("members").toArray();
     for(auto e:array){
         QJsonObject memberObjExt= e.toObject();
         QStringList keys= memberObjExt.keys();
         for(auto key:keys){
           QJsonObject memberObj=  memberObjExt.value(key).toObject();
           Member * member=new Member();
            member->fromJson(memberObj);
            members[key.toStdString()]=member;
         }
     }
    }
    ~Room(){
        for(auto e:members){
            if(e)
            delete  e;
        }
        members.clear();
    }
}Room;
typedef struct Service{
    std::string jid;
    std::string name;
    QMap<std::string,Room*> rooms;
    QJsonObject  toJson(){
        QJsonObject obj;
        obj.insert("jid",jid.c_str());
        obj.insert("name",name.c_str());
        QJsonArray array;
        for(auto e:rooms){
            QJsonObject roomsObj;
            roomsObj.insert(e->jid.c_str(),e->toJson());
            array.append(roomsObj);
        }
        obj.insert("rooms",array);
        return obj;
    }
   void fromJson(const char * jsonStr){
       QJsonDocument doc=QJsonDocument::fromJson(jsonStr);
       fromJson(doc.object());
   }
    void fromJson(QJsonObject obj){
      jid=obj.value("jid").toString().toStdString();
      name=obj.value("name").toString().toStdString();
     QJsonArray array=obj.value("rooms").toArray();
     for(auto e:array){
         QJsonObject roomObjExt= e.toObject();
         QStringList keys= roomObjExt.keys();
         for(auto key:keys){
           QJsonObject roomObj=  roomObjExt.value(key).toObject();
           Room * room=new Room();
            room->fromJson(roomObj);
            rooms[key.toStdString()]=room;
         }
     }
    }
    ~Service(){
        for(auto e:rooms){
            if(e)
            delete  e;
        }
        rooms.clear();
    }
}Service;
class ImMUCEventReciever
{

public:
    virtual  void MucServiceRefresh(QString serviceJson)=0;
    virtual  void MucInviteIncoming(QString roomName, QString serviceName, QString from, QString msg)=0;

};

#endif // IM_MUCEVENTRECIEVER_H
