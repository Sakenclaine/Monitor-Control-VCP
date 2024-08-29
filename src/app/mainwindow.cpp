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


#include "mainwindow.h"
#include "MonitorHandler.h"
#include "winController.h"
#include "SignalLinker.h"
#include "CustomSlider.h"

#include "helpers.h"
#include "utilities.h"
#include "MonitorTable.h"
#include "MonitorSettingsWidget.h"

#include "TrayIconControlled.h"



MainWindow::MainWindow(QWidget* parent) : 
    QMainWindow(parent)
{
    setup();

}

MainWindow::~MainWindow()
{
    Controller::getInstance().stop_worker();
};

void MainWindow::setup()
{
    QList<Monitor*> monitors;
    bool bChk = get_connected_monitors(monitors);

    if (bChk) { 
        Linker::getInstance().register_monitor(monitors); 
    }

    qDebug() << QString("Found (%1) connected monitors").arg(monitors.size());

    // Create main widget and layout set as content for the main window
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainLayout = new QVBoxLayout;
    mainWidget->setLayout(mainLayout);
    
    createActions();
    createMonitorGroupBox();
    createTrayMenu();


    monitorSettings = new MonitorWidget(this);
    monitorSettings->add_contextMenu(trayIconMenu);
    
    mainLayout->addWidget(monitorSettings);

    connect(&Linker::getInstance(), &Linker::emit_icon_click, this, &MainWindow::iconActivated);
}

void MainWindow::createMonitorGroupBox()
{
    monitorGroupBox = new QGroupBox(tr("Registered Monitors"));
    monitorGroupBox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    QVBoxLayout* vLayout = new QVBoxLayout;
    monitorGroupBox->setLayout(vLayout);

    tableWidget = new MonitorTable(this, QStringList{ "", "Monitor" });

    vLayout->addWidget(tableWidget);

    for (auto& monitor : Linker::getInstance().get_monitors())
    {
        tableWidget->add_monitor(monitor);
    }


    tableWidget->selectRow(0);
    mainLayout->addWidget(monitorGroupBox);
}

void MainWindow::createTrayMenu()
{
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
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &MainWindow::restore); //&QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!event->spontaneous() || !isVisible())
        return;

    bool trayIcon_visible = false;

    for (auto sldr : Linker::getInstance().get_sliders())
    {
        trayIcon_visible = trayIcon_visible || sldr->get_icon()->isVisible();
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

    qDebug() << "\n\nSaving Settings ...";

    for (auto& elem : Linker::getInstance().get_sliders())
    {
        qDebug() << "Slider " << elem->get_code() << " -- " << elem->get_color() << " -- " << elem->get_ID() << " -- " << elem->get_trayCheck();
        
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