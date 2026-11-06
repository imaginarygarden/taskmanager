#ifndef QTASKMANAGER_H
#define QTASKMANAGER_H

#include <QObject>
#include <QMutex>

#include "qconsolelistener.h"
#include "qtaskprocessor.h"

class QTaskManager final : public QThread {
    Q_OBJECT

    static constexpr unsigned long DEFAULT_TIMEOUT{10};

    using MenuOption = std::pair<QString, std::function<void()>>;

    QConsoleListener m_listener{this};
    QTaskProcessor m_processor{this};

    QList<QString> m_logs;
    QList<MenuOption> m_menu{
        {"Add task", [this] { m_processor.addTask(); }},
        {"List tasks", [this] { m_processor.listTasks(); }},
        {"Start task processing", [this] { m_processor.startTasks(); }},
        {"Pause processing", [this] { m_processor.pauseTasks(); }},
        {"Show statistics", [this] { m_processor.showStats(); }},
        {"Clear screen", [this] { m_logs.clear(); }},
        {"Quit", [this] { emit exited(); }},
    };

    void clearScreen();
    void renderMenu();
    void renderLogs();
    void renderAll();

  protected:
    void run() override;

  public:
    explicit QTaskManager(QObject *parent = nullptr);

  signals:
    void exited();

  private slots:
    void input(const QString &input);
    void log(const QString &message);
};

#endif // QTASKMANAGER_H
