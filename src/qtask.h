#ifndef QTASK_H
#define QTASK_H

#include <QObject>
#include <QMutex>
#include <QRandomGenerator>

#include <cstdint>
#include <qobject.h>

class QTask final : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(QTask)

  public:
    enum class QTaskStatus : std::uint8_t {
        PENDING,
        PROCESSING,
        COMPLETE,
        FAILED,
    };

    enum class QTaskResult : std::uint8_t {
        SUCCESS,
        FAIL,
        COMPLETED,
        TAKEN,
    };

    explicit QTask(QString name, QObject *parent = nullptr)
        : m_name{std::move(name)}, QObject{parent} {}

    QTaskResult process();

    QString name() const { return m_name; }
    QTaskStatus status() const { return m_status; }

  signals:
    void statusUpdated(const QString &name, const QTaskStatus &status);
    void taskStarted(const QString &name);

  private:
    static constexpr std::uint16_t DEFAULT_TIMEOUT{1000};

    QString m_name;
    QRandomGenerator m_rng{QRandomGenerator::securelySeeded()};
    QTaskStatus m_status{QTask::QTaskStatus::PENDING};
    QMutex m_mutex{};

    void start();
    void complete();
    void fail();
};

#endif
