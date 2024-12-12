#include "widget.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <cstdio>


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
    //socket.connectToHost("192.168.0.110", 41825); // google DNS, or something else reliable
    socket.connectToHost("183.230.190.196", 41825);
    if(socket.waitForConnected()){
        qDebug()<<"localAddress:"<<socket.localAddress().toString();
    }else{
        qDebug()<<"error:"<<socket.errorString();

    }

#endif

    Widget w;
    w.show();
    return a.exec();
}
