#include "helpers/textfilehelper.h"
#include "logger.h"
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>


QString TextFileHelper::Load(const QString& filename) {
    QFileInfo fi(filename);
    if(!fi.isAbsolute())
    {
        zInfo(QStringLiteral("path is not absolute: %1").arg(filename));
        return QString();
    }

    if(!fi.exists())
    {
        zInfo(QStringLiteral("file not exist: %1").arg(filename));
        return QString();
    }

    QFile f(filename);
    QString e;

    // TODO ha relatív a filename, akkor abszolúttá kell tenni
    // egyébként megnyitható azaz

    if (f.open(QFile::ReadOnly | QFile::Text))  {
        zInfo(QStringLiteral("loaded: %1").arg(filename));
        QTextStream st(&f);
        st.setCodec("UTF-8");
        e = st.readAll();
    }
    else{
        zInfo(QStringLiteral("cannot read file (%1): %2").arg(f.errorString()).arg(filename));
        e= QString();
    }
    return e;
}

QStringList TextFileHelper::LoadLines(const QString& filename) {
    QFileInfo fi(filename);
    if(!fi.isAbsolute())
    {
        zInfo(QStringLiteral("path is not absolute: %1").arg(filename));
        return QStringList();
    }

    if(!fi.exists())
    {
        zInfo(QStringLiteral("file not exist: %1").arg(filename));
        return QStringList();
    }

    QFile f(filename);
    QStringList e;

    // TODO ha relatív a filename, akkor abszolúttá kell tenni
    // egyébként megnyitható azaz

    if (f.open(QFile::ReadOnly | QFile::Text))  {
        zInfo(QStringLiteral("loaded: %1").arg(filename));
        QTextStream st(&f);
        st.setCodec("UTF-8");

        while (!st.atEnd())
        {
            e << st.readLine();
        }
        f.close();

    }
    else{
        zInfo(QStringLiteral("cannot read file (%1): %2").arg(f.errorString()).arg(filename));
        e= QStringList();
    }
    return e;
}


bool TextFileHelper::Save(const QString& txt, const QString& fn, bool isAppend) {

    QFile f(fn);

    auto om = QIODevice::WriteOnly | QIODevice::Text; // openmode
    if(isAppend) om |= QIODevice::Append;

    if (!f.open(om)){                
        zInfo(QStringLiteral("cannot write file (%1): %2").arg(f.errorString()).arg(fn));
        return false;
        }

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QTextStream out(&f);
    //out.setCodec(QTextCodec::codecForName("UTF-8"));
    //out.setGenerateByteOrderMark(true);
    out << txt;//.toUtf8();
    f.close();
    return true;
}

