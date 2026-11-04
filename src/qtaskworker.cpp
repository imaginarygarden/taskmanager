#include "qtaskworker.h"

#include <QTextStream>

void QTaskWorker::run() {
    QTextStream stream{stdout};
    qsizetype iterator{};

    while (iterator < m_tasks.size() && !QThread::isInterruptionRequested()) {
        QTask *task{m_tasks[iterator]};

        stream << QString{"[Worker] Processing: %1"}.arg(task->name())
               << Qt::endl;

        QTask::QTaskResult result{task->process()};

        bool isLogged{
            result == QTask::QTaskResult::SUCCESS ||
                result == QTask::QTaskResult::FAIL,
        };

        if (isLogged) {
            QString status{result == QTask::QTaskResult::SUCCESS ? "Completed"
                                                                 : "Failed"};

            stream << QString{"[Worker] %1: %2"}.arg(status, task->name())
                   << Qt::endl;
        }

        ++iterator;
    }
}
