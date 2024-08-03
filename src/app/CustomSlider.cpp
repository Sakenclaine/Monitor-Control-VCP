#include "CustomSlider.h"
#include "SignalLinker.h"
#include "workers.h"
#include "TrayIconControlled.h"
#include "helpers.h"

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QCheckBox>

#include <string>


int CustomSlider::idProvider = 0;


CustomSlider::CustomSlider(QWidget* parent, uint16_t code) :
	QWidget(parent),
	_id(++idProvider),
	code(code)
{
	setup();
}



CustomSlider::CustomSlider(QWidget* parent, QColor color, uint16_t code) :
	QWidget(parent),
	_id(++idProvider),
	color(color),
	code(code)
{
	setup();
}



CustomSlider::~CustomSlider()
{

}

void CustomSlider::setup()
{
	connect(this, &CustomSlider::send_monitor_value, &Linker::getInstance(), &Linker::receive_monitor_value);

	mainVLayout = new QVBoxLayout();
	mainHLayout = new QHBoxLayout();
	buttonLayout = new QVBoxLayout();


	slider = new QSlider();
	slider->setMinimum(0);
	slider->setMaximum(100);

	if (VCP_FEATURES.commands.find(n2hexstr(code, 2)) != VCP_FEATURES.commands.end())
	{
		slider->setToolTip(VCP_FEATURES.commands[n2hexstr(code, 2)].name);
	}

	lower_fill = color.name().toStdString();

	std::string style = std::format("QSlider::groove:vertical {{background: red; position: absolute; left: {}px; right: {}px; }} QSlider::handle:vertical {{ height: 10px; background: {}; margin: 0 0px; }} QSlider::add-page:vertical {{ background: {}; }} QSlider::sub-page:vertical {{  background: {}; }}", width, width, handle, lower_fill, upper_fill);


	slider->setStyleSheet(QString::fromStdString(style));

	connect(slider, &QSlider::valueChanged, this, &CustomSlider::slider_changed);


	for (int i = 0; i < 21; i++)
	{
		QString name = QString::number(100 - i * 5);

		QPushButton* pButton = new QPushButton(name, this);
		pButton->setStyleSheet(QString("padding: 2px; min-width: 2em;"));

		QSize minSize = pButton->minimumSizeHint();

		connect(pButton, &QPushButton::clicked, this, &CustomSlider::buttonClick);

		pButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
		buttonLayout->addWidget(pButton);

	}

	mainHLayout->addWidget(slider);
	mainHLayout->addLayout(buttonLayout);

	mainVLayout->addLayout(mainHLayout);

	this->setLayout(mainVLayout);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

	QString idCode = n2hexstr(code);
	QString idSlider = QString("sliderControl_%1_%2").arg(_id).arg(idCode);

	this->setObjectName(idSlider);

	Linker::getInstance().register_slider(this);

}

void CustomSlider::set_colors(QColor col_handle, QColor col_fill, QColor col_background)
{
	handle = col_handle.name().toStdString();
	lower_fill = col_fill.name().toStdString();
	upper_fill = col_background.name().toStdString();

	std::string style = std::format("QSlider::groove:vertical {{background: red; position: absolute; left: {}px; right: {}px; }} QSlider::handle:vertical {{ height: 10px; background: {}; margin: 0 0px; }} QSlider::add-page:vertical {{ background: {}; }} QSlider::sub-page:vertical {{  background: {}; }}", width, width, handle, lower_fill, upper_fill);

	slider->setStyleSheet(QString::fromStdString(style));
}

void CustomSlider::set_tooltip(QString tooltip)
{
	slider->setToolTip(tooltip);
	
	if (trayIcon) icon->setToolTip(tooltip);
}

void CustomSlider::add_trayIcon()
{
	trayIcon = true;
	icon = new TrayIconControlled(this, 0, color, 0, 100, 10);

	if (VCP_FEATURES.commands.find(n2hexstr(code, 2)) != VCP_FEATURES.commands.end())
	{
		icon->setToolTip(VCP_FEATURES.commands[n2hexstr(code, 2)].name);
	}

	connect(&Linker::getInstance(), &Linker::emit_mouse_update, icon, &TrayIconControlled::mouse_over);
	connect(icon, &TrayIconControlled::value_changed, &Linker::getInstance(), &Linker::receive_value_update);

	connect(&Linker::getInstance(), &Linker::emit_value_update, icon, &TrayIconControlled::update_value);

	
	connect(this, &CustomSlider::slider_changed_value, &Linker::getInstance(), &Linker::receive_value_update);
	connect(&Linker::getInstance(), &Linker::emit_value_update, this, &CustomSlider::set_slider_value);

	QString idCode = n2hexstr(code);
	QString idIcon = QString("trayIcon_%1_%2").arg(_id).arg(idCode);

	icon->setObjectName(idIcon);
	Linker::getInstance().register_icon(icon);

	icon->show();

	QCheckBox* cbtoggleTray = new QCheckBox();
	cbtoggleTray->setCheckState(Qt::Checked);
	mainVLayout->addWidget(cbtoggleTray);

	connect(cbtoggleTray, &QCheckBox::checkStateChanged, this, &CustomSlider::toggle_trayIcon);

}

void CustomSlider::toggle_trayIcon(bool toggle)
{
	if (trayIcon)
	{
		if (toggle == false)
		{
			icon->hide();
		}

		else if (toggle == true)
		{
			icon->show();
		}
	}
}

TrayIconControlled* CustomSlider::get_trayIcon()
{
	if (trayIcon) { return icon; }
	else { return nullptr; }
}

QSlider* CustomSlider::get_slider()
{
	return slider;
}

void CustomSlider::set_contextMenu(QMenu& menu)
{
	icon->setContextMenu(&menu);
}

void CustomSlider::value_changed()
{
	qDebug() << "Code: " <<  slider->value();
}


void CustomSlider::buttonClick()
{	
	auto obj = qobject_cast<QPushButton*>(sender());
	
	if (nullptr != obj)
	{
		QPushButton* buttonSender = qobject_cast<QPushButton*>(sender()); // retrieve the button you have clicked
		QString buttonText = buttonSender->text(); // retrive the text from the button clicked

		slider->setValue(buttonText.toInt());
		}
}

void CustomSlider::set_slider_value(int& value, QObject& senderObj)
{
	QString senderName = senderObj.objectName();
	auto obj = qobject_cast<QPushButton*>(sender());

	if (senderName != "" && obj == nullptr)
	{
		QStringList id_components = senderName.split("_");
		QString senderType = id_components.value(id_components.length() - 3);
		int sender_id = (id_components.value(id_components.length()-2)).toInt();
		QString sender_code = id_components.value(id_components.length() - 1);

		if (sender_id == _id && current_value != value)
		{
			slider->setValue(value);
		}

	}	
}

void CustomSlider::slider_changed()
{
	current_value = slider->value();

	//qDebug() << " Receiver: " << " .. " << _id << receiverType << " --> " << current_value;
	
	emit slider_changed_value(current_value);
	emit send_monitor_value(code, current_value);
}

void CustomSlider::refresh_value()
{
	emit request_value(code);
}