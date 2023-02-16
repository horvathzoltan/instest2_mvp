#ifndef IPSCANNER_H
#define IPSCANNER_H

#include <QHostAddress>
#include <QList>
#include <QMap>
#include <QSet>
#include <QTcpSocket>

class IpScanner
{
private:
    static bool _verbose;

    static void log(const QString& mag);
    static bool TryConnect(QTcpSocket& socket, const QHostAddress& address, int port, int timeout);
public:                
    static QMap<QString, QSet<int>> Scan(QHostAddress ip, int i1, int i2, QSet<int> p, int timeout, int steps);
    static QList<QHostAddress> GetLocalAddresses();
    static void setVerbose(bool v);

};

#endif // IPSCANNER_H
