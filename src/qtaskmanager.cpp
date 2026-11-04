#include "qtaskmanager.h"

#include <QTextStream>

QTaskManager::QTaskManager(QObject *parent) : QObject{parent} {
    renderMenu();

    m_listener.start();

    QObject::connect(&m_listener, &QConsoleListener::newLine, this,
                     &QTaskManager::input, Qt::BlockingQueuedConnection);
}

void QTaskManager::renderMenu() {
    QTextStream stream{stdout};

    stream << "=== Qt Task Manager ===" << Qt::endl;

    for (std::uint8_t iterator{}; auto &[name, function] : m_menu) {
        stream << QString{"%1. %2"}.arg(QString::number(iterator + 1), name)
               << Qt::endl;
        ++iterator;
    }
}

void QTaskManager::input(const QString &input) {
    bool conversionStatus{};
    int option{input.toInt(&conversionStatus)};

    bool isValidInput{
        conversionStatus && option <= m_menu.size() && option > 0,
    };

    if (!isValidInput) {
        QTextStream{stdout}
            << QString{"Error: %1 is not valid input."}.arg(input) << Qt::endl;
        return;
    }

    const auto &[name, action] = m_menu[option - 1];

    action();

    QTextStream{stdout} << "" << Qt::endl;
}
