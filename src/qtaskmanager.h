#ifndef QTASKMANAGER_H
#define QTASKMANAGER_H

#include <QObject>
#include <QMutex>

#include "qconsolelistener.h"
#include "qtaskprocessor.h"

class QTaskManager final : public QObject {
    Q_OBJECT

    using MenuOption = std::pair<QString, std::function<void()>>;

    QConsoleListener m_listener{this};
    QTaskProcessor m_processor{this};

    QList<MenuOption> m_menu{
        {"Add task", [this] { m_processor.addTask(); }},
        {"List tasks", [this] { m_processor.listTasks(); }},
        {"Start task processing", [this] { m_processor.startTasks(); }},
        {"Pause processing", [this] { m_processor.pauseTasks(); }},
        {"Show statistics", [this] { m_processor.showStats(); }},
        {"Quit", [this] { emit quit(); }},
    };

    void renderMenu();

  public:
    explicit QTaskManager(QObject *parent = nullptr);

  signals:
    void quit();

  public slots:
    void input(const QString &input);
};

#endif // QTASKMANAGER_H
