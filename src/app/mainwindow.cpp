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
#include "errors.h"
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

    Controller::getInstance().operate();

    connect(&Controller::getInstance(), &Controller::updated, &Linker::getInstance(), &Linker::receive_mouse_update);
    
    setup();


    add_monitor_control_widget();

    connect(&Linker::getInstance(), &Linker::emit_slider_add_request, this, &MainWindow::add_slider);

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

    dAddSlider = new Dialog_AddSlider();

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

    connect(&Linker::getInstance(), &Linker::emit_icon_click, this, &MainWindow::iconActivated);
    connect(this, &MainWindow::emit_add_slider, &Linker::getInstance(), &Linker::receive_add_slider);

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
    //get_physical_monitors_WIN(monitors);

    qDebug() << "Number of physical monitors: " << monitors.size();

    #ifndef QT_DEBUG 
    //Register monitors
    for (auto& mons : monitors)
    {
        QString name = QString::fromWCharArray(mons.szPhysicalMonitorDescription);

        Monitor* mon = new Monitor(mons, name);

        mon->monitor_init();
        registered_monitors.push_back(mon);
    }
    #endif
    
    #ifdef QT_DEBUG
    Monitor* mon1 = new Monitor("Monitor 1");
    mon1->monitor_init();
    registered_monitors.push_back(mon1);

    Linker::getInstance().register_monitor(mon1);

    Monitor* mon2 = new Monitor("Monitor 2");
    mon2->monitor_init();
    registered_monitors.push_back(mon2);

    Linker::getInstance().register_monitor(mon2);

    Monitor* mon3 = new Monitor("Monitor 3 XYZABC");
    mon3->monitor_init();
    registered_monitors.push_back(mon3);

    Linker::getInstance().register_monitor(mon3);
    #endif


    for (auto& elem : registered_monitors)
    {
        connect(&Linker::getInstance(), &Linker::emit_monitor_value_update, elem, &Monitor::receive_signal);
    }

}

void MainWindow::init_monitors_UNIX()
{
    qDebug() << "Not Implemented";
}

void MainWindow::add_monitor_control_widget()
{
    MonitorWidget* wMonSet = new MonitorWidget();
    wMonSet->add_slider(0x10, true);
    wMonSet->add_slider(0x12, QColor(255, 0, 255), true);
    wMonSet->add_slider(0x62, QColor(50, 240, 250), true);

    mainLayout->addWidget(wMonSet);

    wMonSet->add_contextMenu(trayIconMenu);

    //QSystemTrayIcon* trayIcon = new QSystemTrayIcon();
    //QIcon icon = createIconFromText("T", QColor(256, 256, 0), QRect(0, 0, 10, 10));

    //trayIcon->setIcon(icon);
    //trayIcon->setContextMenu(trayIconMenu);

    //trayIcons.push_back(trayIcon);

    //connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    //
    //trayIcon->show();
}


void MainWindow::add_slider()
{
    bool ok;
    QList<QVariant> list = Dialog_AddSlider::get_input(this, &ok);
    if (ok) {
        qDebug() << "Results:";
        qDebug() << "Color: " << list[1].value<QColor>();
        qDebug() << "Value: " << list[0].toInt();

        uint16_t cde = list[0].toInt();
        QString cde_str = n2hexstr(cde, 2);
        QColor col = list[1].value<QColor>();
        bool trayCheck = list[2].toBool();

        if (VCP_FEATURES.commands.find(cde_str) != VCP_FEATURES.commands.end())
        {
            qDebug() << "Code (" << cde_str << ") Exists--> " << VCP_FEATURES.commands[cde_str].name;

            if (VCP_FEATURES.commands[cde_str].continous == true)
            {
                bool code_supported = true;

                for (auto& mon : registered_monitors)
                {
                    if (!(mon->features.find(cde_str) != mon->features.end())) code_supported = false;
                }

                if (!code_supported)
                {
                    FeatureWarning dlg(cde);
                    int ret = dlg.exec();

                    switch (ret) {
                    case QMessageBox::Yes:
                        qDebug() << "Continue";
                        emit emit_add_slider(cde, col, trayCheck);

                        break;
                    case QMessageBox::Abort:
                        qDebug() << "Abort";
                        break;
                    default:
                        // should never be reached
                        break;
                    }

                }
            }
        }

        else qDebug() << "Code (" << cde_str << ") Does Not Exist ";
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!event->spontaneous() || !isVisible())
        return;

    bool trayIcon_visible = false;
    
    for (auto icon : Linker::getInstance().get_icons())
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




