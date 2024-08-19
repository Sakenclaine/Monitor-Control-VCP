#include "SignalLinker.h"

#include "MonitorHandler.h"

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

    emit send_lock(!chks.contains(true));
    emit send_checked_monitors(chkd_mons);
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
}






