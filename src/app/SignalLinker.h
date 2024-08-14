#pragma once
#include <QObject>
#include <QtCore>
#include <QList>
#include <QMap>
#include <QVariant>

#include <QSystemTrayIcon>

#include "MonitorHandler.h"
#include "CustomSlider.h"
#include "TrayIconControlled.h"

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
    QList<CustomSlider*> registered_sliders;
    QList<TrayIconControlled*> registered_trayIcons;

public:
    QMap<int, QMap<QString, QVariant>> monitor_global_status;
    QMap<QString, QMap<QString, QVariant>> global_settings;

public:
    ~Linker();
    static Linker& getInstance();

public:
    void register_monitor(Monitor* monitor);
    void register_slider(CustomSlider* slider);
    void register_icon(TrayIconControlled* icon);

    QList<Monitor*> get_monitors();
    QList<CustomSlider*> get_sliders();
    QList<TrayIconControlled*> get_icons();

    Monitor* get_monitor_byID(int& id);
    Monitor* get_monitor_byName(QString& name);


public slots:
    void receive_signal();
    
    // Slot to receive update on mouse data from controller
    void receive_mouse_update(const struct inSignal&);

    // Slot to receive value update from slider/tray icon
    void receive_value_update(int&);

    // Receive value update for a specific code
    void receive_monitor_value(uint16_t& code, int& value);
    void receive_monitor_value_ID(uint16_t& code, int& value, int& id);

    // Receive Tray Icon Clicked
    void receive_icon_click(QSystemTrayIcon::ActivationReason reason);

    // Add Slider
    void receive_slider_add_request();
    void receive_add_slider(uint16_t&, QColor&, bool&);

    //Receive update to monitor information
    void receive_monitor_status_update(int& id_, QMap<QString, QVariant>& update_);

    void receive_monitor_setting(int&, QString&, uint16_t&);


signals:
    void emit_mouse_update(const struct inSignal& output);
    void emit_value_update(int& value, QObject&);

    void emit_monitor_value_update(uint16_t& code, int& value);

    void emit_icon_click(QSystemTrayIcon::ActivationReason reason);

    void emit_slider_add_request();
    void emit_add_slider(uint16_t&, QColor&, bool&);
};


