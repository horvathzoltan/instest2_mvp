#ifndef COM_COMMANLINEPARSERHELPER_H
#define COM_COMMANLINEPARSERHELPER_H

#include <QCommandLineParser>

class CommandLineParserHelper
{
public:
    CommandLineParserHelper();
    static bool addOption(QCommandLineParser *p, const QString& o, const QString& d);
    static bool addOptionBool(QCommandLineParser *p, const QString& o, const QString& d);
};

#endif // COMMANLINEPARSERHELPER_H
