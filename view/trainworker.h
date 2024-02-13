#ifndef TRAINWORKER_H
#define TRAINWORKER_H

#include <QEventLoop>
#include <QObject>
#include <QRunnable>
#include <QScopedPointer>
#include <QThread>
#include <QTimer>

#include "../controller/controller.h"

class TrainWorker : public QObject, public QRunnable {
  Q_OBJECT
 public:
  explicit TrainWorker(s21::Controller *c, QObject *parent = nullptr);
  ~TrainWorker();
  void run() {
    emit updateStatus("Training process started...");
    emit updateStatus(
        "Initializing model and loading training and testing sources...");
    controller->initModel(isMatrixModel);
    emit updateStatus("Model succesfully initialized.");
    for (int i = 0; i < epochesAmount; i++) {
      std::string message;
      emit updateStatus("Training for epoch #" + std::to_string(i + 1) + "...");
      message = controller->trainModelForOneEpoch(
          isMatrixModel, evaluateModelAfterEachEpoch, 1, i + 1, miniBatchSize,
          learningConstant);
      emit updateStatus("Epoch finished.");
      emit updateStatus(message);
    }
    emit finished();
  }
  void setParameters(bool isMatrixM, bool evalModel, float tRatio, int eAmount,
                     int mBatchSize, float lConstant) {
    evaluateModelAfterEachEpoch = evalModel;
    isMatrixModel = isMatrixM;
    testingRatio = tRatio;
    epochesAmount = eAmount;
    miniBatchSize = mBatchSize;
    learningConstant = lConstant;
  }
  void delay(int millisecondsWait) {
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
  }

 signals:
  void updateStatus(std::string message);
  void finished();

 private:
  s21::Controller *controller;
  bool isMatrixModel;
  float testingRatio;
  int epochesAmount;
  int miniBatchSize;
  float learningConstant;
  bool evaluateModelAfterEachEpoch;
};

#endif  // TRAINWORKER_H
