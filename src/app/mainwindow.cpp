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



void get_screen_geometry(int & xWO_taskbar, int& yWO_taskbar)
{
    // size of screen (primary monitor) without taskbar or desktop toolbars
    xWO_taskbar = GetSystemMetrics(SM_CXFULLSCREEN);
    yWO_taskbar = GetSystemMetrics(SM_CYFULLSCREEN);

    //qDebug() << GetSystemMetrics(SM_CXFULLSCREEN) << " x " << GetSystemMetrics(SM_CYFULLSCREEN) << "\n";

    // size of screen (primary monitor) without just the taskbar
    RECT xy;
    BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &xy, 0);
    //qDebug() << xy.right - xy.left << " x " << xy.bottom - xy.top << "\n";

    // the full width and height of the screen (primary monitor)
    //qDebug() << GetDeviceCaps(GetDC(NULL), HORZRES) << " x " << GetDeviceCaps(GetDC(NULL), VERTRES) << "\n";

}



MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    readSettings();
    
    //setAppToStartAutomatically(false);

    int xScreen, yScreen;
    get_screen_geometry(xScreen, yScreen);
    

    Linker::getInstance().sendSignal();

    Controller* cntr = new Controller;
    cntr->operate();

    std::vector<PHYSICAL_MONITOR> monitors;
    get_physical_monitors(monitors);

    qDebug() << "Number of physical monitors: " << monitors.size();
    
    // Register monitors
    for (auto& mons : monitors)
    {
        QString name = QString::fromWCharArray(mons.szPhysicalMonitorDescription);

        Monitor* mon = new Monitor(mons, name, true);

        mon->monitor_init();

        registered_monitors.push_back(mon);
        //chosen_monitors.push_back(false);
    }

    {
        Monitor* mon2 = new Monitor("Monitor 2", true);

        mon2->monitor_init();

        registered_monitors.push_back(mon2);
        //chosen_monitors.push_back(false);
    }

    
    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    
    createActions(); // Setup globally used actions like minimize/maximize ...
    createMonitorGroupBox();
    createPositionGroupBox();
    
    mainLayout = new QVBoxLayout;
    sliderLayout = new QHBoxLayout;

    mainLayout->addWidget(monitorGroupBox);
    mainLayout->addWidget(posGroupBox);

    mainWidget->setLayout(mainLayout);
    mainLayout->addLayout(sliderLayout);
    


    setWindowTitle(tr("Monitor Control"));
    //resize(400, 300);
    int xSize = width();
    int ySize = height();

    qDebug() << xSize << " " << ySize;

    move(xScreen-xSize, yScreen-2*ySize-25);

    connect(cntr, &Controller::updated, this, &MainWindow::updatePosLabel);
    connect(cntr, &Controller::updated, &Linker::getInstance(), &Linker::receive_mouse_update);

    createTrayIcon();


    //qDebug() << "================================================";
    //registered_monitors[0]->get_feature(0x10);
    //qDebug() << "================================================\n\n";

    //this->setWindowFlags(Qt::Popup);

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
            event->accept();
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
            event->accept();
            qApp->quit();

        }
        else if (msgBox.clickedButton() == cancelButton) {
            event->ignore();
        }
    }
}

void MainWindow::writeSettings()
{
    QString path = qApp->applicationDirPath();
    QSettings settings(path+"/conf.ini", QSettings::IniFormat);

    qDebug() << "\n===========================================";
    qDebug() << "Write Settings to " << path + "/conf.ini";

    settings.beginGroup("MainWindow");
    settings.setValue("test", 100);
    settings.endGroup();

}

void MainWindow::readSettings()
{
    QString path = qApp->applicationDirPath();
    QSettings settings(path + "/conf.ini", QSettings::IniFormat);

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


void MainWindow::updatePosLabel(const struct inSignal& input)
{
    //qDebug() << tray_rect.contains(input.mouse.xCoo, input.mouse.yCoo);
    
    QString positions = QString("x: %1, y: %2").arg(input.mouse.xCoo).arg(input.mouse.yCoo);
    
    mousePosX->setText(positions);


}


void MainWindow::createMonitorGroupBox()
{
    monitorGroupBox = new QGroupBox(tr("Registered Monitors"));
    monitorGroupBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QStringList tableHeaders = { "", "Monitor", "Brightness", "Contrast", "R", "G", "B"};
    QTableWidget* tableWidget = new QTableWidget(registered_monitors.size(), 7, this);

    tableWidget->setHorizontalHeaderLabels(tableHeaders);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setVisible(false);

    for (int row = 0; row < registered_monitors.size(); row++)
    {
        QWidget* checkBoxWidget = new QWidget(); //create QWidget
        QCheckBox* checkBox = new QCheckBox();   //create QCheckBox
        QHBoxLayout* layoutCheckBox = new QHBoxLayout(checkBoxWidget); //create QHBoxLayout 
        layoutCheckBox->addWidget(checkBox);     //add QCheckBox to layout
        layoutCheckBox->setAlignment(Qt::AlignCenter); //set Alignment layout
        layoutCheckBox->setContentsMargins(0, 0, 0, 0);
        
        checkBox->setChecked(registered_monitors[row]->get_enabled());
        tableWidget->setCellWidget(row, 0, checkBoxWidget);

        connect(checkBox, &QCheckBox::checkStateChanged, registered_monitors[row], &Monitor::set_enabled);
        connect(registered_monitors[row], &Monitor::send_status, checkBox, &QCheckBox::setChecked);
        
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

    trayMonitorMenu = trayIconMenu->addMenu("Monitor");
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    for (auto elem : registered_monitors)
    {
        auto action = new QAction(elem->get_name(), trayMonitorMenu);
        action->setCheckable(true);
        action->setChecked(elem->get_enabled());

        trayMonitorMenu->addAction(action);

        connect(action, &QAction::triggered, elem, &Monitor::set_enabled);
        connect(elem, &Monitor::send_status, action, &QAction::setChecked);
    }

    add_slider(QColor(0, 255, 255), "Brightness", 0x10);
    add_slider(QColor(255, 0, 255), "Contrast", 0x12);
    add_slider(QColor(255, 0, 0), "Volume", 0x62);

}

void MainWindow::add_slider(QColor color, QString name, uint16_t code)
{
    CustomSlider* cSlider = new CustomSlider(NULL, true, color, code);

    cSlider->set_contextMenu(*trayIconMenu);
    trayIcons.push_back(cSlider->get_trayIcon());
    sliders.push_back(cSlider);

    cSlider->get_trayIcon()->setToolTip(name);
    cSlider->setToolTip(name);
    sliderLayout->addWidget(cSlider);

    connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    for (auto& monitor : registered_monitors)
    {
        connect(cSlider, &CustomSlider::send_monitor_signal, monitor, &Monitor::receive_signal);
    }
}


//https://stackoverflow.com/questions/11562319/cannot-get-qsystemtrayicon-to-work-correctly-with-activation-reason
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        //qDebug() << "\n-------------\nTray Trigger" << reason << "\n-------------\n";
        restoreAction->trigger();
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