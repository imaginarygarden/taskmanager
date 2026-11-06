#include "qtaskworker.h"

#include <QTextStream>

void QTaskWorker::run() {
    qsizetype iterator{};

    while (iterator < m_tasks.size() && !QThread::isInterruptionRequested()) {
        QTask *task{m_tasks[iterator]};

        emit log(QString{"[Worker] Processing: %1"}.arg(task->name()));

        QTask::QTaskResult result{task->process()};

        bool isLogged{
            result == QTask::QTaskResult::SUCCESS ||
                result == QTask::QTaskResult::FAIL,
        };

        if (isLogged) {
            QString status{result == QTask::QTaskResult::SUCCESS ? "Completed"
                                                                 : "Failed"};

            emit log(QString{"[Worker] %1: %2"}.arg(status, task->name()));
            emit task->statusUpdated(task->name(), task->status());
        } else {
            emit log("\n");
        }

        ++iterator;
    }
}
