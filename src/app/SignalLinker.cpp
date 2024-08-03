#include "SignalLinker.h"

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

void Linker::register_slider(CustomSlider* slider)
{
    registered_sliders.append(slider);
}

void Linker::register_icon(TrayIconControlled* icon)
{
    registered_trayIcons.append(icon);
}

QList<Monitor*> Linker::get_monitors()
{
    return registered_monitors;
}

QList<CustomSlider*> Linker::get_sliders()
{
    return registered_sliders;
}

QList<TrayIconControlled*> Linker::get_icons()
{
    return registered_trayIcons;
}

void Linker::receive_signal()
{
    //qDebug("Linker received signal...");
    //qDebug() << &(this->get_instance());
    //qDebug() << sender();
    
}

void Linker::receive_mouse_update(const struct inSignal& input)
{
    emit emit_mouse_update(input);
}

void Linker::receive_value_update(int& value)
{
    QObject* senderObj = sender();
    
    //qDebug() << "Linker: Updated value from " << sender() << " "<< sender()->objectName() << " --> " << value;
    emit emit_value_update(value, *senderObj);
}

void Linker::receive_monitor_value(uint16_t& code, int& value)
{
    //qDebug() << "Code: " << code << " -- Value: " << value;

    emit emit_monitor_value_update(code, value);
}

void Linker::receive_icon_click(QSystemTrayIcon::ActivationReason reason)
{
    emit emit_icon_click(reason);
}

void Linker::receive_slider_add_request()
{
    emit emit_slider_add_request();
}

void Linker::receive_add_slider(uint16_t& code, QColor& color, bool& trayCheck)
{
    emit emit_add_slider(code, color, trayCheck);
}