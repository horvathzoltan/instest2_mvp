#ifndef ZSHORTGUID_H
#define ZSHORTGUID_H

#include <QString>
#include <QUuid>



class zShortGuid
{
public:
    static QString Encode(const QUuid& g);
    static QUuid Decode(const QString& s);
};

#endif // ZSHORTGUID_H
