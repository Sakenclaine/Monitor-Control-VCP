#pragma once
#include <QObject>
#include <QList>


class Monitor;
class SliderWidget;


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
    QList<int> checked_monitors;

    QList<SliderWidget*> registered_sliders;

public:
    void register_monitor(Monitor* monitor);
    void register_monitor(QList<Monitor*> monitors);

    QList<Monitor*> get_monitors();
    const QList<int>& get_checked_monitors();

    Monitor* get_monitor_byID(int& id);
    Monitor* get_monitor_byName(QString& name);

    void register_slider(SliderWidget* sldr);
    QList<SliderWidget*> get_sliders();


public:
    ~Linker();
    static Linker& getInstance();

public slots:
    // Handle signals from monitor table
    void receive_lock(bool);
    void receive_monitor_selection(QString&, int&);

    // Handle value updates between sliders and tray icons
    void receive_value_update(int value);

    // Manage Signals to monitor
    void receive_discrete_setting(int& monitorID, QString& cde_str, uint16_t& value);
    void receive_monitor_settingID(int& monitorID, QString& cde_str, uint16_t& value);
    void receive_monitor_setting(QString& cde_str, int value);


signals:
    void send_lock(bool);
    void send_checked_monitors(QList<int>);
    void send_selected_monitor(QString&, int&);

    // Handle value updates between sliders and tray icons
    void send_value_update(int&, QObject&);

};


