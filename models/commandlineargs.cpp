#include "models/commandlineargs.h"
#include "logger.h"

#include <QCommandLineParser>

#include <helpers/commandlineparserhelper.h>

auto CommadLineArgs::Parse(const QCoreApplication& app) -> CommadLineArgs
{
    QCommandLineParser parser;

    parser.setApplicationDescription(QStringLiteral("raspilink client1"));
    parser.addHelpOption();
    parser.addVersionOption();

    const QString OPTION_WORKINGDIR = QStringLiteral("working_dir");
//    const QString OPTION_OUT = QStringLiteral("output");
//    const QString OPTION_BACKUP = QStringLiteral("backup");
    const QString OPTION_TEST = QStringLiteral("test");

    CommandLineParserHelper::addOption(&parser, OPTION_WORKINGDIR, QStringLiteral("working directory"));
//    CommandLineParserHelper::addOption(&parser, OPTION_OUT, QStringLiteral("g-code file as output"));
//    CommandLineParserHelper::addOptionBool(&parser, OPTION_BACKUP, QStringLiteral("set if backup is needed"));
    CommandLineParserHelper::addOptionBool(&parser, OPTION_TEST, QStringLiteral("set to activate test mode"));

    parser.process(app);

    CommadLineArgs p;
    p.working_dir = parser.value(OPTION_WORKINGDIR);
    p.isTest = parser.isSet(OPTION_TEST);
    return p;
}

auto CommadLineArgs::IsValid() -> bool
{
    QStringList err;
    if(working_dir.isEmpty())
    {
        err.append(QStringLiteral("working_dir is empty"));
    }
    if(!err.isEmpty()) zInfo(err);
    return err.isEmpty();
}
