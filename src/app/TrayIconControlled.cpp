#include "TrayIconControlled.h"
#include "helpers.h"
#include "RawData.h"


TrayIconControlled::TrayIconControlled(const QIcon& icon, QObject* parent = nullptr, int initial_value = 50, QColor color = QColor(0, 255, 0, 255), int min_val = 0, int max_val = 100, int step = 5) :
    QSystemTrayIcon(icon, parent),
    value(initial_value),
    font_color(color),
    min_val(min_val),
    max_val(max_val),
    step(step)
    {
    this->update_icon();
    }

TrayIconControlled::TrayIconControlled(QObject* parent = nullptr, int initial_value = 50, QColor color = QColor(0, 255, 0, 255), int min_val = 0, int max_val = 100, int step = 5) :
    QSystemTrayIcon(parent),
    value(initial_value),
    font_color(color),
    min_val(min_val),
    max_val(max_val),
    step(step)
{
    this->update_icon();
}

TrayIconControlled::~TrayIconControlled()
{

}

int TrayIconControlled::get_value()
{
    return value;
}



void TrayIconControlled::update_icon()
{
    QString s;
    
    if (std::abs(value) > 99)
    {
        s = tr("00");
    }

    else { s = QString::number(value); }

    QIcon test = createIconFromText(s, font_color, this->geometry());
    this->setIcon(test);

}

void TrayIconControlled::update_value(int& val, QObject& senderObj)
{
    QString senderName = senderObj.objectName();
    
    try
    {
        QStringList id_componentsReceiver = (this->objectName()).split("_");

        if (id_componentsReceiver.length() > 1)
        {
            QString receiverType = id_componentsReceiver.value(id_componentsReceiver.length() - 2);
            receiver_id = (id_componentsReceiver.value(id_componentsReceiver.length() - 1)).toInt();
        }

        else { throw 0; }
    }

    catch (...)
    {
        receiver_id = -1;
    }
    
    if (senderName != "")
    {
        QStringList id_components = senderName.split("_");
        QString senderType = id_components.value(id_components.length() - 2);
        int sender_id = (id_components.value(id_components.length()-1)).toInt();

        //qDebug() << "Tray " << this->objectName() << " Updated from: " << senderName;
        //qDebug() << "Sender: " << senderType << " .. " << sender_id << " Receiver: " << " .. " << receiver_id << receiverType;
        //qDebug() << "Current value: " << value << " Sended Value: " << val << "\n";
        
        if (sender_id == receiver_id && senderType != receiverType)
        {
            if (value != val)
            {     
                value = val;
                this->update_icon();

                emit value_changed(value);
            }
        }  
    }
}

void TrayIconControlled::mouse_over(const struct inSignal& input)
{
    QRect tray_rect = this->geometry();
    QPoint tray_center = this->geometry().center();

    bool mouse_inside = tray_rect.contains(input.mouse.xCoo, input.mouse.yCoo);

    if (mouse_inside)
    {
        
        
        if (input.mouse.wheel == 1 && value < max_val)
        {
            value += step;

            if (value > max_val) { value = max_val; }

            this->update_icon();

            emit value_changed(value);

        }

        else if (input.mouse.wheel == -1 && value > min_val)
        {
            value -= step;

            if (value < min_val) { value = min_val; }

            this->update_icon();

            emit value_changed(value);  
            
        }
    }
}