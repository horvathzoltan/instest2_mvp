#include "logger.h"
#include "mainwindow.h"
#include "mainpresenter.h"
#include "macro.h"
#include "models/commandlineargs.h"
#include <QApplication>
#include <QDir>
#include <QProcess>
#include <models/settings.h>
#include "logger.h"

int main(int argc, char *argv[])
{
#if defined (STRING) && defined (TARGI)
    auto poj = STRING(TARGI);
    zInfo(QStringLiteral("started ")+poj);
#else
    auto poj=QStringLiteral("manymany");
#endif

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(poj);

    // remember what current directory is
    QString currentDir=QDir::currentPath();

    //only for testing - show openssl version provided by OS
    QProcess process; process.start("openssl version");
    process.waitForFinished(-1);
    QString stdout = process.readAllStandardOutput();
    qDebug() << "OS openssl version: " << stdout;

    // change current Dir to application directory path (it will point into AppImage filesystem usr/bin)
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // get SSL support status and runtime openssl version
    // this forces Qt to find (initialise) openssl libs and use them for all later SSL operations
    qDebug() << "SSL supported:" << QSslSocket::supportsSsl();
    qDebug() << "Qt is using:" << QSslSocket::sslLibraryVersionString();

    // restore current dir
    QDir::setCurrent(currentDir);

    CommadLineArgs params = CommadLineArgs::Parse(a);

    Settings settings = Settings::Load(params.working_dir);
    if(!settings.isValid())
    {
        zInfo("cannot load settings");
        return 1;
    }



    MainWindow w;
    MainPresenter p;
    p.init({settings});
    p.appendView(&w);
    w.show();
    p.initView(&w);

    return a.exec();
}
