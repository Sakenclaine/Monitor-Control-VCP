#include "SignalLinker.h"
#include "helpers.h"
#include "MonitorHandler.h"
#include "CustomSlider.h"

Linker::Linker(QObject* parent=nullptr) : QObject(parent)
{

}

Linker::~Linker()
{
}

Linker& Linker::getInstance()
{
    static Linker cntr;
    return cntr;
}

void Linker::register_monitor(Monitor* monitor)
{
    registered_monitors.append(monitor);
}

void Linker::register_monitor(QList<Monitor*> monitors)
{
    registered_monitors = monitors;
}

QList<Monitor*> Linker::get_monitors()
{
    return registered_monitors;
}

Monitor* Linker::get_monitor_byID(int& id)
{
    for (auto elem : registered_monitors)
    {
        if (elem->get_ID() == id) { return elem; }
    }

    return NULL;
}

Monitor* Linker::get_monitor_byName(QString& name)
{
    for (auto elem : registered_monitors)
    {
        if (elem->get_name() == name) { return elem; }
    }

    return NULL;
}

const QList<int>& Linker::get_checked_monitors()
{
    return checked_monitors;
}

void Linker::register_slider(SliderWidget* sldr)
{
    registered_sliders.append(sldr);
}

QList<SliderWidget*> Linker::get_sliders()
{
    return registered_sliders;
}



// SLOTS
void Linker::receive_lock(bool lck)
{
    QList<bool> chks;
    QList<int> chkd_mons;

    for (auto& elem : registered_monitors)
    {    
        bool chk = elem->get_status();
        chks.append(chk);

        if (chk) chkd_mons.append(elem->get_ID());
    }

    checked_monitors = chkd_mons;

    emit send_lock(!chks.contains(true));
    emit send_checked_monitors(chkd_mons);
}

void Linker::receive_monitor_selection(QString& name, int& id)
{
    qDebug() << name << " " << id;

}

void Linker::receive_icon_click(QSystemTrayIcon::ActivationReason reason)
{
    emit emit_icon_click(reason);
}


void Linker::delete_slider()
{
    QObject* obj = sender();

    if (obj != nullptr) {
        QString name = obj->objectName();

        qDebug() << "Deleting Slider " << name << " ...";

        int i = 0;
        for (auto elem : registered_sliders)
        {
            if (elem->objectName() == name)
            {
                elem->deleteLater();

                registered_sliders.removeAt(i);
                
                emit emit_slider_delete(n2hexstr(elem->get_code(), 2));
            }

            i++;
        }
    }
}


void Linker::receive_value_update(int value)
{
    QObject* obj = sender();

    if (obj != nullptr) { 
        qDebug() << obj->objectName() << " -> " << value; 
        emit send_value_update(value, *obj);
    }
}

void Linker::receive_discrete_setting(int& monitorID, QString& cde_str, uint16_t& value)
{
    qDebug() << "Monitor " << monitorID << " -- Code" << cde_str << " -- Value " << value;

    uint16_t code = hexstr2uint(cde_str);
    bool bChk = get_monitor_byID(monitorID)->set_feature(code, value);
}

void Linker::receive_monitor_settingID(int& monitorID, QString& cde_str, uint16_t& value)
{
    qDebug() << "Monitor " << monitorID << " -- Code" << cde_str << " -- Value " << value;

    uint16_t code = hexstr2uint(cde_str);

    bool bChk = get_monitor_byID(monitorID)->set_feature(code, value);
}

void Linker::receive_monitor_setting(QString& cde_str, int value)
{
    qDebug()  << " -- Code" << cde_str << " -- Value " << value;
    qDebug() << checked_monitors;

    uint16_t code = hexstr2uint(cde_str);

    for (auto& monitorID : checked_monitors)
    {
        bool bChk = get_monitor_byID(monitorID)->set_feature(code, value);
    }   
}






