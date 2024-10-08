#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQml>
#include "softphone_lib.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget /*,ImEventReciever,ImContactEventReciever*/,
        SipEventReciever
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    static  Softphone_lib *phoneLib;

private:
    Ui::Widget *ui;
 QWidget * preViewWin;
 QWidget * callWin;
    // SipEventReciever interface


 private slots:
    void on_registerButton_clicked();
    void showRreuqestSlot(QString uid,QString msg);
    void newMessageSlot(const QString &from, const QString &message);

    // ImEventReciever interface
    void on_sendMsgButton_clicked();


QString  getImageStr();
void on_subScribeButton_clicked();

void on_createRoomButton_clicked();



void on_makeCallBTN_clicked();

void on_showPreviewBTN_clicked();

void on_showPreviewBTN1_clicked();

void setVideoWindows();
void on_callStaticsInfoBTN_clicked();
void on_playFileBTN_clicked();

void on_holdCallBTN_clicked();

void on_confereceAllBTN_clicked();

void on_ShowVideoBTN_clicked();

public slots:
void mediaIsRunningSlot();

signals:
    void showRreuqest(QString uid,QString msg);
    void newMessageSignal(const QString &from, const QString &message);
    void mediaIsRunning();
    // SipEventReciever interface
public:
    virtual void sipConnected(long accid) override;
    virtual void sipDisconnected(long accid) override;
    virtual void sipConnectionQualityReport(int Strength, long accid) override;
    virtual void callIsAnswserd(QString, int callID, long accid) override;
    virtual void callIsReleased(QString number, int callID, int causeCode, QString reasonText, long accid) override;
    virtual void callIsIncoming(QString peerAdder, int callID, long accid) override;
    virtual void callMediaAnnounce(QString peerAddr, MediaInfo::MediaType mediaType, int callID, long accid) override;
    virtual void callMediaIsRunning(QString peerAddr, int callID, long accid) override;
    virtual void sipIMMessageInComing(QString peerAddr, QString msgText, int callID, long) override;
    virtual void callIsBeingTransfered(QString peerAddr, QString destAddr, int callID, long) override;
    virtual void sipInfoInComing(QString type, QString subType, QString body, int callID, long accID) override;
    virtual void DTMFInComing(QString digit, unsigned int duration, int callID, long accID) override;
    virtual void onBuddyState(QString peerAddr, int stateid, long accid) override;
};

#endif // WIDGET_H
