I'd recommend building a console-based task manager with a simulated background worker in Qt.

It's a good middle ground for your experience: you already know C++20, so you don't need another exercise in classes, loops, or basic syntax. Instead, you'll learn how Qt structures applications, how its event loop works, and how signals and slots let independent objects communicate.

The key is to build something that feels like a small real application, without touching Widgets or QML.

# Project: Console Task Manager

Imagine running a program like this:

text

```
=== Qt Task Manager ===

1. Add task
2. List tasks
3. Start task processing
4. Pause processing
5. Show statistics
6. Quit

> 1
Enter task name: Download files
Task added: Download files [Pending]

> 1
Enter task name: Process data
Task added: Process data [Pending]

> 3
Starting task processor...

[Worker] Processing: Download files
[Manager] Task started: Download files
[Worker] Completed: Download files
[Manager] Task completed: Download files

[Worker] Processing: Process data
```

The processing should happen asynchronously, so the console can remain responsive while the worker does its job. You can start with simulated work using `QTimer`, then introduce a real worker thread later.

This project teaches you something important: Qt is not just a collection of C++ libraries; it also provides an event-driven programming model.

## What you'll learn

1. Signals and slots

Create your own signals such as `taskAdded`, `taskStarted`, and `taskCompleted`. Connect them to slots in other objects and observe how events propagate through the application.

2. `QObject` and object ownership

Make separate classes for the task manager and task processor. Learn how QObject parent-child ownership works, and why object lifetime matters when using connections.

3. The event loop and `QTimer`

Schedule work without manually polling in a loop. Understand why Qt applications call `app.exec()` and how timers deliver events.

4. Connection types and lifecycle

Experiment with automatic, direct, and queued connections, and understand what happens when a connected object is destroyed.

## Your implementation roadmap

I'd approach this in six milestones. Each one should leave you with a working program rather than a large unfinished codebase.

### Milestone 1 — Build the Qt foundation

Create a Qt Console Application using Qt 6 and CMake.

Your application should use `QCoreApplication`, not `QApplication`, and should have no UI dependencies.

Start with two classes:

- `TaskManager` — stores tasks and manages their state.
- `TaskProcessor` — takes pending tasks and simulates processing them.

Define a `Task` type containing an ID, a name, and a status.

cpp

```
enum class TaskStatus
{
    Pending,
    Running,
    Completed,
    Failed
};
```

For now, call the manager's methods directly. Make sure you understand the CMake build and the basic Qt project structure.

### Milestone 2 — Introduce your own signals and slots

Make `TaskManager` inherit from `QObject`. Use `Q_OBJECT` and declare your signals and slots.

Your design should include signals similar to these:

cpp

```
signals:
    void taskAdded(int id, const QString& name);
    void taskStarted(int id);
    void taskCompleted(int id);
    void taskFailed(int id, const QString& reason);
```

Your processor should emit events, and the manager should react to them. Use `QObject::connect()` to connect the relevant objects.

Your challenge:

- Adding a task emits an event.
- Starting a task emits an event.
- Completing a task emits an event.
- The manager prints a log message whenever it receives an event.

Important: A signal is not a normal function that you implement yourself. Qt generates the necessary machinery through its Meta-Object Compiler (MOC). You declare signals and emit them.

Try connecting one signal to a lambda first, then connect it to a member slot in another class. Compare the two approaches.

### Milestone 3 — Add asynchronous processing with `QTimer`

This is where the project starts to become distinctly Qt-like.

Instead of processing a task immediately, start a timer:

cpp

```
QTimer::singleShot(2000, this, &TaskProcessor::finishCurrentTask);
```

The idea is to simulate a task that takes two seconds to complete. When the timer fires, the processor emits `taskCompleted()` and moves on to the next task.

Implement these rules:

- Only one task can run at a time.
- Tasks are processed in FIFO order.
- The processor automatically advances to the next task.
- The application remains event-driven instead of using `sleep()` or a busy-wait loop.

One important trap: a blocking `std::cin` or `std::getline` call on the main thread prevents the event loop from processing timers while it waits for input. Initially, use a predefined task queue and let the event loop run. Add interactive console input afterward using a nonblocking approach or a separate input thread.

### Milestone 4 — Add pause, resume, and cancellation

Now you have enough infrastructure to add more interesting behavior.

- Pause: stop processing new work and preserve the remaining delay for the current task.
- Resume: continue from where the task was paused.
- Cancel: cancel the current task or remove a pending task.
- Shutdown: stop timers and ensure the processor finishes cleanly.

You'll discover that signals and slots don't automatically solve state management. Your objects still need clear rules about which states are valid and which transitions are allowed.

### Milestone 5 — Explore Qt's event semantics

This milestone is specifically about understanding Qt rather than adding features.

Experiment with:

- `Qt::AutoConnection`
- `Qt::DirectConnection`
- `Qt::QueuedConnection`
- Lambda connections and receiver context objects
- Disconnecting connections
- Destroying a receiver while other objects still exist

Try connecting a signal to a slot and printing when each part executes. This will help you understand that a connection is not always an asynchronous message: a direct connection invokes the slot immediately, while a queued connection schedules delivery through the receiver's event loop.

### Milestone 6 — Add a worker thread (optional stretch goal)

Only attempt this after the timer-based version works.

Move `TaskProcessor` to a `QThread`, and communicate with it through queued signals and slots.

Learn about:

- `QObject::moveToThread()`
- Thread affinity
- Queued signal delivery across threads
- Thread-safe shutdown
- Why you shouldn't casually manipulate a worker QObject from the main thread

This is a substantial step up in difficulty, so don't make it a requirement for finishing the core project.

## Suggested architecture

Here's the separation I'd aim for:

Console / application controller

Prints logs and handles commands

TaskManager : QObject

Owns task state and the pending queue

TaskProcessor : QObject

Runs tasks and emits lifecycle events

Signals communicate events; slots react to them.

Don't make everything inherit from `QObject`. Your `Task` data structure can remain a plain C++ type. Use `QObject` for objects that actually need Qt's object model, signals and slots, or related features.

## How to know you're ready to move on

You should be able to explain and demonstrate these things without relying on a tutorial:

- [ ]Why `QCoreApplication` and `app.exec()` are needed.
- [ ]Why `Q_OBJECT` is used and what MOC does.
- [ ]The difference between a signal and a slot.
- [ ]How to connect a signal to a member function, lambda, or another signal.
- [ ]When a slot runs immediately versus being queued.
- [ ]Why a `QTimer` is usually preferable to sleeping on the main thread.
- [ ]How QObject ownership and destruction affect connections.
- [ ]How to organize Qt classes into separate headers and source files with CMake.

## Official resources to use alongside the project

- Qt — [Signals & Slots](<https://doc.qt.io/qt-6/signalsandslots.html?utm_source=chatgpt.com>), the essential guide to the mechanism.
- [The Qt Object Model](<https://doc.qt.io/qt-6/object.html?utm_source=chatgpt.com>), for `QObject`, ownership, and object lifetime.
- [QCoreApplication](<https://doc.qt.io/qt-6/qcoreapplication.html?utm_source=chatgpt.com>), for the console application and event loop.
- [QTimer](<https://doc.qt.io/qt-6/qtimer.html?utm_source=chatgpt.com>), for event-driven task scheduling.
- [Qt 6 CMake documentation](<https://doc.qt.io/qt-6/cmake-get-started.html?utm_source=chatgpt.com>), for setting up your project properly.

My recommendation is to stop after Milestone 3 and rebuild the application from scratch once before moving on. If you can recreate the classes, connections, and timer-driven workflow without copying your previous implementation, you'll have a much stronger foundation for deciding whether you want to explore Qt Widgets or Qt Quick.