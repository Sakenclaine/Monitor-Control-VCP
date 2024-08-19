#include "MonitorSettingsWidget.h"
#include "MonitorHandler.h"
#include "CustomFrames.h"
#include "SignalLinker.h"
#include "helpers.h"
#include "CustomSlider.h"


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QComboBox>

#include <QList>
#include <QString>


MonitorWidget::MonitorWidget(QWidget* parent) :
	QWidget(parent)
{
	layout = new QHBoxLayout(this);

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

    continousLayout = new QHBoxLayout();
    continousLayout->setSpacing(0);
    settings_continous->setLayout(continousLayout);


    layout->addWidget(settings_discrete);
    layout->addWidget(settings_continous);

    setup_discrete_settings();
    setup_continous_settings();
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
    SliderWidget* slider = new SliderWidget(this, 0x12);
    slider->add_trayIcon();

    continousLayout->addWidget(slider);
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