#include <QApplication>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QDebug>

#include <QTranslator>

#include "mainwindow.h"
#include "winController.h"
#include "utilities.h"


#include "MonitorHandler.h"


int main(int argc, char* argv[])
{
    displayDevices();
    
    qDebug() << "\n\n\n";
    
    list_devices();

    //////////////////////////////////////////////////////////////////////

    qDebug() << "\n\n\n";


    Controller::getInstance().start_worker();

    // Setup main application 
    QApplication a(argc, argv);

    ApplicationManager appMngr;
    QString language = SettingsManager::getInstance().readSetting("Settings", "language").toString();

    QTranslator tlator;
    bool loadChk = false;

    if (language == "en") loadChk = tlator.load(":/i18n/MonitorControl_en");
    else if (language == "de") loadChk = tlator.load(":/i18n/MonitorControl_de");

    qDebug() << "Translation File: " << tlator.filePath() << " " << loadChk;

    a.installTranslator(&tlator);

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
    MainWindow w(&appMngr);
    w.show();


    return a.exec();
}



