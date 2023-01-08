#include "zshortguid.h"


QString zShortGuid::Encode(const QUuid& g){
    auto b = g.toRfc4122().toBase64(QByteArray::Base64UrlEncoding);
    QString e(b);
    e = e.replace('/', '_').replace('+', '-').left(22);// - "=="
    return e;
}

QUuid zShortGuid::Decode(const QString& s){
      QString x(s);
      x = x.replace('_', '/').replace('-', '+') + "==";
      auto o = QByteArray::fromBase64(x.toLocal8Bit());
      QUuid a = QUuid::fromRfc4122(o);
      return a;
    }
