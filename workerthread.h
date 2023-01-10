#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QHostAddress>
#include <QSet>
#include <QString>
#include <QThread>
#include "ipscanner.h"

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    struct Model{
        Model();
        Model(QHostAddress host, QSet<int> ports, int timeout);

        QHostAddress host;
        QSet<int> ports;
        int timeout;
    };

    struct ModelR{
        QMap<QString, QSet<int>> ipList;
    };

private:

    Model _model;

public:
    WorkerThread(Model m);

private:
    void run() override {
        ModelR result;

        auto a= IpScanner::Scan(_model.host, 1, 254, _model.ports, _model.timeout);
            //IpScanner::Scan(a.first(), 1, 254, ports, timeout);

        result.ipList = a;
        emit resultReady(result);
    }
signals:
    void resultReady(const ModelR &s);
};

#endif // WORKERTHREAD_H
