#include "filenamehelper.h"
#include "qcoreapplication.h"

#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

QString FileNameHelper::GetAbsolutFn(const QString& fn){
    QFileInfo fi(fn);

    if(fi.isAbsolute())
        return fn;
    auto a =  QDir::homePath();//+'/'+ fn;
    //if(a.isEmpty()) a = qApp->applicationDirPath();
    if(a.isEmpty()||!a.startsWith("/home")) a= getenv("PWD");
    //if(a.isEmpty()) a= QDir::currentPath();
    a+='/'+ fn;
    return a;
}
