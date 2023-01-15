#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QHostAddress>
#include <QSet>
#include <QString>
#include <QThread>
#include <QUuid>

class FindPiThread : public QThread
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

    void run() override;
public:    
     FindPiThread(Model m, QUuid guid);
signals:
    void resultReady(const Result &s);
};

#endif // WORKERTHREAD_H
