#include <QApplication>

#include "mainwindow.h"
#include "winController.h"




int main(int argc, char* argv[])
{
    Controller::getInstance().start_worker();

    // Setup main application 
    QApplication a(argc, argv);

    // Create main window and show it
    MainWindow w;
    w.show();


    return a.exec();
}



