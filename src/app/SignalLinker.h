#pragma once
#include <QObject>
#include <QtCore>
#include <QList>
#include <QVariant>

#include <QSystemTrayIcon>

#include "MonitorHandler.h"

//Singleton Class: https://stackoverflow.com/questions/45059490/how-to-connect-signals-to-the-slots-of-all-instances-of-a-class (not working)

//https://isocpp.org/wiki/faq/ctors#static-init-order
//https://stackoverflow.com/questions/59405661/singleton-with-qobject-inheritance-qt

class Linker : public QObject
{
    Q_OBJECT

private:
    explicit Linker(QObject* parent);

private:
    QList<Monitor*> registered_monitors;

public:
    ~Linker();
    static Linker& getInstance();

public:
    void register_monitor(Monitor* monitor);

    QList<Monitor*> get_monitors();


public slots:
    void receive_signal();
    
    // Slot to receive update on mouse data from controller
    void receive_mouse_update(const struct inSignal&);

    // Slot to receive value update from slider/tray icon
    void receive_value_update(int&);

    // Receive value update for a specific code
    void receive_monitor_value(uint16_t& code, int& value);

    // Receive Tray Icon Clicked
    void receive_icon_click(QSystemTrayIcon::ActivationReason reason);

    // Add Slider
    void receive_slider_add_request();
    void receive_add_slider(uint16_t&, QColor&, bool&);


signals:
    void emit_mouse_update(const struct inSignal& output);
    void emit_value_update(int& value, QObject&);

    void emit_monitor_value_update(uint16_t& code, int& value);

    void emit_icon_click(QSystemTrayIcon::ActivationReason reason);

    void emit_slider_add_request();
    void emit_add_slider(uint16_t&, QColor&, bool&);
};


