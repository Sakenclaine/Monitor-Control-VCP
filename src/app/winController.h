#pragma once

#include <QtCore>
#include <QString>
#include <QObject>
#include <QThread>

#include "winMouseLoop.h"


class Worker : public QObject
{
    Q_OBJECT

public:
    Worker();
    ~Worker();

private:
    bool quit = false;

public slots:
    void process();
    void stop();

signals:
    void finished();
    void update_mouse_pos(const struct mouseSignal& input);
};


class Controller : public QObject
{
    Q_OBJECT

private:
    explicit Controller(QObject* parent = nullptr);

    QThread* thread;

public:
    ~Controller();
    static Controller& getInstance();

public slots:
    void start_worker();
    void stop_worker();

    void receive_update(const struct mouseSignal&);

signals:
    void startP();
    void stopP();

    void updated(const struct mouseSignal& input);
};
