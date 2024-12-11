#include "widget.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <cstdio>


//// 全局变量
//QFile *file = nullptr;
//QTextStream *stream = nullptr;
//QMutex mutex;  // 保护日志文件写入的互斥锁

//// 外部 messageHandler 函数
//void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg) {
//    //QMutexLocker locker(&mutex);  // 确保线程安全
//    if (stream) {
//        switch (type) {
//        case QtDebugMsg:
//            (*stream) << "DEBUG: " << msg << '\n';
//            break;
//        case QtWarningMsg:
//            (*stream) << "WARNING: " << msg << '\n';
//            break;
//        case QtCriticalMsg:
//            (*stream) << "CRITICAL: " << msg << '\n';
//            break;
//        case QtFatalMsg:
//            (*stream) << "FATAL: " << msg << '\n';
//            break;
//        }
//        //stream->flush();  // 确保内容即时写入文件
//        file = new QFile(filePath);
//        if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
//            qWarning() << "无法打开文件进行输出重定向:" << filePath;
//            return;
//        }

//        // 创建 QTextStream 关联到文件
//        stream = new QTextStream(file);
//        file->close();
//    }
//}


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

//// 重定向输出到文件的函数
void redirectOutputToFile(const QString &filePath) {
    // 设置 Qt 的日志处理器
    qInstallMessageHandler(messageHandler);
    // 重定向标准输出和标准错误
    //std::freopen(filePath.toStdString().c_str(), "w", stdout);
    std::freopen(filePath.toStdString().c_str(), "w", stderr);
}

//// 关闭日志文件的函数
//void closeLogFile() {
//    if (file && file->isOpen()) {
//        file->close();
//        delete file;
//        file = nullptr;
//    }
//    if (stream) {
//        delete stream;
//        stream = nullptr;
//    }
//}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString logFilePath = "outputlog.txt";


    //qDebug() << "00000000000000000000";
    // 调用重定向函数
    redirectOutputToFile(logFilePath);

    // 示例输出
    //qDebug()<< "1111111111111111111";
    //printf("232323\n");
    //fprintf(stdout,"hello\n");
    //fprintf(stderr,"world\n");
    //fflush(stdout);
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
    //closeLogFile();
    //fprintf(stderr,"4444444");
    return a.exec();
}
