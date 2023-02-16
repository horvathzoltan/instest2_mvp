#include "ipscanner.h"
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QThread>
#include <iostream>

bool IpScanner::_verbose;

void IpScanner::setVerbose(bool v)
{
    _verbose = v;
}

void IpScanner::log(const QString &msg)
{
    static QTextStream out(stdout);

    out<<msg;
    out.flush();
}

QMap<QString, QSet<int>> IpScanner::Scan(QHostAddress ip, int i1, int i2, QSet<int> ports, int timeout, int steps)
{
    if(i1<1||i1>255) return {};
    if(i2<1||i2>255) return {};
    if(steps<1||steps>10) return {};
    if(i1>i2) return {};
    for(auto&port:ports) if(port<1||port>UINT16_MAX) return {};

    QTcpSocket socket;
    QMap<QString, QSet<int>> ipList;
    QHostAddress address;

    qint32 i = ip.toIPv4Address();
    unsigned char* ip2 = reinterpret_cast<unsigned char*>(&i);//mutató az i LSB-re

    int timeout2 = timeout/2;
    for(unsigned char u=i1;u<i2;u++){
        *ip2=u;
        address.setAddress(i);
        for(auto&port:ports){
            bool sok = false;
            for(int i=0;i<steps;i++){
                int t = timeout+(timeout2*i);
                bool ok = TryConnect(socket, address, port, t);
                if(ok){
                    sok = true;
                    log("steps: "+QString::number(i+1)+" timeout: "+QString::number(t));
                    break;
                }
            }
            if(sok){
                auto a = address.toString();
                ipList[a].insert(port);
                continue;
            }
        }
    }
    if(_verbose) log("\r");
    return ipList;
}

bool IpScanner::TryConnect(QTcpSocket& socket, const QHostAddress& address, int port, int timeout)
{
    socket.connectToHost(address, port, QIODevice::ReadWrite);
    bool ok = socket.waitForConnected(timeout);
    if(ok) socket.disconnectFromHost();
    return ok;
}

QList<QHostAddress> IpScanner::GetLocalAddresses()
{
    QList<QHostAddress> result;
    QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();

    for(auto&eth:allInterfaces) {
        QList<QNetworkAddressEntry> allEntries = eth.addressEntries();        
        for(auto&entry:allEntries) {
            auto ip = entry.ip();
            if (ip.protocol() != QAbstractSocket::IPv4Protocol) continue;
            if(ip == QHostAddress(QHostAddress::LocalHost)) continue;
            if(entry.netmask().isNull()) continue;
            result.append(ip);
        }
    }
    return result;
}
