#include "qconsolelistener.h"

#include <QTextStream>
// #include <iostream>

void QConsoleListener::init() {
    m_notifier.moveToThread(&m_thread);

    QObject::connect(
        &m_thread , &QThread::finished,
        &m_notifier, &QObject::deleteLater
    );

    QObject::connect(&m_notifier,
#ifdef Q_OS_WIN
    &QWinEventNotifier::activated
#else
    &QSocketNotifier::activated
#endif
    ,[this]() {
        QTextStream{stdout} << "> ";
        QTextStream stream{stdin};
        emit this->newLine(stream.readLine());
    });

    m_thread.start();
}
