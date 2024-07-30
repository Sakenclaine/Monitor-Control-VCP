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
