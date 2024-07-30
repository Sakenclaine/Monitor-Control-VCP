#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>

#include <QDebug> 
#include <QEvent>
#include <QMetaEnum>




#include "mainwindow.h"
#include "helpers.h"
#include "workers.h"
#include "TrayIconControlled.h"
#include "MonitorHandler.h"
#include "CustomSlider.h"

#include "SettingsWidget.h"
#include "MonitorSettingsWidget.h"
#include "MonitorTable.h"

#include "SignalLinker.h"

// andreybutov https://gist.github.com/andreybutov/33783bca1af9db8f9f36c463c77d7a86
QString windowsAppPath()
{
#ifdef Q_OS_WIN
    return QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
#else
    return "";
#endif
}

void setAppToStartAutomatically(bool startAutomatically)
{
#if defined ( Q_OS_WIN )

    QString key = "Monitor-Control";

    QSettings registrySettings(
        "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        QSettings::NativeFormat);

    registrySettings.remove(key);

    if (startAutomatically) {
        registrySettings.setValue(key, QString("\"" + windowsAppPath() + "\""));
    }

    registrySettings.sync();

#endif
}




MainWindow::MainWindow(QWidget* parent) : 
    QMainWindow(parent)
{
    config_path = qApp->applicationDirPath() + "/config.ini";

    readSettings();
    
    #ifdef Q_OS_WIN
    init_monitors_WIN();

    #elif Q_OS_UNIX
    init_monitors_UNIX();

    #endif

    //Controller* cntr = new Controller;
    //cntr->operate();

    Controller::getInstance().operate();

    connect(&Controller::getInstance(), &Controller::updated, &Linker::getInstance(), &Linker::receive_mouse_update);
    
    setup();


    add_monitor_control_widget();

 }


MainWindow::~MainWindow()
{
};


void MainWindow::setup()
{
    // Create main widget and layout set as content for the main window
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    mainLayout = new QVBoxLayout;
    mainWidget->setLayout(mainLayout);

    createActions();
    createMonitorGroupBox();

    wSettings = new SettingsWidget();
    dialogueWidget = new PlaceholderWidget();

    mainLayout->addWidget(monitorGroupBox);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();

    trayMonitorMenu = trayIconMenu->addMenu("Monitor");
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    for (auto elem : registered_monitors)
    {
        auto action = new QAction(elem->get_name(), trayMonitorMenu);
        action->setCheckable(true);
        action->setChecked(elem->get_status());

        trayMonitorMenu->addAction(action);

        connect(action, &QAction::triggered, elem, &Monitor::set_status);
        connect(elem, &Monitor::send_status, action, &QAction::setChecked);
    }

}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::init_monitors_WIN()
{
    std::vector<PHYSICAL_MONITOR> monitors;
    get_physical_monitors_WIN(monitors);

    qDebug() << "Number of physical monitors: " << monitors.size();

    // Register monitors
    for (auto& mons : monitors)
    {
        QString name = QString::fromWCharArray(mons.szPhysicalMonitorDescription);

        Monitor* mon = new Monitor(mons, name);

        mon->monitor_init();
        registered_monitors.push_back(mon);
    }

    Monitor* mon2 = new Monitor("Monitor 2");

    mon2->monitor_init();
    registered_monitors.push_back(mon2);
}

void MainWindow::init_monitors_UNIX()
{
    qDebug() << "Not Implemented";
}

void MainWindow::add_monitor_control_widget()
{
    MonitorWidget* wMonSet = new MonitorWidget();
    wMonSet->add_slider(true);

    mainLayout->addWidget(wMonSet);

    wMonSet->add_contextMenu(trayIconMenu);

    QSystemTrayIcon* trayIcon = new QSystemTrayIcon();


    QIcon icon = createIconFromText("T", QColor(256, 256, 0), QRect(0, 0, 10, 10));

    trayIcon->setIcon(icon);
    trayIcon->setContextMenu(trayIconMenu);

    trayIcons.push_back(trayIcon);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    
    trayIcon->show();
}



void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!event->spontaneous() || !isVisible())
        return;

    bool trayIcon_visible = false;
    
    for (auto icon : trayIcons)
    {
        trayIcon_visible = trayIcon_visible || icon->isVisible();
    }

    if (trayIcon_visible) {
        QMessageBox msgBox;

        msgBox.setWindowTitle("Closing");
        msgBox.setText("The program will keep running in the "
                    "system tray when pressing <b>Close</b> (Enter/Space). To terminate the program, "
                    "choose <b>Quit</b> in the context menu "
                    "of the system tray entry or this message.");

        QPushButton* closeButton = msgBox.addButton(tr("Close"), QMessageBox::ActionRole);
        QPushButton* quitButton = msgBox.addButton(tr("Quit"), QMessageBox::ActionRole);
        QPushButton* cancelButton = msgBox.addButton(QMessageBox::Cancel);

        msgBox.setDefaultButton(closeButton);
        msgBox.setEscapeButton(cancelButton);

        msgBox.exec();

        if (msgBox.clickedButton() == closeButton) {
            hide();
            event->ignore();
        }
        else if (msgBox.clickedButton() == quitButton) {
            writeSettings();
            qApp->quit();
            
        }
        else if (msgBox.clickedButton() == cancelButton) {
            event->ignore();

        }
    }

    else
    {
        QMessageBox msgBox;

        //QFont font;
        //font.setPointSize(16);
        //msgBox.setFont(font);

        msgBox.setWindowTitle("Closing");
        msgBox.setText("No Tray Icon\nProgram will quit on exit.");

        QPushButton* quitButton = msgBox.addButton(tr("Quit"), QMessageBox::ActionRole);
        //quitButton->setFont(QFont());
        QPushButton* cancelButton = msgBox.addButton(QMessageBox::Cancel);

        msgBox.setDefaultButton(quitButton);
        msgBox.setEscapeButton(cancelButton);

        msgBox.exec();

        if (msgBox.clickedButton() == quitButton) {
            writeSettings();
            qApp->quit();

        }
        else if (msgBox.clickedButton() == cancelButton) {
            event->ignore();
        }
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(config_path, QSettings::IniFormat);

    qDebug() << "\n===========================================";
    qDebug() << "Write Settings to " << config_path;

    settings.beginGroup("MainWindow");
    settings.setValue("test", 100);
    settings.endGroup();

}

void MainWindow::readSettings()
{
    QSettings settings(config_path, QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    const auto geometry = settings.value("test").value<int>();

    if (geometry == NULL)
    {
        qDebug() << "No saved value";
    }

    else
    {
        qDebug() << "Saved value: " << geometry;
    }
    
    settings.endGroup();
}


void MainWindow::createMonitorGroupBox()
{
    monitorGroupBox = new QGroupBox(tr("Registered Monitors"));
    monitorGroupBox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    QVBoxLayout* vLayout = new QVBoxLayout;
    monitorGroupBox->setLayout(vLayout);

    MonitorTable* tableWidget = new MonitorTable(this, QStringList{ "", "Monitor" });

    vLayout->addWidget(tableWidget);

    QTableWidgetItem* newItem = new QTableWidgetItem("Test");
    newItem->setTextAlignment(Qt::AlignCenter);

    for (auto& monitor : registered_monitors)
    {
        tableWidget->add_monitor(monitor);
    }  
}


//https://stackoverflow.com/questions/11562319/cannot-get-qsystemtrayicon-to-work-correctly-with-activation-reason
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        //qDebug() << "\n-------------\nTray Trigger" << reason << "\n-------------\n";
        dialogueWidget->show();
        break;

    case QSystemTrayIcon::DoubleClick:
        //qDebug() << "\n-------------\nTray Double Click" << reason << "\n-------------\n";
        break;

    case QSystemTrayIcon::MiddleClick:
        //qDebug() << "\n-------------\nTray Middle Click" << reason << "\n-------------\n";
        break;

    case QSystemTrayIcon::Context:
        //qDebug() << "\n-------------\nTray Context Click" << reason << "\n-------------\n";
        break;

    case QSystemTrayIcon::Unknown:
        //qDebug() << "\n-------------\nTray Unknown" << reason << "\n-------------\n";
        break;

    default:
        ;
    }
}




//void MainWindow::createTrayIcon()
//{
//    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(minimizeAction);
//    trayIconMenu->addAction(maximizeAction);
//    trayIconMenu->addAction(restoreAction);
//    trayIconMenu->addSeparator();
//
//    trayMonitorMenu = trayIconMenu->addMenu("Monitor");
//    trayIconMenu->addSeparator();
//    trayIconMenu->addAction(quitAction);
//
//    for (auto elem : registered_monitors)
//    {
//        auto action = new QAction(elem->get_name(), trayMonitorMenu);
//        action->setCheckable(true);
//        action->setChecked(elem->get_status());
//
//        trayMonitorMenu->addAction(action);
//
//        connect(action, &QAction::triggered, elem, &Monitor::set_status);
//        connect(elem, &Monitor::send_status, action, &QAction::setChecked);
//    }
//
//    add_slider(QColor(0, 255, 255), "Brightness", 0x10);
//    add_slider(QColor(255, 0, 255), "Contrast", 0x12);
//    add_slider(QColor(255, 0, 0), "Volume", 0x62);
//
//}
//
//
//void MainWindow::add_slider(QColor color, QString name, uint16_t code)
//{
//    CustomSlider* cSlider = new CustomSlider(NULL, true, color, code);
//
//    cSlider->set_contextMenu(*trayIconMenu);
//    trayIcons.push_back(cSlider->get_trayIcon());
//    sliders.push_back(cSlider);
//
//    cSlider->get_trayIcon()->setToolTip(name);
//    cSlider->setToolTip(name);
//    sliderLayout->addWidget(cSlider);
//
//    connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
//
//    for (auto& monitor : registered_monitors)
//    {
//        connect(cSlider, &CustomSlider::send_monitor_signal, monitor, &Monitor::receive_signal);
//    }
//}



//Linker::getInstance().sendSignal();





//
   //createActions(); // Setup globally used actions like minimize/maximize ...
   //createMonitorGroupBox();
   //createPositionGroupBox();
   //

   //sliderLayout = new QHBoxLayout;

   //mainLayout->addWidget(monitorGroupBox);
   //mainLayout->addWidget(posGroupBox);
   //mainLayout->addWidget(monitorSettings);

   //mainWidget->setLayout(mainLayout);
   //mainLayout->addLayout(sliderLayout);
   //


   //setWindowTitle(tr("Monitor Control"));
   ////resize(400, 300);
   //int xSize = width();
   //int ySize = height();

   //qDebug() << xSize << " " << ySize;

   //move(xScreen-xSize, yScreen-2*ySize-25);

   //connect(cntr, &Controller::updated, this, &MainWindow::updatePosLabel);
   //connect(cntr, &Controller::updated, &Linker::getInstance(), &Linker::receive_mouse_update);

   //createTrayIcon();


   //qDebug() << "================================================";
   //registered_monitors[0]->get_feature(0x10);
   //qDebug() << "================================================\n\n";

   //this->setWindowFlags(Qt::Popup);















//https://stackoverflow.com/questions/9261175/how-to-emit-a-signal-from-a-qpushbutton-when-the-mouse-hovers-over-it

//bool MainWindow::eventFilter(QObject* obj, QEvent* event)
//{
//    // This function repeatedly call for those QObjects
//    // which have installed eventFilter (Step 2)
//
//    if (event->type() == QEvent::Enter)
//    {
//        // Whatever you want to do when mouse goes over targetPushButton
//        //qDebug() << "\n-------------\nhandling an event" << event << " <-- " << obj->metaObject()->className() << "\n-------------\n";
//
//        hoverLabel->setText("Yes");
//    }
//
//    else if (event->type() == QEvent::Leave)
//    {
//        //qDebug() << "\n-------------\nhandling an event" << event << " <-- " << obj->metaObject()->className() << "\n-------------\n";
//        hoverLabel->setText("No");
//    }
//
//    else {
//
//        return QWidget::eventFilter(obj, event);
//    }
//
//}




/// Gives human-readable event type information.
//QDebug operator<<(QDebug str, const QEvent* ev) {
//    static int eventEnumIndex = QEvent::staticMetaObject
//        .indexOfEnumerator("Type");
//    str << "QEvent";
//    if (ev) {
//        QString name = QEvent::staticMetaObject
//            .enumerator(eventEnumIndex).valueToKey(ev->type());
//        if (!name.isEmpty()) str << name; else str << ev->type();
//    }
//    else {
//        str << (void*)ev;
//    }
//    return str.maybeSpace();
//}