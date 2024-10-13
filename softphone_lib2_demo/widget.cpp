#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>
#include <QBuffer>
#include <QQuickItem>
//#include <sphvideodecoder.h>
#include "videosurfacehandle.h"
#ifdef Q_OS_DARWIN64
#import<Cocoa/Cocoa.h>
#endif

#define HANGUP "Hangup Call"
#define MAKECALL "Make Call"
#define ANSWERCALL "Answer Call"
   Softphone_lib * Widget::phoneLib=NULL;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    preViewWin=NULL;
    callWin=NULL;
    ui->setupUi(this);
    phoneLib =new Softphone_lib(0,0,this,TCP);
    connect(this,SIGNAL(showRreuqest(QString,QString)),
            this,SLOT(showRreuqestSlot(QString,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(newMessageSignal(QString,QString)),
            this,SLOT(newMessageSlot(QString,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(mediaIsRunning()),SLOT(mediaIsRunningSlot()),Qt::QueuedConnection);
    // qmlRegisterType<SPHVideoDecoder>("com.loogear.softphone", 1, 0, "SPHVideoDecoder");
    // qmlRegisterType<VideoSurfaceHandle>("com.loogear.softphone", 1, 0, "VideoSurfaceHandle");

 }

Widget::~Widget()
{
    phoneLib->unRegister();
    delete ui;
}




void Widget::on_registerButton_clicked()
{
    QString userID=this->ui->accountEdit->text();
    QString userPWD="0J072n75Jyt2hLvGdarPFw==";


    this->phoneLib->enableVideo(true);
    AcccConfig acc;
    long accountID=-2;
    acc.sip.domain="system.loogear.com";
    acc.sip.userID=userID;
    acc.sip.userPWD=userPWD;
#if 1
    acc.sip.serverAddr="as3.loogear.com";
    acc.sip.port=41825;
#else
    acc.sip.serverAddr="192.168.0.110";
    acc.sip.port=41825;
#endif
    acc.sip.authWithDomain=false;
    acc.sip.useIMS3GPP=false;
    acc.option=AcccConfig::OnlySIP;

  bool ret=  this->phoneLib->registerToServer(acc,accountID);
    qDebug()<<"registerToServer:"<<ret;

}

void Widget::showRreuqestSlot(QString uid, QString msg)
{
    QMessageBox msgBox;
    msgBox.setText("请求加为好友");
    msgBox.setInformativeText(uid+"请求加你为好友，并发来消息："+msg+"要接受吗");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch(ret){
    case QMessageBox::Yes:
        //this->phoneLib->acceptSubScribe(uid,true);
        break;
    case QMessageBox::No:
        // this->phoneLib->acceptSubScribe(uid,false);

        break;
    case QMessageBox::Cancel:
        break;
    }
}

void Widget::newMessageSlot(const QString &from, const QString &message)
{
    // this->ui->msgTextEdit->clear();
    QByteArray ba2= QByteArray::fromBase64(message.toLocal8Bit());
    QByteArray ba2Uzip= qUncompress(ba2);

    QImage image=QImage::fromData(ba2Uzip);
    QPixmap pix=QPixmap::fromImage(image);
    this->ui->msgTextEdit->append(from+":\n"+message+"\n");
}



void Widget::on_sendMsgButton_clicked()
{
    //this->phoneLib->sendTextMessageTo("yue.heqiang@95131.cc",
    //                                   this->getImageStr());
    //    this->phoneLib->sendTextMessageTo(this->ui->peerAddrEdit->text(),
    //                                      this->getImageStr());
    //    this->phoneLib->sendTextMessageTo(this->ui->peerAddrEdit->text(),
    //                                      this->ui->msgTextEdit->toPlainText());
}



QString Widget::getImageStr()
{
    QString imageStr="";
    QImage image;
    bool loaded=image.load("/data/data/lzl_small.jpg");
    qDebug()<<"image:"<<image.byteCount();

    if(loaded){
        QImage image_small=image.scaledToWidth(100);
        image_small.save("/data/data/lzl_small.jpg");
        qDebug()<<"image_small:"<<image_small.byteCount();
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer,"jpg");
        QByteArray baZip= qCompress(ba,9);
        QByteArray ba64=baZip.toBase64();

        imageStr=QString::fromLocal8Bit(ba64);
        qDebug()<<"imageStr len:"<<imageStr.length();
    }
    return imageStr;
}

void Widget::on_subScribeButton_clicked()
{

}

void Widget::on_createRoomButton_clicked()
{
    //this->phoneLib->createMucRoom("技术交流");
}

void Widget::mediaIsRunningSlot()
{
    setVideoWindows();
    phoneLib->showIncominWindow();
    phoneLib->showOutgoingWindow();
}

void Widget::on_makeCallBTN_clicked()
{
    if(ui->makeCallBTN->text()==MAKECALL){
        QString phoneNumer=this->ui->phoneNumber->text();
        if(this->phoneLib->makeVoiceCall(phoneNumer))
            this->ui->makeCallBTN->setText(HANGUP);

    }else   if(ui->makeCallBTN->text()==ANSWERCALL){
        this->phoneLib->answerCall();
    }else   if(ui->makeCallBTN->text()==HANGUP){
        this->phoneLib->releaseCall();
    }
}

void Widget::on_showPreviewBTN_clicked()
{
    this->setVideoWindows();
    this->phoneLib->showVideoPrevieWindow();

    //on_showPreviewBTN1_clicked();
}

void Widget::on_showPreviewBTN1_clicked()
{
    preViewWin->move(preViewWin->x(),preViewWin->y()-1);
    preViewWin->move(preViewWin->x(),preViewWin->y()+1);
    callWin->move(preViewWin->x(),preViewWin->y()-1);
    callWin->move(preViewWin->x(),preViewWin->y()+1);
    //preViewWin->update();
    //preViewWin->show();
}

void Widget::setVideoWindows()
{
    if(preViewWin==NULL)
        preViewWin=new QWidget();
    if(callWin==NULL)
        callWin=new QWidget();
    preViewWin->winId();
    VideoDisplay prVwin;
    VideoDisplay inComingVwin;
#ifdef Q_OS_WIN32
    prVwin.window= (void *)  preViewWin->winId();
    inComingVwin.window= (void *)  callWin->winId();
#endif

#ifdef Q_OS_DARWIN64
   NSWindow *prewin = [(NSView*)preViewWin->winId() window];
    NSWindow *incomingwin = [(NSView*)callWin->winId() window];
    prVwin.window=prewin;
    inComingVwin.window=incomingwin;
#endif
    this->phoneLib->setVideoDisplays(prVwin,inComingVwin);
    preViewWin->move(0,0);
    callWin->move(0,20);
    preViewWin->show();
    callWin->show();
}





void Widget::on_callStaticsInfoBTN_clicked()
{

    qDebug("Statics:%s",phoneLib->callStaticInfo().c_str());
}

void Widget::on_playFileBTN_clicked()
{
    this->phoneLib->playFileToCall("/data/ttt/voice/pipayu.wav",0);
}

void Widget::on_holdCallBTN_clicked()
{
#define UNHOLD_TEXT  "Un Hold"
    if(ui->holdCallBTN->text()==UNHOLD_TEXT){
        if(this->phoneLib->releaseHold()){
            this->ui->holdCallBTN->setText("Hold Call");
        }
    }else{
        if(this->phoneLib->setOnHold()){
            this->ui->holdCallBTN->setText(UNHOLD_TEXT);
        }
    }
}

void Widget::on_confereceAllBTN_clicked()
{
 this->phoneLib->putCallInConferece();
}

void Widget::on_ShowVideoBTN_clicked()
{
     //engine.load(QUrl(QStringLiteral("qrc:/cameraView.qml")));

}


void Widget::sipConnected(long accid)
{
    qDebug()<<"sipConnected..."<<accid;

}

void Widget::sipDisconnected(long accid)
{
    qDebug()<<"sipDisconnected..."<<accid;

}

void Widget::sipConnectionQualityReport(int Strength, long accid)
{
}

void Widget::callIsAnswserd(QString number, int callID, long accid)
{
    qDebug()<<"callIsAnswserd..."<<(number);
    this->ui->makeCallBTN->setText(HANGUP);

}

void Widget::callIsReleased(QString number, int callID, int causeCode, QString reasonText, long accid)
{
    qDebug()<<"callIsReleased..."<<(number);
    this->ui->makeCallBTN->setText(MAKECALL);
}

void Widget::callIsIncoming(QString peerAdder, int callID, long accid)
{
    qDebug()<<"callIsIncoming..."<<(peerAdder);
    this->ui->phoneNumber->setText((peerAdder));
    this->ui->makeCallBTN->setText(ANSWERCALL);
}

void Widget::callMediaAnnounce(QString peerAddr, MediaInfo::MediaType mediaType, int callID, long accid)
{
}

void Widget::callMediaIsRunning(QString peerAddr, int callID, long accid)
{
    qDebug()<<"callMediaIsRunning..."<<peerAddr;
   // mediaIsRunningSlot();
    emit mediaIsRunning();
}

void Widget::sipIMMessageInComing(QString peerAddr, QString msgText, int callID, long)
{
    qDebug()<<"fromAddr..."<< (peerAddr)<<" msgText:"<< (msgText);

}

void Widget::callIsBeingTransfered(QString peerAddr, QString destAddr, int callID, long)
{
}

void Widget::sipInfoInComing(QString type, QString subType, QString body, int callID, long accID)
{
}

void Widget::DTMFInComing(QString digit, unsigned int duration, int callID, long accID)
{
}

void Widget::onBuddyState(QString peerAddr, int stateid, long accid)
{
}
