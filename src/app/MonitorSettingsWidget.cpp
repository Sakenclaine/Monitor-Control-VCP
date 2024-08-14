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
    settings_continous_layout->setSpacing(0);


    setup_discrete_settings();


    CustomFrame* settingsFrame = new CustomFrame(this);
    settingsFrame->setLayout(settings_continous_layout);
    
    connect(settingsFrame->get_button(), &QPushButton::clicked, &Linker::getInstance(), &Linker::receive_slider_add_request);

    connect(&Linker::getInstance(), &Linker::emit_add_slider, this, &MonitorWidget::receive_add_slider);

	layout->addWidget(settings_discrete);
	layout->addWidget(settingsFrame);
}


void MonitorWidget::setup_discrete_settings()
{
    settings_discrete = new ComboBoxFrame(this); //new QGroupBox(tr("Monitor Settings"));
    settings_discrete->setMinimumWidth(150);
    settings_discrete->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    discreteLayout = new QVBoxLayout();
    discreteLayout->setAlignment(Qt::AlignTop);
    settings_discrete->setLayout(discreteLayout);

    cbInput = new QComboBox();
    cbInput->setObjectName("cb_60");
    cbClrProfile = new QComboBox();
    cbClrProfile->setObjectName("cb_14");
    cbMode = new QComboBox();
    
    foreach(auto elem, Linker::getInstance().get_monitors())
    {
        settings_discrete->monitorBox->addItem(elem->get_name());
    }

    connect(settings_discrete, &ComboBoxFrame::comboBoxItemChanged, this, &MonitorWidget::cb_monitor_change);
    
    discreteLayout->setContentsMargins(5, 15, 10, 0);
    discreteLayout->addWidget(cbInput);
    discreteLayout->addWidget(cbClrProfile);
    discreteLayout->addWidget(cbMode);

    QString cName = settings_discrete->monitorBox->currentText();
    int cInd = settings_discrete->monitorBox->currentIndex();

    cb_monitor_change(cName, cInd);

}

void MonitorWidget::add_slider(uint16_t code, bool btrayIcon)
{
    CustomSlider* cSlider = new CustomSlider(NULL, code);

    QString name = n2hexstr(code);
    customSliders[name] = cSlider;

    if (btrayIcon) cSlider->add_trayIcon();

    settings_continous_layout->addWidget(cSlider);

    if (btrayIcon)
    {
        connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, &Linker::getInstance(), &Linker::receive_icon_click);
    }
}

void MonitorWidget::add_slider(uint16_t code, QColor color, bool btrayIcon)
{
    CustomSlider* cSlider = new CustomSlider(NULL, color, code);

    QString name = n2hexstr(code);
    customSliders[name] = cSlider;

    if (btrayIcon) cSlider->add_trayIcon();

    settings_continous_layout->addWidget(cSlider);

    if (btrayIcon)
    {
        connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, &Linker::getInstance(), &Linker::receive_icon_click);
    }
    //connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, parent, &MainWindow::iconActivated);
}

void MonitorWidget::add_contextMenu(QMenu* menu)
{
    foreach(CustomSlider * elem, customSliders)
        elem->get_trayIcon()->setContextMenu(menu);

}

void MonitorWidget::receive_add_slider(uint16_t& cde, QColor& col, bool& trayCheck)
{
    qDebug() << "Adding Slider ... " << cde << " " << col;
    this->add_slider(cde, col, trayCheck);
}


void MonitorWidget::cb_monitor_change(QString& name, int& id)
{
    qDebug() << name << " -- " << id;
    qDebug() << "Update ComboBoxes ...";

    cbInput->clear();

    chk_add_discrete_feature(name, "60");
    chk_add_discrete_feature(name, "14");


}

void MonitorWidget::chk_add_discrete_feature(QString monName, QString qsft)
{
    Monitor* mon = Linker::getInstance().get_monitor_byName(monName);

    if (mon->features.contains(qsft))
    {
        auto tempVec = VCP_FEATURES.commands[qsft].possible_values;
        auto nm = VCP_FEATURES.commands[qsft].name;

        for (auto& ft : mon->features[qsft].possible_values)
        {
            auto it = std::find(tempVec.begin(), tempVec.end(), ft);

            if (it != tempVec.end())
            {
                int index = it - tempVec.begin();

                QString cbName = QString("cb_%1").arg(qsft);
                QComboBox* cb = this->findChild<QComboBox*>(cbName);
                
                if (cb != NULL) {
                    cb->addItem(VCP_FEATURES.commands[qsft].possible_values_desc[index]);
                }
            }
        }
    }

}

PlaceholderWidget::PlaceholderWidget()
{
    int xS, yS, a, b, c, d;
    get_screen_geometry(xS, yS, a, b, c, d);

    
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