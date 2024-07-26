#pragma once
#include <QObject>
#include <QtCore>

//Singleton Class: https://stackoverflow.com/questions/45059490/how-to-connect-signals-to-the-slots-of-all-instances-of-a-class (not working)

//https://isocpp.org/wiki/faq/ctors#static-init-order
//https://stackoverflow.com/questions/59405661/singleton-with-qobject-inheritance-qt

class Linker : public QObject
{
    Q_OBJECT

private:
    explicit Linker(QObject* parent);

public:
    ~Linker();

    static Linker& getInstance();

    void sendSignal();


public slots:
    void receive_signal();
    
    // Slot to receive update on mouse data from controller
    void receive_mouse_update(const struct inSignal&);

    // Slot to receive value update from slider/tray icon
    void receive_value_update(int&);

signals:
    void mySignal();

    void emit_mouse_update(const struct inSignal& output);
    void emit_value_update(int& value, QObject&);
};


