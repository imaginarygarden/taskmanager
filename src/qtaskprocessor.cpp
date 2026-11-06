#include "qtaskprocessor.h"
#include "qtaskworker.h"

#include <QTextStream>
#include <QtNumeric>
#include <cstdint>

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

QString QTaskProcessor::requestInput(const QString &prompt) {
    QTextStream outputStream{stdout};
    outputStream << prompt << Qt::flush;

    QTextStream inputStream{stdin};

    QString input{inputStream.readLine()};

    emit log(prompt + input);
    return input;
}

void QTaskProcessor::addTask() {

    QString name{requestInput("Enter task name: ")};

    QTask *task{new QTask{name, this}};

    QObject::connect(task, &QTask::taskStarted, this,
                     &QTaskProcessor::taskStarted);

    QObject::connect(task, &QTask::statusUpdated, this,
                     &QTaskProcessor::taskUpdated);

    m_tasks.append(task);
    emit log(QString{"Task added: %1 [%2]\n"}.arg(
        task->name(), statusToString(task->status())));
}

void QTaskProcessor::listTasks() {
    QString stream{};

    for (std::uint8_t iterator{}; auto &task : m_tasks) {
        stream += QString{"%1. %2 [%3]\n"}.arg(QString::number(iterator + 1),
                                               task->name(),
                                               statusToString(task->status()));

        ++iterator;
    }

    if (stream.length() > 0) {
        emit log(stream);
    }
}

void QTaskProcessor::startTasks() {
    emit log("Starting task processor..\n");

    for (std::uint16_t i{}; i < QTaskProcessor::DEFAULT_THREADS; ++i) {
        QTaskWorker *worker{new QTaskWorker{m_tasks, this}};

        QObject::connect(worker, &QTaskWorker::log, this, &QTaskProcessor::log);

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

    emit log(
        QString{"Stats: %1 (succeeded) / %2 (failed) = %3% success rate\n"}.arg(
            QString::number(m_succeeded), QString::number(failed),
            QString::number(m_succeeded / failed)));
}

void QTaskProcessor::taskUpdated(const QString &name,
                                 const QTask::QTaskStatus &status) {
    QString statusMessage{"failed"};

    if (status == QTask::QTaskStatus::COMPLETE) {
        m_succeeded++;
        statusMessage = "completed";
    }

    emit log(QString{"[Manager] Task %2: %1\n"}.arg(name, statusMessage));
}

void QTaskProcessor::taskStarted(const QString &name) {
    emit log(QString{"[Manager] Task started: %1"}.arg(name));
}
