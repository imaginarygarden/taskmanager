#include "qconsolelistener.h"

#include <QTextStream>

void QConsoleListener::requestInput() {
    QTextStream{stdout} << "> " << Qt::flush;
    QTextStream stream{stdin};
    emit newLine(stream.readLine());
}

void QConsoleListener::start() {
    if (m_thread.isRunning()) {
        return;
    }

    m_notifier.moveToThread(&m_thread);

    QObject::connect(&m_thread, &QThread::finished, &m_notifier,
                     &QObject::deleteLater);

    QObject::connect(&m_notifier,
#ifdef Q_OS_WIN
                     &QWinEventNotifier::activated
#else
        &QSocketNotifier::activated
#endif
                     ,
                     [this]() {
                         QTextStream stream{stdin};
                         emit this->newLine(stream.readLine());
                     });

    m_thread.start();
}

void QConsoleListener::stop() {
    if (!m_thread.isRunning()) {
        return;
    }

    m_thread.quit();
    m_thread.wait();
}
