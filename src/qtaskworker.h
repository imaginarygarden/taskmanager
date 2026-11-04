#ifndef QTASKWORKER_H
#define QTASKWORKER_H

#include <QThread>

#include "qtask.h"

class QTaskWorker final : public QThread {
    Q_OBJECT
    Q_DISABLE_COPY(QTaskWorker)

    const QList<QTask *> &m_tasks;

  protected:
    void run() override;

  public:
    QTaskWorker(const QList<QTask *> &tasks, QObject *parent = nullptr)
        : m_tasks{tasks}, QThread{parent} {}
};

#endif
