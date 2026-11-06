#include "qtaskmanager.h"

#include <QTextStream>

QTaskManager::QTaskManager(QObject *parent) : QThread{parent} {
    QObject::connect(&m_listener, &QConsoleListener::newLine, this,
                     &QTaskManager::input, Qt::BlockingQueuedConnection);

    QObject::connect(&m_processor, &QTaskProcessor::log, this,
                     &QTaskManager::log);
}

void QTaskManager::clearScreen() {
    printf("\033[2J"); // Clear Screen
    printf("\033[3J"); // Clear Scrollback
}

void QTaskManager::renderMenu() {
    QTextStream stream{stdout};

    stream << "=== Qt Task Manager ===" << Qt::endl;
    stream << "" << Qt::endl;

    for (std::uint8_t iterator{}; auto &[name, function] : m_menu) {
        stream << QString{"%1. %2"}.arg(QString::number(iterator + 1), name)
               << Qt::endl;
        ++iterator;
    }

    stream << "" << Qt::endl;
}

void QTaskManager::renderLogs() {
    QTextStream stream{stdout};

    for (auto &log : m_logs) {
        stream << log << Qt::flush;
    }
}

void QTaskManager::renderAll() {
    clearScreen();
    renderMenu();
    renderLogs();
    m_listener.requestInput();
}

void QTaskManager::run() {
    while (!QThread::isInterruptionRequested()) {
        renderAll();
        QThread::msleep(QTaskManager::DEFAULT_TIMEOUT);
    }
}

void QTaskManager::input(const QString &input) {
    bool conversionStatus{};
    int option{input.toInt(&conversionStatus)};

    bool isValidInput{
        conversionStatus && option <= m_menu.size() && option > 0,
    };

    if (!isValidInput) {
        log(QString{"Error: %1 is not valid input.\n"}.arg(input));
        return;
    }

    const auto &[name, action] = m_menu[option - 1];

    action();

    QTextStream{stdout} << "" << Qt::endl;
}

void QTaskManager::log(const QString &message) {
    m_logs.push_back(QString{"%1\n"}.arg(message));
}
