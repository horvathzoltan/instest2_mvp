#ifndef LOGGER_H
#define LOGGER_H

#include <QString>


class LocInfo
{
public:
    QString func;
    QString file;
    int line;

    LocInfo(const char *func, const char *file, int line)
    {
        this->func=QString(func);
        this->file =QString(file);
        this->line=line;
    }

    QString toString() const
    {
        return file+':'+QString::number(line)+' '+func;
    }
};

#if defined(__GNUC__)
#define getLocInfo LocInfo(static_cast<const char*>(__PRETTY_FUNCTION__) ,__FILE__,__LINE__)
#elif defined(__MINGW32__)
#define getLocInfo LocInfo(__PRETTY_FUNCTION__ ,__FILE__,__LINE__)
#elif defined (_MSC_VER )
#define getLocInfo LocInfo(__FUNCSIG__ ,__FILE__,__LINE__)
#endif

#define zError(msg) Logger::error2((msg), getLocInfo)

#define zWarning(msg) Logger::warning2((msg), getLocInfo);
#define zInfo(msg) Logger::info2((msg), getLocInfo);
#define zDebug() Logger::debug2(getLocInfo);
#define zTrace() Logger::trace2(getLocInfo);

#define zError2(msg,i) Logger::error2((msg), getLocInfo,(i));
#define zWarning2(msg,i) Logger::warning2((msg), getLocInfo,(i));
#define zInfo2(msg,i) Logger::info2((msg), getLocInfo,(i));

namespace Errlevels{
    enum Levels:int {
        ERROR_,
        WARNING,
        INFO,
    };

    [[gnu::unused]]
    static QString toString  (const Levels &l){
        switch(l)
        {
        case ERROR_: return QStringLiteral("ERROR");
        case WARNING: return QStringLiteral("WARNING");
        case INFO: return QStringLiteral("INFO");
        default: return QStringLiteral("unknown");
        }
    };
};

namespace Dbglevels{
enum Levels:int {
    NONE=0,
    TRACE,
    DEBUG,
//    INFOAPPEND,
//    INFOCLOSE
};

[[gnu::unused]]
static QString toString  (const Levels &l){
    switch(l){
        case NONE: return QStringLiteral("TRACE");
        case TRACE: return QStringLiteral("TRACE");
        case DEBUG: return QStringLiteral("DEBUG");
//    case INFOAPPEND: return QStringLiteral("INFO");
//    case INFOCLOSE: return QStringLiteral("INFO");
        default: return QStringLiteral("unknown");
    }
};
}

class Logger
{
public:
    static bool _isBreakOnError;
    static bool _isVerbose;
    static Errlevels::Levels _errlevel;
    static Dbglevels::Levels _dbglevel;


    static const QString OK;
    static const QString ERROR_;
    static const QString WARNING;

    static void init(Errlevels::Levels level,
                     Dbglevels::Levels dbglevel,
                     bool isBreak, bool isVerbose);

    static QString ToErrorString(Errlevels::Levels, const QString&, const QString&, const QString&);
    static QString ToDebugString(Dbglevels::Levels level, const QString &msg, const QString &loci, const QString &st);
    static void err_message(Errlevels::Levels level, const QString& msg, int flag =0);
    static void dbg_message(Dbglevels::Levels level, const QString& msg, int flag =0);

    static void error2(const QString& msg, const LocInfo& l, int flag = 0);
    static void warning2(const QString& msg, const LocInfo& l, int flag = 0);
    static void info2(const QString& msg, const LocInfo& l, int flag = 0);
    static void info2(const QStringList& msg, const LocInfo& l, int flag = 0);
    static void debug2(const LocInfo& l);
    static void trace2(const LocInfo& l);

    static QString zStackTrace();

};

#endif // LOGGER_H
