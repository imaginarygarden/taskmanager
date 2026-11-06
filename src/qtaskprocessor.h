#ifndef QTASKPROCESSOR_H
#define QTASKPROCESSOR_H

#include <QObject>
#include <QThread>

#include "qtask.h"
#include "qtaskworker.h"

class QTaskProcessor : public QObject {
    Q_OBJECT

    static constexpr std::uint16_t DEFAULT_THREADS{1};

    QString requestInput(const QString &prompt);

    qsizetype m_succeeded{};
    QList<QTask *> m_tasks;
    QList<QTaskWorker *> m_workers;

  public:
    explicit QTaskProcessor(QObject *parent = nullptr) : QObject{parent} {}

  signals:
    void log(const QString &message);

  public slots:
    void addTask();
    void listTasks();
    void startTasks();
    void pauseTasks();
    void showStats();
    void taskUpdated(const QString &name, const QTask::QTaskStatus &status);
    void taskStarted(const QString &name);
};

#endif
