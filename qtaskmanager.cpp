#include "qtaskmanager.h"

#include <QTextStream>

QTaskManager::QTaskManager(QObject *parent)
    : QObject{parent}
{
    renderMenu();

    m_listener.init();

    QObject::connect(&m_listener, &QConsoleListener::newLine, this, &QTaskManager::input);
}

void QTaskManager::renderMenu() {
    QTextStream stream{stdout};

    stream << "=== Qt Task Manager ===" << Qt::endl;

    for (std::uint8_t i{}; auto &[name, function] : m_menu) {
        stream << QString{"%1. %2"}.arg(QString::number(i + 1), name) << Qt::endl;
        ++i;
    }
}

void QTaskManager::addTask() {
    QTextStream{stdout} << "Adding task.." << Qt::endl;
}

void QTaskManager::listTasks() {
    QTextStream{stdout} << "Listing tasks.." << Qt::endl;
}

void QTaskManager::startTasks() {
    QTextStream{stdout} << "Starting tasks.." << Qt::endl;
}

void QTaskManager::pauseTasks() {
    QTextStream{stdout} << "Pausing tasks.." << Qt::endl;
}

void QTaskManager::showStats() {
    QTextStream{stdout} << "Showing stats.." << Qt::endl;
}

void QTaskManager::quit() {
    emit processClosed();
}

void QTaskManager::input(const QString &input) {
    bool conversionStatus{};
    int option{input.toInt(&conversionStatus)};

    bool isValidInput{conversionStatus && option <= m_menu.size() && option > 0};

    if (!isValidInput) {
        QTextStream{stdout} << QString{"Error: %1 is not valid input."}.arg(input) << Qt::endl;
        return;
    }

    m_menu[option - 1].second();
}