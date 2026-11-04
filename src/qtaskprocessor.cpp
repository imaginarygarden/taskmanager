#include "qtaskprocessor.h"
#include "qtaskworker.h"

#include <QTextStream>
#include <QtNumeric>
#include <cstdint>
#include <qobject.h>

namespace {

QString statusToString(const QTask::QTaskStatus &status) {
    switch (status) {
    case QTask::QTaskStatus::PENDING:
        return "Pending";
        break;
    case QTask::QTaskStatus::PROCESSING:
        return "Processing";
        break;
    case QTask::QTaskStatus::COMPLETE:
        return "Complete";
        break;
    case QTask::QTaskStatus::FAILED:
        return "Failed";
        break;
    default:
        return "Unknown";
        break;
    }
}

}; // namespace

void QTaskProcessor::addTask() {
    QTextStream outputStream{stdout};

    outputStream << "Enter task name: " << Qt::flush;

    QTextStream inputStream{stdin};
    QTask *task{new QTask{inputStream.readLine(), this}};

    QObject::connect(task, &QTask::taskStarted, this,
                     &QTaskProcessor::taskStarted);

    QObject::connect(task, &QTask::taskCompleted, this,
                     &QTaskProcessor::taskCompleted);

    QObject::connect(task, &QTask::taskFailed, this,
                     &QTaskProcessor::taskFailed);

    m_tasks.append(task);
    outputStream << QString{"Task added: %1 [%2]"}.arg(
                        task->name(), statusToString(task->status()))
                 << Qt::endl;
}

void QTaskProcessor::listTasks() {
    QTextStream outputStream{stdout};

    for (std::uint8_t iterator{}; auto &task : m_tasks) {
        outputStream << QString{"%1. %2 [%3]"}.arg(
                            QString::number(iterator + 1), task->name(),
                            statusToString(task->status()))
                     << Qt::endl;

        ++iterator;
    }
}

void QTaskProcessor::startTasks() {
    QTextStream{stdout} << "Starting task processor.." << Qt::endl;

    for (std::uint16_t i{}; i < QTaskProcessor::DEFAULT_THREADS; ++i) {
        QTaskWorker *worker{new QTaskWorker{m_tasks, this}};
        worker->start();
        m_workers.append(worker);
    }
}

void QTaskProcessor::pauseTasks() {
    for (auto &worker : m_workers) {
        worker->requestInterruption();
        worker->wait();
    }
}

void QTaskProcessor::showStats() {
    qsizetype failed{qAbs(m_tasks.size() - m_succeeded)};

    QTextStream{stdout}
        << QString{"Stats: %1 (succeeded) / %2 (failed) = %3% success rate"}
               .arg(QString::number(m_succeeded), QString::number(failed),
                    QString::number(m_succeeded / failed))
        << Qt::endl;
}

void QTaskProcessor::taskStarted(const QString &name) {
    QTextStream{stdout} << QString{"[Manager] Task started: %1"}.arg(name)
                        << Qt::endl;
}

void QTaskProcessor::taskCompleted(const QString &name) {
    ++m_succeeded;

    QTextStream{stdout} << QString{"[Manager] Task completed: %1"}.arg(name)
                        << Qt::endl;
}

void QTaskProcessor::taskFailed(const QString &name) {
    QTextStream{stdout} << QString{"[Manager] Task failed: %1"}.arg(name)
                        << Qt::endl;
}
