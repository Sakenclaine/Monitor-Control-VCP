#include <QApplication>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QDebug>

#include "mainwindow.h"
#include "winController.h"


#include "MonitorHandler.h"

int main(int argc, char* argv[])
{
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    DWORD deviceNum = 0;
    while (EnumDisplayDevices(NULL, deviceNum, &dd, 0)) {
   	DumpDevice(dd, 0);
   	DISPLAY_DEVICE newdd = { 0 };
   	newdd.cb = sizeof(DISPLAY_DEVICE);
   	DWORD monitorNum = 0;
   	while (EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
   	{
   		DumpDevice(newdd, 4);
   		monitorNum++;
   	}
   	puts("");
   	deviceNum++;
    }

    qDebug() << "\n\n\n";


    Controller::getInstance().start_worker();

    // Setup main application 
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        auto choice = QMessageBox::critical(
            nullptr,
            QObject::tr("Systray"),
            QObject::tr("I couldn't detect any system tray on this system."),
            QMessageBox::Close | QMessageBox::Ignore
        );

        if (choice == QMessageBox::Close)
            return 1;
        // Otherwise "lurk": if a system tray is started later, the icon will appear.
    }

    // Prevent program from closing, when closing main window without tray icon
    QApplication::setQuitOnLastWindowClosed(false);

    // Create main window and show it
    MainWindow w;
    w.show();


    return a.exec();
}



