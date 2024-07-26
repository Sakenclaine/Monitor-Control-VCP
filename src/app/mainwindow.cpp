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

#include "SignalLinker.h"



MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    autoStart = false;

    QSettings bootUpSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString app_path = QCoreApplication::applicationFilePath();

    qDebug() << app_path;

    if (autoStart == true) {
        bootUpSettings.setValue("MonitorControl", app_path);
    }
    else {
        bootUpSettings.remove("MonitorControl");
    }
    
    // size of screen (primary monitor) without taskbar or desktop toolbars
    qDebug() << GetSystemMetrics(SM_CXFULLSCREEN) << " x " << GetSystemMetrics(SM_CYFULLSCREEN) << "\n";

    // size of screen (primary monitor) without just the taskbar
    RECT xy;
    BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &xy, 0);
    qDebug() << xy.right - xy.left << " x " << xy.bottom - xy.top << "\n";

    // the full width and height of the screen (primary monitor)
    qDebug() << GetDeviceCaps(GetDC(NULL), HORZRES) << " x " << GetDeviceCaps(GetDC(NULL), VERTRES) << "\n";

   
    Linker::getInstance().sendSignal();

    std::vector<PHYSICAL_MONITOR> monitors;
    get_physical_monitors(monitors);

    qDebug() << "Number of monitors: " << monitors.size();
    
    // Register monitors
    for (auto& mons : monitors)
    {
        QString name = QString::fromWCharArray(mons.szPhysicalMonitorDescription);

        Monitor* mon = new Monitor(&mons, name, true);

        mon->monitor_init();

        registered_monitors.push_back(mon);
        chosen_monitors.push_back(false);
    }

    
    QWidget* mainWidget = new QWidget(this);
   
    setCentralWidget(mainWidget);
    
    createMonitorGroupBox();
    createPositionGroupBox();

    createActions();


    mainLayout = new QVBoxLayout;
    sliderLayout = new QHBoxLayout;

    mainLayout->addWidget(monitorGroupBox);
    mainLayout->addWidget(posGroupBox);

    mainWidget->setLayout(mainLayout);

    mainLayout->addLayout(sliderLayout);
    

    monitorComboBox->setCurrentIndex(1);
    

    setWindowTitle(tr("Monitor Control"));
    resize(400, 300);


    Controller* cntr = new Controller;
    cntr->operate();

    connect(cntr, &Controller::updated, this, &MainWindow::updatePosLabel);
    connect(cntr, &Controller::updated, &Linker::getInstance(), &Linker::receive_mouse_update);

    createTrayIcon();

    this->setWindowIcon(QIcon(":/icons/monitor.ico"));



}


MainWindow::~MainWindow()
{
};


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
                    "system tray when pressing <b>Close</b>. To terminate the program, "
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
            qApp->quit();
            
        }
        else if (msgBox.clickedButton() == cancelButton) {
            event->ignore();

        }

    }
}


void MainWindow::updatePosLabel(const struct inSignal& input)
{
    //qDebug() << tray_rect.contains(input.mouse.xCoo, input.mouse.yCoo);
    
    QString positions = QString("x: %1, y: %2").arg(input.mouse.xCoo).arg(input.mouse.yCoo);
    
    mousePosX->setText(positions);


}


void MainWindow::createMonitorGroupBox()
{
    monitorGroupBox = new QGroupBox(tr("Monitor"));
    monitorGroupBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    monitorLabel = new QLabel("Icon:");
    hoverLabel = new QLabel(tr("No"));

    hoverLabel->setToolTip("Label Tooltip");
    //hoverLabel->installEventFilter(this);


    monitorComboBox = new QComboBox;
    monitorComboBox->addItem(QIcon("./images/world.ico"), tr("World"));
    monitorComboBox->addItem(QIcon("./images/star.ico"), tr("Star"));
    monitorComboBox->addItem(QIcon("./images/house.ico"), tr("House"));

    QStringList tableHeaders = { "", "Monitor", "Brightness", "Contrast", "R", "G", "B"};
    QTableWidget* tableWidget = new QTableWidget(registered_monitors.size(), 7, this);

    tableWidget->setHorizontalHeaderLabels(tableHeaders);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setVisible(false);

    for (int row = 0; row < registered_monitors.size(); row++)
    {
        for (int column = 1; column < 7; column++)
        {
            QString cell_content;
            
            if (column == 1)
            {
                cell_content = registered_monitors[row]->get_name();
            }

            else if (column == 2)
            {
                cell_content = QString::number(registered_monitors[row]->get_brightness());
            }

            else if (column == 3)
            {
                cell_content = QString::number(registered_monitors[row]->get_contrast());
            }

            else if (column == 4)
            {
                cell_content = QString::number(registered_monitors[row]->get_R());
            }

            else if (column == 5)
            {
                cell_content = QString::number(registered_monitors[row]->get_G());
            }

            else if (column == 6)
            {
                cell_content = QString::number(registered_monitors[row]->get_B());
            }

            
            QTableWidgetItem* newItem = new QTableWidgetItem(cell_content);
            newItem->setTextAlignment(Qt::AlignCenter);

            tableWidget->setItem(row, column, newItem);
        }
    }

    //tableWidget->resizeColumnsToContents();

    int widgetWidth = tableWidget->viewport()->size().width();
    int tableWidth = 0;

    for (int i = 0; i < tableWidget->columnCount(); ++i)
        tableWidth += tableWidget->horizontalHeader()->sectionSize(i); //sections already resized to fit all data

    double scale = (double)widgetWidth / tableWidth;

    qDebug() << "Scale: " << scale;

    for (int i = 0; i < tableWidget->columnCount(); ++i)
        tableWidget->setColumnWidth(i, (tableWidget->horizontalHeader()->sectionSize(i)) * scale);

    QVBoxLayout* vLayout = new QVBoxLayout;

    QHBoxLayout* iconLayout = new QHBoxLayout;
    iconLayout->addWidget(monitorLabel);
    iconLayout->addWidget(hoverLabel);
    iconLayout->addWidget(monitorComboBox);
    iconLayout->addStretch();



    //vLayout->addLayout(iconLayout);
    vLayout->addWidget(tableWidget);

    monitorGroupBox->setLayout(vLayout);
}


void MainWindow::createPositionGroupBox()
{
    posGroupBox = new QGroupBox(tr("Positions"));

    trayPos = new QLabel();
    mousePosX = new QLabel();
    mousePosY = new QLabel();


    QGridLayout* posLayout = new QGridLayout;
    posLayout->addWidget(trayPos, 0, 0);
    posLayout->addWidget(mousePosX, 1, 0);
    posLayout->addWidget(mousePosY, 1, 1);


    posGroupBox->setLayout(posLayout);
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


void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();

    QMenu* monitorMenu = trayIconMenu->addMenu("Monitor");
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    
    for (auto elem : registered_monitors)
    {
        auto action = new QAction(elem->get_name(), monitorMenu);
        action->setCheckable(true);
        action->setChecked(elem->get_enabled());

        monitorMenu->addAction(action);

        connect(action, &QAction::triggered, elem, &Monitor::set_enabled);
    }

    CustomSlider* cSlider = new CustomSlider(this, true, QColor(0, 255, 255));
    CustomSlider* cSlider2 = new CustomSlider(this, true, QColor(255, 0, 255));
    CustomSlider* cSlider3 = new CustomSlider(this, true, QColor(255, 0, 0));

    sliderLayout->addWidget(cSlider);
    sliderLayout->addWidget(cSlider2);
    sliderLayout->addWidget(cSlider3);

    TrayIconControlled* test = cSlider->get_trayIcon();

    cSlider->set_contextMenu(*trayIconMenu);
    cSlider2->set_contextMenu(*trayIconMenu);
    cSlider3->set_contextMenu(*trayIconMenu);

    trayIcons.push_back(cSlider->get_trayIcon());
    trayIcons.push_back(cSlider2->get_trayIcon());
    trayIcons.push_back(cSlider3->get_trayIcon());
    
    cSlider->get_trayIcon()->setToolTip("Adjust brightness");
    cSlider2->get_trayIcon()->setToolTip("Adjust contrast");
    cSlider3->get_trayIcon()->setToolTip("Adjust volume");

    connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    connect(cSlider2->get_trayIcon(), &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    connect(cSlider3->get_trayIcon(), &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);


//https://stackoverflow.com/questions/11562319/cannot-get-qsystemtrayicon-to-work-correctly-with-activation-reason

}


void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        qDebug() << "\n-------------\nTray Trigger" << reason << "\n-------------\n";
        restoreAction->trigger();
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "\n-------------\nTray Double Click" << reason << "\n-------------\n";
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "\n-------------\nTray Middle Click" << reason << "\n-------------\n";
        break;
    case QSystemTrayIcon::Context:
        qDebug() << "\n-------------\nTray Context Click" << reason << "\n-------------\n";
        break;
    case QSystemTrayIcon::Unknown:
        qDebug() << "\n-------------\nTray Unknown" << reason << "\n-------------\n";
        break;
    default:
        ;
    }
}





















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