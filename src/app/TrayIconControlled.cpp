#include "TrayIconControlled.h"
#include "helpers.h"
#include "winMouseLoop.h"
#include "winController.h"


TrayIconControlled::TrayIconControlled(QObject* parent = nullptr, int initial_value = 50, QColor color = QColor(0, 255, 0, 255), int min_val = 0, int max_val = 100, int step = 5) :
    QSystemTrayIcon(parent),
    value(initial_value),
    font_color(color),
    min_val(min_val),
    max_val(max_val),
    step(step)
{
    update_icon();

#ifdef Q_OS_WIN
    connect(&Controller::getInstance(), &Controller::updated, this, &TrayIconControlled::mouse_over);
#endif
}

TrayIconControlled::~TrayIconControlled()
{

}

void TrayIconControlled::update_icon()
{
    QString s;

    if (std::abs(value) > 99)
    {
        s = "00";
    }

    else { s = QString::number(value); }

    QIcon iconText = createIconFromText(s, font_color, this->geometry());
    setIcon(iconText);

}


int TrayIconControlled::get_value()
{
    return value;
}

void TrayIconControlled::set_identifier(int id, QString code)
{
    setObjectName(QString("%1_%2_%3").arg(receiverType).arg(id).arg(code));
}

void TrayIconControlled::set_step(int new_step)
{
    step = new_step;
}

void TrayIconControlled::set_color(QColor color)
{
    font_color = color;

    update_icon();
}


void TrayIconControlled::set_lock(bool lock) {
    locked = lock;
}

void TrayIconControlled::lock() {
    locked = true;
}

void TrayIconControlled::unlock() {
    locked = false;
}

void TrayIconControlled::update_value(int& val, QObject& senderObj)
{
    if (!locked)
    {
        QString senderName = senderObj.objectName();

        if (senderName != "")
        {
            try
            {
                QStringList id_components_receiver = (objectName()).split("_");

                if (id_components_receiver.length() > 1)
                {
                    QString receiverType = id_components_receiver.value(id_components_receiver.length() - 3);
                    receiver_id = (id_components_receiver.value(id_components_receiver.length() - 2)).toInt();
                    receiver_code = id_components_receiver.value(id_components_receiver.length() - 1);
                }

                else { throw 0; }
            }

            catch (...)
            {
                receiver_id = -1;
            }

            QStringList id_components_sender = senderName.split("_");
            QString senderType = id_components_sender.value(id_components_sender.length() - 3);
            int sender_id = (id_components_sender.value(id_components_sender.length() - 2)).toInt();
            QString sender_code = id_components_sender.value(id_components_sender.length() - 1);

            if (sender_id == receiver_id && senderType != receiverType)
            {
                if (value != val)
                {
                    value = val;
                    update_icon();

                    emit value_changed(value);
                }
            }
        }
    }
}

void TrayIconControlled::mouse_over(const struct mouseSignal& input)
{
    if (!locked)
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

                update_icon();

                emit value_changed(value);

            }

            else if (input.mouse.wheel == -1 && value > min_val)
            {
                value -= step;

                if (value < min_val) { value = min_val; }

                update_icon();

                emit value_changed(value);

            }
        }
    }
}