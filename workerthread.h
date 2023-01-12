#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QHostAddress>
#include <QSet>
#include <QString>
#include <QThread>
#include <QUuid>
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

    struct Result{
        QMap<QString, QSet<int>> ipList;
        QUuid guid;
    };

private:

    Model _model;
    QUuid _guid;

public:
    WorkerThread(Model m, QUuid guid);

private:
    void run() override {
        Result result;

        auto a= IpScanner::Scan(_model.host, 1, 254, _model.ports, _model.timeout);

        result.ipList = a;
        result.guid = _guid;
        emit resultReady(result);
    }
signals:
    void resultReady(const Result &s);
};

#endif // WORKERTHREAD_H
