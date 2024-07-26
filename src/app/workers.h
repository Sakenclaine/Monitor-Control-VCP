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
    struct inSignal get_mouse_data();

signals:
    void finished();
    void update_mouse_pos(const struct inSignal& input);
};


class Controller : public QObject
{
    Q_OBJECT

    QThread thread;

public:
    Controller();
    ~Controller();

public slots:
    void handleResults();
    void receive_update(const struct inSignal&);

signals:
    void operate();
    void stop();
    void get_data();
    void updated(const struct inSignal& input);
};
