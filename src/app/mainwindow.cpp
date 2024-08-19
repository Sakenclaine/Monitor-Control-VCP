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

    //SliderWidget* test = new SliderWidget(this, 0x12);
    //test->add_trayIcon();
    //test->set_contextMenu(trayIconMenu);

    mainLayout->addWidget(monitorSettings);


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
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}


