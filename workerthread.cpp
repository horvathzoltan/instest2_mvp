#include "workerthread.h"
#include "ipscanner.h"

FindPiThread::Model::Model(){}

FindPiThread::Model::Model(QHostAddress host, QSet<int> ports, int timeout, int steps)
    : host(std::move(host)),
      ports(std::move(ports)),      
      timeout(std::move(timeout)),
      steps(std::move(steps))
{}

FindPiThread::FindPiThread(Model m, QUuid guid){
    _model = m;
    _guid = guid;
}

void FindPiThread::run() {
    auto a= IpScanner::Scan(_model.host, 1, 254, _model.ports, _model.timeout, _model.steps);

    Result result;
    result.ipList = a;
    result.guid = _guid;
    emit resultReady(result);
}

