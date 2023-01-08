#ifndef COM_TEXTFILEHELPER_H
#define COM_TEXTFILEHELPER_H

#include <QStringList>

class  TextFileHelper
{
public:
    static QString Load(const QString& filename);
    static QStringList LoadLines(const QString& filename);
    static bool Save(const QString& txt, const QString& fileName, bool isAppend = false);
};

#endif // TEXTFILEHELPER_H
