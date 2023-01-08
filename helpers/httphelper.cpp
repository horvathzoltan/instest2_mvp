#include "httphelper.h"
#include "logger.h"

#include <QNetworkReply>
#include <QUrlQuery>
#include "zshortguid.h"

const QString HttpHelper::KEY=QStringLiteral("key");

HttpHelper::HttpHelper(QObject *parent)
    : QObject{parent}
{

}

bool HttpHelper::init(const QString& host, int port)
{
    _inited = false;

    if(host.startsWith("http")){
        _url = QUrl(host);
    }else{
        _url = QUrl();
        _url.setScheme("http");
        _url.setHost(host);
    }

    if(port>=0&&port<=65535) _url.setPort(port);
    if(!_url.isValid()) return _inited;
    _inited = true;
    return _inited;
}

QUuid HttpHelper::GetAction(const QString& action)//, const QString& query)
{
    auto mgr = CreateMgr();
    auto guid = RegisterAction(action, mgr);
    if(!action.startsWith('#')) _url.setPath((action.startsWith('/')?action:'/'+action));
    //_url.setQuery(query);
    auto request = CreateRequest(_url);
    mgr->get(request);
    return guid;
}

QUuid HttpHelper::GetAction2(const QString& action, const QString& query)
{
    auto mgr = CreateMgr();
    auto guid = RegisterAction(action, mgr);
    if(!action.startsWith('#')) _url.setPath((action.startsWith('/')?action:'/'+action));

    auto qtxt = _url.query();
    QUrlQuery q(qtxt);
    q.addQueryItem("c","b");
    _url.setQuery(q);

    auto request = CreateRequest(_url);
    mgr->get(request);
    return guid;
}

QUuid HttpHelper::Download(const QString& action, const QString& url)
{
    QUrl u(url);
    if(u.isValid()){
        auto mgr = CreateMgr();
        auto guid = RegisterAction(action, mgr);
        auto request = CreateRequest(u);
        mgr->get(request);
        return guid;
    } else{
        return {};
    }
}

QUuid HttpHelper::DownloadFromHost(const QString& action, const QString& path)
{
    auto mgr = CreateMgr();
    auto guid = RegisterAction(action, mgr);
    _url.setPath((path.startsWith('/')?path:'/'+path));
    auto request = CreateRequest(_url);
    mgr->get(request);
    return guid;
}

QNetworkAccessManager* HttpHelper::CreateMgr()
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinish(QNetworkReply*)));
    return mgr;
}

QUuid HttpHelper::RegisterAction(const QString &action, QNetworkAccessManager *mgr)
{
    auto guid = QUuid::createUuid();
    if(!action.isEmpty()) _actions.insert(guid, {action,mgr});
    auto guid2 = zShortGuid::Encode(guid);
    QUrlQuery q;
    q.addQueryItem(KEY, guid2);
    _url.setQuery(q.query());
    return guid;
}

QNetworkRequest HttpHelper::CreateRequest(const QUrl& url){
    QNetworkRequest request(url);
    if(url.scheme()=="https"){
        //QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
        //conf.setProtocol(QSsl::TlsV1_2);
        //request.setSslConfiguration(conf);
        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(conf);
    }

    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (X11; Linux x86_64; rv:101.0) Gecko/20100101 Firefox/101.0");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8");

    request.setTransferTimeout(180000);
    return request;
}



void HttpHelper::SendPost(const QString& source_lang, const QString& dest_lang, const QString& msg)
{
    QString urltxt = R"(https://api-free.deepl.com/v2/translate?auth_key=1bfdbee3-6605-3752-dd94-3b5ae775f090:fx&text={TEXT}&target_lang={LANG})";
    auto urltxt2 = urltxt.replace("{TEXT}", msg).replace("{LANG}",dest_lang);
    QUrl url = QUrl(urltxt2);

    QByteArray postData;

    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);

    QNetworkRequest request(url);

    QUrlQuery params;
    params.addQueryItem("POST", R"(/v2/translate?auth_key=1bfdbee3-6605-3752-dd94-3b5ae775f090:fx> HTTP/1.0)");
    params.addQueryItem("Host:", R"(api-free.deepl.com)");
    params.addQueryItem("User-Agent:", R"(wcode1)");
    params.addQueryItem("Accept:", R"(*/*)");
    params.addQueryItem("Content-Length:", R"(4096)");
    params.addQueryItem("content-type", R"(application/x-www-form-urlencoded)");

    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinish(QNetworkReply*)));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

    //QHttpMultiPart http;

    //QHttpPart receiptPart;
    //receiptPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
    //receiptPart.setBody(postData);

    //http.append(receiptPart);

    mgr->post(request, params.query().toUtf8());
}

void HttpHelper::onFinish(QNetworkReply *rep)
{
    QString err;
    auto url = rep->request().url();
    QByteArray b;

    if(rep->error() != QNetworkReply::NoError) {
        err = rep->errorString();
        int statusCode = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        auto a =  QVariant(statusCode).toString();
        zInfo("error: "+err+ " code: "+a);
    } else {
        QByteArray location = rep->rawHeader(QString("Location").toLocal8Bit());
        if (location.size() > 0) {
            zInfo("redirect: "+QString(location));
            Download("", location);
            return;
        } else {
            b = rep->readAll();

            if(b.isEmpty())
            {
                err = "no reply";
            }

            if(url.hasQuery()){
                auto queryString = url.query();
                auto q = QUrlQuery(queryString);
                if(q.hasQueryItem(KEY)){
                    QString keyString=q.queryItemValue(KEY);
                    auto guid = zShortGuid::Decode(keyString);
                    auto action = _actions[guid];

                    if(!err.isEmpty())
                    {
                        emit ResponseErr(guid, action.action);
                    }
                    else{
                        QString msg;
                        int ix1 = b.indexOf('\n');
                        if(ix1>20) ix1=20;
                        msg = b.mid(0,ix1);
                        if(b.length()>20) msg+="...";
                        msg+=" url: " + url.toDisplayString();

                        zInfo("reply: "+msg);
                        emit ResponseOk(guid, action.action, b);
                    }
                    if(action.mgr) action.mgr->deleteLater();
                    _actions.remove(guid);
                }
            }
        }
    }
    if(!err.isEmpty()){
        QString msg;
        msg+=" err: " + err;
        msg+=" url: " + url.toDisplayString();
        zInfo(msg);
    }
}
