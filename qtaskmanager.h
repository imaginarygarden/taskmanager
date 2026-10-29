#ifndef QTASKMANAGER_H
#define QTASKMANAGER_H

#include <QObject>

class QTaskManager : public QObject
{
    Q_OBJECT


    void renderMenu();
    void addTask();
    void listTasks();
    void startTasks();
    void pauseTasks();
    void showStats();
    void quit();

    QList<std::function<void()>> m_menu{std::bind(&QTaskManager::listTasks, this, std::placeholders::_1)};
public:
    explicit QTaskManager(QObject *parent = nullptr);

signals:
    void processClosed();
};

#endif // QTASKMANAGER_H
