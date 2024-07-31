#include "MonitorSettingsWidget.h"
#include "helpers.h"

#include <QGroupBox>
#include <QHBoxLayout>

#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QScrollArea>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QFrame>
#include <QLabel>




MonitorWidget::MonitorWidget()
{
	QHBoxLayout* layout = new QHBoxLayout(this);
    QHBoxLayout* settings_continous_layout = new QHBoxLayout();

    hSliderLayout = new QHBoxLayout();
    hSliderLayout->setSpacing(0);

	settings_discrete = new QGroupBox();
	settings_continous = new QGroupBox("Controls");

    CustomFrame* settingsFrame = new CustomFrame();
    settingsFrame->setLayout(hSliderLayout);


    //settings_continous->setLayout(settings_continous_layout);
    //settings->setLayout(settings_continous_layout);

    //scrollArea = new QScrollArea();
    //scrollArea->setLayout(hSliderLayout);

    //scrollArea->setBackgroundRole(QPalette::Dark);
    //scrollArea->setWidgetResizable(true);
    //scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //scrollArea->verticalScrollBar()->setEnabled(false);
   

    //settings_continous_layout->addWidget(scrollArea);

	layout->addWidget(settings_discrete);
	layout->addWidget(settingsFrame);
}


void MonitorWidget::add_slider(uint16_t code, bool btrayIcon)
{
    CustomSlider* cSlider = new CustomSlider(NULL, code);

    QString name = QString::fromStdString(n2hexstr(code));
    customSliders[name] = cSlider;

    if (btrayIcon) cSlider->add_trayIcon();

    hSliderLayout->addWidget(cSlider);


    //scrollArea->setMinimumWidth(3 * cSlider->sizeHint().width() + 10);
    //scrollArea->setMinimumHeight(cSlider->sizeHint().height() + 5);

    //connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, parent, &MainWindow::iconActivated);
}

void MonitorWidget::add_slider(uint16_t code, QColor color, bool btrayIcon)
{
    CustomSlider* cSlider = new CustomSlider(NULL, color, code);

    QString name = QString::fromStdString(n2hexstr(code));
    customSliders[name] = cSlider;

    if (btrayIcon) cSlider->add_trayIcon();

    hSliderLayout->addWidget(cSlider);

    //scrollArea->setMinimumWidth(3 * cSlider->sizeHint().width() + 10);
    //scrollArea->setMinimumHeight(cSlider->sizeHint().height() + 5);
    
    //connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, parent, &MainWindow::iconActivated);
}

void MonitorWidget::add_contextMenu(QMenu* menu)
{
    foreach(CustomSlider * elem, customSliders)
        elem->get_trayIcon()->setContextMenu(menu);

}





PlaceholderWidget::PlaceholderWidget()
{
    int xS, yS;
    get_screen_geometry(xS, yS);

    
    QHBoxLayout* tLayout = new QHBoxLayout();

    QImage image(":/images/placeholder.png");

    QLabel* imageLabel = new QLabel();
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);

    QPushButton* button = new QPushButton;
    button->setIcon(QIcon(":/images/monitorcontrol.ico"));
    button->setIconSize(QSize(65, 65));

    tLayout->addWidget(scrollArea);
    tLayout->addWidget(button);

    setLayout(tLayout);
    setWindowFlags(Qt::Popup);

    imageLabel->setPixmap(QPixmap::fromImage(image));
    imageLabel->resize(0.5 * imageLabel->pixmap(Qt::ReturnByValue).size());
    scrollArea->setVisible(true);

    move(xS - this->sizeHint().width() - 40, yS - this->sizeHint().height());
}