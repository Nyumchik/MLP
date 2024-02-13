#include "trainworker.h"
TrainWorker::TrainWorker(s21::Controller *c, QObject *parent)
    : QObject{parent} {
  controller = c;
}

TrainWorker::~TrainWorker() {}
