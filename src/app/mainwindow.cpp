#include <QAction>
#include <QDebug> 
#include <QEvent>
#include <QCloseEvent>
#include <QMetaEnum>

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

#include <QMenu>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>


#include "utilities.h"
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



MainWindow::MainWindow(QWidget* parent) : 
    QMainWindow(parent)
{
    readSettings();

    #ifdef Q_OS_WIN
    init_monitors_WIN();

    int a, b, c, d;
    get_screen_geometry(a, b, screenSizeX, screenSizeY, c, d);

    #elif Q_OS_UNIX
    init_monitors_UNIX();

    #endif

    foreach(auto elem, Linker::getInstance().get_monitors())
    {
        int id = elem->get_ID();
        QMap<QString, QVariant> temp;
        QVariant chkd(false);

        temp["checked"] = chkd;
        Linker::getInstance().monitor_global_status[id] = temp;
    }

    Controller::getInstance().operate();

    connect(&Controller::getInstance(), &Controller::updated, &Linker::getInstance(), &Linker::receive_mouse_update);
    

    setup();
 
    
    connect(&Linker::getInstance(), &Linker::emit_icon_click, this, &MainWindow::iconActivated);
    connect(this, &MainWindow::emit_add_slider, &Linker::getInstance(), &Linker::receive_add_slider);
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

    createActions(); // Create all actions defined in the class with forward declarations
    createMonitorGroupBox(); // Create table listing the monitors
    create_monitor_control_widget();

    wSettings = new SettingsWidget();

    // Create context menu for tray icons
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();

    trayMonitorMenu = trayIconMenu->addMenu("Monitor");
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    for (auto elem : Linker::getInstance().get_monitors())
    {
        auto action = new QAction(elem->get_name(), trayMonitorMenu);
        action->setCheckable(true);
        action->setChecked(elem->get_status());

        trayMonitorMenu->addAction(action);

        connect(action, &QAction::triggered, elem, &Monitor::set_status);
        connect(elem, &Monitor::send_status, action, &QAction::setChecked);
    }

    mainLayout->addWidget(monitorGroupBox);
    mainLayout->addWidget(wMonSet);
    wMonSet->add_contextMenu(trayIconMenu);
}

void MainWindow::createMonitorGroupBox()
{
    monitorGroupBox = new QGroupBox(tr("Registered Monitors"));
    monitorGroupBox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    QVBoxLayout* vLayout = new QVBoxLayout;
    monitorGroupBox->setLayout(vLayout);

    MonitorTable* tableWidget = new MonitorTable(this, QStringList{ "", "Monitor" });

    vLayout->addWidget(tableWidget);

    for (auto& monitor : Linker::getInstance().get_monitors())
    {
        tableWidget->add_monitor(monitor);
    }
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &MainWindow::restore);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::init_monitors_WIN()
{
    #ifndef QT_DEBUG 
    //Register monitors
    std::vector<PHYSICAL_MONITOR> monitors;

    qDebug() << "Number of physical monitors: " << monitors.size();

    for (auto& mons : monitors)
    {
        QString name = QString::fromWCharArray(mons.szPhysicalMonitorDescription);

        Monitor* mon = new Monitor(mons, name);

        mon->monitor_init();
        //registered_monitors.push_back(mon);
        Linker::getInstance().register_monitor(mon);
    }
    #endif
    
    #ifdef QT_DEBUG
    Monitor* mon1 = new Monitor("Monitor 1");
    mon1->monitor_init();
    Linker::getInstance().register_monitor(mon1);

    Monitor* mon2 = new Monitor("Monitor 2");
    mon2->monitor_init();
    Linker::getInstance().register_monitor(mon2);

    #endif

    for (auto& elem : Linker::getInstance().get_monitors())
    {
        connect(&Linker::getInstance(), &Linker::emit_monitor_value_update, elem, &Monitor::receive_signal);
    }
}

void MainWindow::init_monitors_UNIX()
{
    qDebug() << "Not Implemented";
}

void MainWindow::create_monitor_control_widget()
{
    wMonSet = new MonitorWidget();
    wMonSet->add_slider(0x10, true);
    wMonSet->add_slider(0x12, QColor(255, 0, 255), true);
    //wMonSet->add_slider(0x62, QColor(50, 240, 250), true);

    const auto clrs = SettingsManager::getInstance().readSetting("Sliders", "colors");
    const auto cdes = SettingsManager::getInstance().readSetting("Sliders", "codes");
    const auto ids = SettingsManager::getInstance().readSetting("Sliders", "ids");
    const auto trayChks = SettingsManager::getInstance().readSetting("Sliders", "tray");

    if (clrs != NULL && cdes != NULL && ids != NULL && trayChks != NULL)
    {
        if (cdes.toList().size() > 2)
        {
            for (int i = 2; i < clrs.toList().size(); i++)
            {
                qDebug() << "Load slider: " << cdes.toList()[i];
                
                uint16_t code = cdes.toList()[i].toUInt();
                QColor color = clrs.toList()[i].value<QColor>();
                bool tray = trayChks.toList()[i].toBool();

                wMonSet->add_slider(code, color, tray);
            }
        }
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
    QList<QVariant> cdes;
    QList<QVariant> clrs;
    QList<QVariant> ids;
    QList<QVariant> bTrayChk;

    for (auto elem : Linker::getInstance().get_sliders())
    {
        cdes.append(QVariant(elem->get_code()));
        clrs.append(QVariant(elem->get_color()));
        ids.append(QVariant(elem->get_ID()));
        bTrayChk.append(QVariant(elem->get_trayCheck()));
    }

    SettingsManager::getInstance().writeSettingInGroup("Sliders", "codes", cdes);
    SettingsManager::getInstance().writeSettingInGroup("Sliders", "colors", clrs);
    SettingsManager::getInstance().writeSettingInGroup("Sliders", "ids", ids);
    SettingsManager::getInstance().writeSettingInGroup("Sliders", "tray", bTrayChk);

}

void MainWindow::readSettings()
{

    
}



// Slots
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

                for (auto& mon : Linker::getInstance().get_monitors())
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

                        for (auto& mon : Linker::getInstance().get_monitors())
                        {
                            monitor_vcp monVCP;
                        }

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

//https://stackoverflow.com/questions/11562319/cannot-get-qsystemtrayicon-to-work-correctly-with-activation-reason
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        //qDebug() << "\n-------------\nTray Trigger" << reason << "\n-------------\n";
        this->setWindowFlags(flags | Qt::Popup);

        //this->move(screenSizeX - width(), screenSizeY - minimumHeight());
        show();

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

void MainWindow::restore()
{
    this->setWindowFlags(flags | Qt::Window);
    show();
}


