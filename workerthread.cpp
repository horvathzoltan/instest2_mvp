#include "workerthread.h"

WorkerThread::Model::Model()
{

}

WorkerThread::Model::Model(QHostAddress host, QSet<int> ports, int timeout)
    : host(std::move(host)),
      ports(std::move(ports)),
      timeout(timeout) {}

WorkerThread::WorkerThread(Model m, QUuid guid){
    _model = m;
    _guid = guid;
}

