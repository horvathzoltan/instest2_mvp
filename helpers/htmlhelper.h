#ifndef HTMLHELPER_H
#define HTMLHELPER_H

#include <QSet>
#include <QString>
#include <QTime>

class HTMLHelper
{
private:
    static const QSet<QString> _selfClosingTags;
    static const QString tagregexp;
    static const QString tagregexp20;
    static const QString tagregexp21;
    static const QString imgsource;
    static const QString timeregexp;
    static QString GetContent(const QString &txt, const QString& regextxt);
public:
    static QString GetDivContent(const QString& txt, const QString& token);
    static QString GetImgSrc(const QString& txt);
    static QStringList GetNestedTagContent(const QString& txt, const QString& tag, const QString& token);
    static QTime GetTime(const QString &txt);
    struct Tag{
        QString tag;
        QString desc;
    };
    static QStringList GetNestedTagContent(const QString& txt, const QList<Tag>& tags);
};

#endif // HTMLHELPER_H
