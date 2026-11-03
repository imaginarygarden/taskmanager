#ifndef QTASKMANAGER_H
#define QTASKMANAGER_H

#include <QObject>

#include "qconsolelistener.h"

class QTaskManager : public QObject
{
    Q_OBJECT

    using MenuOption = std::pair<QString, std::function<void()>>;

    QConsoleListener m_listener;

    QList<MenuOption> m_menu{
        {"Add task", std::bind(&QTaskManager::addTask, this)},
        {"List tasks", std::bind(&QTaskManager::listTasks, this)},
        {"Start task processing", std::bind(&QTaskManager::startTasks, this)},
        {"Pause processing", std::bind(&QTaskManager::pauseTasks, this)},
        {"Show statistics", std::bind(&QTaskManager::showStats, this)},
        {"Quit", std::bind(&QTaskManager::quit, this)},
    };

    void renderMenu();
    void addTask();
    void listTasks();
    void startTasks();
    void pauseTasks();
    void showStats();
    void quit();
public:
    explicit QTaskManager(QObject *parent = nullptr);

signals:
    void processClosed();

private slots:
    void input(const QString &input);
};

#endif // QTASKMANAGER_H
