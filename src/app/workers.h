#pragma once

#include <QtCore>
#include <QString>
#include <QObject>
#include <QThread>

#include "RawData.h"


class Worker : public QObject
{
    Q_OBJECT

public:
    Worker();
    ~Worker();

private:
    struct inSignal input;
    bool quit = false;

public slots:
    void process();
    void stop();

signals:
    void finished();
    void update_mouse_pos(const struct inSignal& input);
};


class Controller : public QObject
{
    Q_OBJECT

private:
    QThread* thread;

    int i = 0;

private:
    explicit Controller(QObject* parent = nullptr);

public:
    ~Controller();
    static Controller& getInstance();

    void stop_worker();

public slots:
    void receive_update(const struct inSignal&);

signals:
    void operate();
    void stop();

    void updated(const struct inSignal& input);
};
