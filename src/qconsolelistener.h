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

class QConsoleListener final : public QObject {
    Q_OBJECT

#ifdef Q_OS_WIN
    QWinEventNotifier m_notifier{GetStdHandle(STD_INPUT_HANDLE)};
#else
    QSocketNotifier m_notifier{fileno(stdin), QSocketNotifier::Read, this};
#endif

    QThread m_thread{this};

  public:
    explicit QConsoleListener(QObject *parent = nullptr) : QObject{parent} {};
    ~QConsoleListener() override { stop(); };

  public slots:
    void requestInput();
    void start();
    void stop();

  signals:
    void newLine(const QString &input);
};

#endif // QCONSOLELISTENER_H
