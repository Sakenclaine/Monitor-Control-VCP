#include "MonitorSettingsWidget.h"
#include "SignalLinker.h"
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
    settings_continous_layout = new QHBoxLayout();

    hSliderLayout = new QHBoxLayout();
    hSliderLayout->setSpacing(0);

	settings_discrete = new QGroupBox();
	settings_continous = new QGroupBox("Controls");

    CustomFrame* settingsFrame = new CustomFrame(this);
    settingsFrame->setLayout(settings_continous_layout);
    
    connect(settingsFrame->get_button(), &QPushButton::clicked, &Linker::getInstance(), &Linker::receive_slider_add_request);

	layout->addWidget(settings_discrete);
	layout->addWidget(settingsFrame);
}


void MonitorWidget::add_slider(uint16_t code, bool btrayIcon)
{
    CustomSlider* cSlider = new CustomSlider(NULL, code);

    QString name = n2hexstr(code);
    customSliders[name] = cSlider;

    if (btrayIcon) cSlider->add_trayIcon();

    settings_continous_layout->addWidget(cSlider);

    connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, &Linker::getInstance(), &Linker::receive_icon_click);
}

void MonitorWidget::add_slider(uint16_t code, QColor color, bool btrayIcon)
{
    CustomSlider* cSlider = new CustomSlider(NULL, color, code);

    QString name = n2hexstr(code);
    customSliders[name] = cSlider;

    if (btrayIcon) cSlider->add_trayIcon();

    settings_continous_layout->addWidget(cSlider);

    connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, &Linker::getInstance(), &Linker::receive_icon_click);
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