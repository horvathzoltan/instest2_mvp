#include "logger.h"
#include <QDebug>

#include <execinfo.h>
#include <cxxabi.h>

#include <csignal>


bool Logger::_isBreakOnError = false;
//zLogGUIfn Logger::_GUILogger = nullptr;
//void* Logger::_ui = nullptr;
bool Logger::_isVerbose = false;
Errlevels::Levels Logger:: _errlevel = Errlevels::INFO;
Dbglevels::Levels Logger:: _dbglevel = Dbglevels::TRACE;

const QString Logger::OK = QStringLiteral("ok");
const QString Logger::ERROR_ = QStringLiteral("error");
const QString Logger::WARNING = QStringLiteral("warning");

void Logger::init(Errlevels::Levels errlevel,
               Dbglevels::Levels dbglevel,
               bool isBreakOnError, bool isVerbose)
{
     _errlevel = errlevel;
     _dbglevel = dbglevel;
    //_GUILogger = ez;
    _isBreakOnError = isBreakOnError;
    //_ui=uiptr;
    _isVerbose = isVerbose;
}


QString Logger::ToDebugString(Dbglevels::Levels level, const QString &msg, const QString &loci, const QString &st){
    auto txt = Dbglevels::toString(level);
    QString msg3;
    switch(level){
    case Dbglevels::TRACE:
        msg3= txt+": "+loci;
        break;
    case Dbglevels::DEBUG:
        msg3= txt+": "+msg+"\n"+loci+"\n"+st;
        break;
    default: break;
    }
    return msg3;
}

QString Logger::ToErrorString(Errlevels::Levels errlevel, const QString &msg, const QString &loci, const QString &st)
{
    auto level = Errlevels::toString(errlevel);
    QString msg3;

    switch(errlevel)
    {
    case Errlevels::ERROR_:
        msg3= level+": "+msg+"\n"+loci+"\n"+st;
        break;
    case Errlevels::WARNING:
        msg3= level+": "+msg+"\n"+loci;
        break;

    case Errlevels::INFO:
        msg3= level+": "+msg;
        if(_isVerbose) msg3+="\n"+loci;
        break;
    default: break;
    }

    return msg3;
}

void Logger::err_message(Errlevels::Levels level, const QString& msg, int flag)
{
  //  GUIModes::Modes guimode;

#ifdef QT_DEBUG
#ifdef Q_OS_WIN
    auto a = __FUNCTION__;
#elif defined(Q_OS_LINUX)
    auto a = static_cast<const char*>(__PRETTY_FUNCTION__);
#endif
    switch(level)
    {
    case Errlevels::ERROR_:
        QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, a).critical().noquote()<<msg;
//        guimode = GUIModes::ERROR;
        break;
    case Errlevels::WARNING:
        QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, a).warning().noquote()<<msg;
//        guimode = GUIModes::WARNING;
        break;
    case Errlevels::INFO:
        QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, a).info().noquote()<<msg;
//        guimode = GUIModes::INFO;
        break;
    default:
//        guimode = GUIModes::INFO;
        break;
    }
//    if(_GUILogger) {
//        _GUILogger(guimode, msg, "", "", _ui, flag);
//    }
#ifdef Q_OS_LINUX
    if((level==Errlevels::ERROR_) && _isBreakOnError) std::raise(SIGTRAP);
#endif
#endif
}


void Logger::dbg_message(Dbglevels::Levels level, const QString& msg, int flag)
{
//    GUIModes::Modes guimode;
#ifdef QT_DEBUG
#ifdef Q_OS_WIN
        auto a = __FUNCTION__;
#elif defined(Q_OS_LINUX)
    auto a = static_cast<const char*>(__PRETTY_FUNCTION__);
#endif
    switch(level){
    case Dbglevels::DEBUG:
    case Dbglevels::TRACE:
        QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, a).debug().noquote()<<msg;
//        guimode = GUIModes::DEBUG;
        break;
    default:
//        guimode = GUIModes::DEBUG;
        break;
    }

//    if(_GUILogger){
//        _GUILogger(guimode, msg, "", "", _ui, flag);
//    }
#ifdef Q_OS_LINUX
    if((level ==Dbglevels::DEBUG) && _isBreakOnError) std::raise(SIGTRAP);
#endif
#endif
}

void Logger::info2(const QString& msg, const LocInfo& locinfo, int flag)
{
    if(_errlevel>Errlevels::INFO) return;
    QString li;
    if(_isVerbose)
    {
        li = locinfo.toString();
    }

    auto msg2 = ToErrorString(Errlevels::INFO, msg, li, nullptr);
    err_message(Errlevels::INFO, msg, flag);
}

void Logger::info2(const QStringList& msgl, const LocInfo& locinfo, int flag)
{
    QString li;
    if(_isVerbose)
    {
        li = locinfo.toString();
    }

    for(auto&msg:msgl)
    {
        auto msg2 = ToErrorString(Errlevels::INFO, msg, nullptr, nullptr);
        err_message(Errlevels::INFO, msg2, flag);
    }
}

void Logger::trace2(const LocInfo& locinfo){
    if(_dbglevel==Dbglevels::NONE || _dbglevel>Dbglevels::TRACE) return;
    auto li = locinfo.toString();
    auto msg2 = ToDebugString(Dbglevels::TRACE, nullptr, li, nullptr);
    dbg_message(Dbglevels::TRACE, msg2);
    }

void Logger::debug2(const LocInfo& locinfo){
    if(_dbglevel==Dbglevels::NONE || _dbglevel>Dbglevels::DEBUG) return;
    auto li = locinfo.toString();
    auto st = Logger::zStackTrace();

    auto msg2 = ToDebugString(Dbglevels::DEBUG, nullptr, li, st);
    dbg_message(Dbglevels::DEBUG, msg2);
    }

void Logger::error2(const QString& msg,  const LocInfo& locinfo, int flag){
    if(_errlevel>Errlevels::ERROR_) return;

    auto li = locinfo.toString();
    auto st = Logger::zStackTrace();

    auto msg2 = ToErrorString(Errlevels::ERROR_, msg, li, st);
    err_message(Errlevels::ERROR_, msg2, flag);
    }

void Logger::warning2(const QString& msg, const LocInfo& locinfo, int flag){
    if(_errlevel>Errlevels::WARNING) return;
    auto li = locinfo.toString();
    auto msg2 = ToErrorString(Errlevels::WARNING, msg, li, nullptr);
    err_message(Errlevels::WARNING, msg, flag);
    }


#ifdef Q_OS_LINUX
QString Logger::zStackTrace()
{
    QStringList e;

    unsigned int max_frames = 64;

    e << QStringLiteral("stack trace:");

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    int addrlen = backtrace(static_cast<void**>(addrlist), sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0)
    {
        e << QStringLiteral("<empty, possibly corrupt>");
        return e.join('\n');
    }

    // resolve addresses into strings containing "filename(function+address)", this array must be free()-ed
    auto symbollist = backtrace_symbols(static_cast<void**>(addrlist), addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    auto funcname = static_cast<char*>(malloc(funcnamesize));

    // iterate over the returned symbol lines. skip the first, it is the address of this function.
    for (int i = 1; i < addrlen; i++)
    {
    char *begin_name = nullptr, *begin_offset = nullptr, *end_offset = nullptr;

    for (char *p = symbollist[i]; *p; ++p)
    {
        if (*p == '(')
        {
            begin_name = p;
        }
        else if (*p == '+')
        {
            begin_offset = p;
        }
        else if (*p == ')' && begin_offset)
        {
            end_offset = p;
            break;
        }
    }

    if (begin_name && begin_offset && end_offset
        && begin_name < begin_offset)
    {
        *begin_name++ = '\0';
        *begin_offset++ = '\0';
        *end_offset = '\0';

        int status;
        char* ret = abi::__cxa_demangle(begin_name,funcname, &funcnamesize, &status);
        if (status == 0)
        {
            funcname = ret; // use possibly realloc()-ed string
            e << QStringLiteral("%1: %2 + %3").arg(symbollist[i],ret,begin_offset);
            //e << QStringLiteral("%1").arg(funcname);
        }
        else
        {
            // demangling failed. Output function name as a C function with
            // no arguments.
            e << QStringLiteral("%1: %2 + %3").arg(symbollist[i],begin_name,begin_offset);
            //e << QStringLiteral("%1").arg(begin_name);
        }
    }
    else
    {
        // couldn't parse the line? print the whole line.
        e << QString(symbollist[i]);
    }
    }

    free(funcname);
    free(symbollist);

    auto a =  e.join('\n');
    return a;
}
#endif
