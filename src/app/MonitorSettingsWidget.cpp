#include "MonitorSettingsWidget.h"
#include "MonitorHandler.h"
#include "CustomFrames.h"
#include "SignalLinker.h"
#include "helpers.h"
#include "CustomSlider.h"
#include "TrayIconControlled.h"
#include "Dialogs.h"


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QComboBox>

#include <QList>
#include <QVariant>
#include <QString>


MonitorWidget::MonitorWidget(QWidget* parent) :
	QWidget(parent)
{
	layout = new QHBoxLayout(this);
    subVLayout = new QVBoxLayout();
    subHLayout = new QHBoxLayout();

    // Discrete settings setup
    settings_discrete = new ComboBoxFrame(this);
    settings_discrete->setMinimumWidth(150);
    settings_discrete->setMaximumWidth(250);
    settings_discrete->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    discreteLayout = new QVBoxLayout();
    discreteLayout->setAlignment(Qt::AlignTop);
    settings_discrete->setLayout(discreteLayout);

    stackedWidget = new QStackedWidget;
    discreteLayout->addWidget(stackedWidget);

    // Contonious settings setup
    settings_continous = new ScrollFrame(this);
    connect(settings_continous, &ScrollFrame::add_clicked, this, &MonitorWidget::add_slider);

    continousLayout = new QHBoxLayout();
    continousLayout->setSpacing(7);
    settings_continous->setLayout(continousLayout);

    subVLayout->addWidget(settings_discrete);

    layout->addLayout(subVLayout);
    layout->addWidget(settings_continous);

    setup_discrete_settings();
    setup_continous_settings();

    connect(&Linker::getInstance(), &Linker::send_checked_monitors, this, &MonitorWidget::receive_checked_monitors);
}

void MonitorWidget::setup_discrete_settings()
{ 
    foreach(auto elem, Linker::getInstance().get_monitors())
    {
        settings_discrete->addItem(elem->get_name(), QVariant(elem->get_ID()));
        
        QWidget* pageWidget = new QWidget;
        QVBoxLayout* pageLayout = new QVBoxLayout;
        pageLayout->setAlignment(Qt::AlignTop);
        pageWidget->setLayout(pageLayout);

        QComboBox* cbInput = new QComboBox;
        cbInput->setObjectName(QString("cb_%1_60").arg(elem->get_name()));

        QComboBox* cbClrProfile = new QComboBox;
        cbClrProfile->setObjectName(QString("cb_%1_14").arg(elem->get_name()));

        pageLayout->addWidget(cbInput);
        pageLayout->addWidget(cbClrProfile);

        stackedWidget->addWidget(pageWidget);

        ////TODO: Set combobox item to current value of monitor
        chk_add_discrete_feature(elem, "60");
        chk_add_discrete_feature(elem, "14");

        connect(cbInput, &QComboBox::currentIndexChanged, this, &MonitorWidget::discrete_setting_changed);
        connect(cbClrProfile, &QComboBox::currentIndexChanged, this, &MonitorWidget::discrete_setting_changed);

    }

    connect(settings_discrete, &ComboBoxFrame::comboBoxItemChanged, this, &MonitorWidget::cb_monitor_change);
}

void MonitorWidget::setup_continous_settings()
{
    QString cde_str = n2hexstr(0x10, 2);
    SliderWidget* lumSlider = new SliderWidget(this, 0x10);
    lumSlider->add_trayIcon();
    lumSlider->lock();

    lumSlider->set_value(10);
    sliders[cde_str] = lumSlider;

    //SliderWidget* volSlider = new SliderWidget(this, 0x62);
    //volSlider->set_color(QColor(0, 255, 0));
    //volSlider->add_trayIcon();

    subHLayout->addWidget(lumSlider);
    //subHLayout->addWidget(volSlider);

    subVLayout->addLayout(subHLayout);
}

void MonitorWidget::add_contextMenu(QMenu* menu)
{
    contextMenu = menu;

    for (auto [key, value] : sliders.asKeyValueRange())
    {
        value->set_contextMenu(menu);
    }
}

void MonitorWidget::chk_add_discrete_feature(Monitor* mon, QString qsft)
{
    if (mon->features.contains(qsft))
    {
        QList<QString> tempVec = VCP_FEATURES.commands[qsft].possible_values.keys();

        QString cbName = QString("cb_%1_%2").arg(mon->get_name()).arg(qsft);
        QComboBox* cb = this->findChild<QComboBox*>(cbName);

        for (auto& ft : mon->features[qsft].possible_values)
        {
            QString ft_name = n2hexstr(ft, 2);

            if (VCP_FEATURES.commands[qsft].possible_values.contains(ft_name))
            {
                QList<QVariant> itemData{ QVariant(qsft), QVariant(ft) };

                if (cb != NULL) {
                    cb->addItem(VCP_FEATURES.commands[qsft].possible_values[ft_name], itemData);

                }
            }

            QVariant currentData(QVariantList{ QVariant(qsft), QVariant(mon->features[qsft].current_value) });
            int ind = cb->findData(currentData);

            if (ind != -1) { // -1 for not found
                cb->setCurrentIndex(ind);
            }
        }
    }
}

void MonitorWidget::discrete_setting_changed(int index)
{
    auto obj = qobject_cast<QComboBox*>(sender());

    if (obj != nullptr)
    {
        int id = settings_discrete->monitorBox->currentData().toInt();
        QString cde_str = (obj->currentData()).toList()[0].toString();
        uint16_t value = (obj->currentData()).toList()[1].toUInt();

        Linker::getInstance().receive_discrete_setting(id, cde_str, value);
        //emit send_discrete_setting(id, cde_str, value);
    }
}

void MonitorWidget::cb_monitor_change(QString& name, int& id)
{
    qDebug() << name << " -- " << id;
    qDebug() << "Update ComboBoxes ...";

    stackedWidget->setCurrentIndex(id);
}

void MonitorWidget::receive_checked_monitors(QList<int> monIDs)
{
    qDebug() << monIDs;

    if (!monIDs.empty())
    {
        if (monIDs.size() < 2)
        {
            for (auto [key, value] : sliders.asKeyValueRange())
            {
                int val = Linker::getInstance().get_monitor_byID(monIDs[0])->features[key].current_value;
                value->set_value(val);
            }
        }
    }
}

void MonitorWidget::add_slider()
{ 
    AddSliderDialog dlg(this);

    int ret = dlg.exec();

    if (ret == 1)
    {
        QList<QVariant> retVals = dlg.get_values();

        QColor col = retVals[1].value<QColor>();
        uint16_t code = retVals[0].toUInt();
        bool trayChk = retVals[2].toBool();

        qDebug() << retVals;

        if (retVals[0].toUInt() > 0)
        {
            QString cde_str = n2hexstr(code, 2);

            if (VCP_FEATURES.commands.contains(cde_str) && !(sliders.contains(cde_str)))
            {
                // TODO add message for unsupported code
                for (auto& elem : Linker::getInstance().get_monitors())
                {
                    bool bChk = elem->add_check_feature(code, true);
                }
                
                
                SliderWidget* newSlider = new SliderWidget(this, code);

                if (trayChk) { 
                    newSlider->add_trayIcon();

                    if (contextMenu != nullptr) newSlider->set_contextMenu(contextMenu);
                }

                if (Linker::getInstance().get_checked_monitors().empty()) newSlider->lock();

                newSlider->set_color(col);

                sliders[cde_str] = newSlider;

                continousLayout->addWidget(newSlider);
            }
        }
    }
}