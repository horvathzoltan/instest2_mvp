#include "htmlhelper.h"
#include "logger.h"

#include <QRegularExpression>

const QSet<QString> HTMLHelper::_selfClosingTags{"area", "base", "br", "col", "embed", "hr",
                                                 "img", "input", "link", "meta", "param", "source",
                                                "track", "wbr", "command", "keygen", "menuitem"};


const QString HTMLHelper::tagregexp = //div tartalma
        QStringLiteral(R"(<%2[^>]*%1[^-]?[^>]*>([^<]*)<\/%2>)");
const QString HTMLHelper::tagregexp20 = //teljes tag 1
        QStringLiteral(R"(<%2[^>]*>)");
const QString HTMLHelper::tagregexp21 = //teljes tag 2
        QStringLiteral(R"(<%2[^>]*%1[^>]*>)");
const QString HTMLHelper::imgsource = // img source
        QStringLiteral(R"(<img[^>]*src=(?:(?:\"([^\"]*)\")|(?:\'([^\']*)\'))[^>]*>)");
const QString HTMLHelper::timeregexp =
        QStringLiteral(R"((\d?\d?:\d\d?))");

//(\d?\d?:\d\d?)
//<img[\w\W]*src="([^"]*)"[^>]*>
// <div.*sunrise(?:.*\n.*)>(.*)<\/div>
//<div.*current-weather-short-desc(?:.*\n*.*)>(.*)<\/div>
///current-weather-short-desc

QString HTMLHelper::GetContent(const QString &txt, const QString& regextxt)
{
    if(regextxt.isEmpty()) return{};
       QRegularExpression rx(regextxt);
       auto m = rx.match(txt);
       if(!m.hasMatch()) return {};
       if(m.captured().count()>=3)
       {
           if(!m.captured(1).isEmpty()) return m.captured(1);
           if(!m.captured(2).isEmpty()) return m.captured(2);
       }
       return {};
}

QString HTMLHelper::GetDivContent(const QString &txt, const QString& token)
{
    if(token.isEmpty()) return{};
    return GetContent(txt, tagregexp.arg(token, "div"));
}

QString HTMLHelper::GetImgSrc(const QString &txt)
{
        auto a = GetContent(txt, imgsource);
        return a;
}

QTime HTMLHelper::GetTime(const QString &txt)
{
    QString txt3 = GetContent(txt, timeregexp);
    QTime t = QTime::fromString(txt3, "h:m");
    return t;
}

QStringList HTMLHelper::GetNestedTagContent(const QString &txt, const QString& tag, const QString& token)
{        
    if(tag.isEmpty()) return{};

    QRegularExpression rx(tagregexp21.arg(token,tag));
    QStringList s;
    int ixc = 0;
    bool selfClosing = _selfClosingTags.contains(tag);
    while(ixc!=-1){
        auto m = rx.match(txt,ixc);
        if(!m.hasMatch()) break;

        int ix1 = m.capturedStart();
        int i=0;
        int ixd=ix1+1;//tag.length();
        while(ixd!=-1){
           if(selfClosing)
           {
               ixd = txt.indexOf('>',ixd);
           }else{
               ixd=txt.indexOf(tag,ixd);
           }
           if(ixd==-1) break;
           auto c = txt[ixd-1];
           if(c=='<')i++;
           else if(c=='/') i--;
           if(i==0) break;
           if(selfClosing){
               ixd++;
           }else{
               ixd+=tag.length();
           }
        }
        if(ixd==-1){
            s.append(txt.mid(ix1));
            break;
        };
        ixd=txt.indexOf('>',ixd);
        s.append(txt.mid(ix1,ixd-ix1+1));
        ixc=ixd;
    }
    return s;
}

QStringList HTMLHelper::GetNestedTagContent(const QString &txt, const QList<Tag>& tags)
{
    QStringList es(txt);
    for(auto&tag:tags)
    {
        QStringList a;
        for(auto&e:es)
        {
            a.append(GetNestedTagContent(e, tag.tag, tag.desc));
        }
        es=a;
    }

    return es;
}

