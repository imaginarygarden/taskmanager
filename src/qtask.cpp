#include "qtask.h"

#include <QThread>
#include <QTimer>
#include <mutex>

QTask::QTaskResult QTask::process() {
    if (!m_mutex.tryLock()) {
        return QTaskResult::TAKEN;
    }

    std::unique_lock<QMutex> locker{m_mutex, std::adopt_lock};

    if (m_status != QTaskStatus::PENDING) {
        return QTaskResult::COMPLETED;
    }

    start();

    QThread::msleep(QTask::DEFAULT_TIMEOUT);

    bool isFailed = m_rng.bounded(2) == 1;

    if (isFailed) {
        fail();
        return QTaskResult::FAIL;
    }

    complete();
    return QTaskResult::SUCCESS;
}

void QTask::start() {
    if (m_status == QTaskStatus::PENDING) {
        m_status = QTaskStatus::PROCESSING;
        emit taskStarted(m_name);
    }
}

void QTask::complete() {
    if (m_status == QTaskStatus::PROCESSING) {
        m_status = QTaskStatus::COMPLETE;
    }
}

void QTask::fail() {
    if (m_status == QTaskStatus::PROCESSING) {
        m_status = QTaskStatus::FAILED;
    }
}
