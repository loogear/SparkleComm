#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


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
    socket.connectToHost("192.168.0.110", 41825); // google DNS, or something else reliable
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
