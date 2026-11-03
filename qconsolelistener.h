#ifndef QCONSOLELISTENER_H
#define QCONSOLELISTENER_H

#include <QObject>
#include <QThread>

#ifdef Q_OS_WIN
#include <QWinEventNotifier>
#include <windows.h>
#else
#include <QSocketNotifier>
#endif

class QConsoleListener : public QObject
{
    Q_OBJECT

#ifdef Q_OS_WIN
    QWinEventNotifier m_notifier{GetStdHandle(STD_INPUT_HANDLE)};
#else
    QSocketNotifier m_notifier{fileno(stdin), QSocketNotifier::Read, this};
#endif

    QThread m_thread;

public:
    explicit QConsoleListener(QObject *parent = nullptr) : QObject{parent} {};
    virtual ~QConsoleListener() {m_thread.quit(); m_thread.wait();};

    void init();

signals:
    void newLine(const QString &input);

};

#endif // QCONSOLELISTENER_H
