#include "widget.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <cstdio>
#include <QObject>
#include "httpclient.h"

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{


    QString txt;
    QString fileName;

    // Get filename of context
    if (context.file) {
        fileName = QFileInfo(context.file).baseName();
    } else {
        fileName = "";
    }


    switch (type) {
    case QtDebugMsg:
        txt = QString("[%1:%2]DEBUG: %3").arg(fileName, 16).arg(context.line, 4).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("[%1:%2]WARN: %3").arg(fileName, 16).arg(context.line, 4).arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("[%1:%2]CRIT: %3").arg(fileName, 16).arg(context.line, 4).arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("[%1:%2]FATAL: %3").arg(fileName, 16).arg(context.line, 4).arg(msg);
        break;
#if QT_VERSION >= 0x050901
    case QtInfoMsg:
        txt = QString("[%1:%2]INFO: %3").arg(fileName, 16).arg(context.line, 4).arg(msg);
        break;
#endif
    default:
        break;
    }

    QByteArray timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz").toLocal8Bit();
    QByteArray thread = QString("[%1]").arg(QString::number(quintptr(QThread::currentThreadId()), 16)).toLocal8Bit();
    QString logString = QString("%1 [%2] %3\n").arg(timestamp.constData()).arg(thread.constData()).arg(txt);

    QFile logfile("outputlog.txt");
    if (!logfile.open(QIODevice::ReadWrite | QIODevice::Append)) {
        fprintf(stderr, "file open failed\n");
        return;
    }

    QTextStream ts(&logfile);
    ts <<logString;
    logfile.close();
}


void redirectOutputToFile(const QString &filePath) 
{
    qInstallMessageHandler(messageHandler);
    std::freopen(filePath.toStdString().c_str(), "w", stderr);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString logFilePath = "outputlog.txt";
    redirectOutputToFile(logFilePath);

#if 0
        QUdpSocket socket;
        socket.connectToHost("192.168.0.22", 7878); // google DNS, or something else reliable
        QByteArray ba="hello";
        QHostAddress host("192.168.0.22");
        socket.writeDatagram(ba,host,7878);
#else
    QTcpSocket socket;
    QNetworkProxy networkProxy;
    socket.setProxy(QNetworkProxy::NoProxy);
    // 创建HttpClient对象，输入应用服务器地址、端口，获取配置信息（如SIP服务器地址、端口等）
    HttpClient *client = new HttpClient("as3.loogear.com", 41833, false);
    QString addr;
    int port = 0;
    // 调用init初始化，校验应用密钥
    client->init("e09137a7c62f442eb78ba5e6886aaae1", "fed9cb0543ae45269d56ddbbd89ce7a8");
    client->getSipServerInfo(addr, port);
    socket.connectToHost(addr, port);
    Widget w;
    // 关联登录完成的信号与槽函数
    QObject::connect(client, &HttpClient::loginFinished, &w, &Widget::loginFinishSlot, Qt::QueuedConnection);
    if(socket.waitForConnected()){
        qDebug()<<"localAddress:"<<socket.localAddress().toString();
    }else{
        qDebug()<<"error:"<<socket.errorString();
    }
#endif
    w.show();
    return a.exec();
}
