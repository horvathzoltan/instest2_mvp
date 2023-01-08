#ifndef HTTPHELPER_H
#define HTTPHELPER_H

#include <QObject>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMap>
#include <QUuid>

class HttpHelper : public QObject
{
    Q_OBJECT

private:
    static const QString KEY;
public:
    explicit HttpHelper(QObject *parent = nullptr);
    bool init(const QString& host, int port);

    void SendPost(const QString& source_lang, const QString& dest_lang, const QString& msg);

    QUuid GetAction(const QString& action);//, const QString& query={});
    QUuid GetAction2(const QString& action, const QString& query);
    QUuid Download(const QString& action, const QString &url);
    QUuid DownloadFromHost(const QString& action, const QString &path);

    //QString url(){ return _url.host();}
public slots:
    void onFinish(QNetworkReply *rep);
signals:
    void ResponseErr(const QUuid& guid, const QString& key);
    void ResponseOk(const QUuid& guid, const QString& key, QByteArray s);

private:
    bool _inited = false;
    QUrl _url;
    struct Action{
        QString action;
        QNetworkAccessManager *mgr;
    };

    QMap<QUuid, Action> _actions;
    static QNetworkRequest CreateRequest(const QUrl& url);
    QUuid RegisterAction(const QString& action, QNetworkAccessManager* mgr);
    QNetworkAccessManager* CreateMgr();
};

#endif // HTTPHELPER_H
