#include "CustomSlider.h"
#include "SignalLinker.h"
#include "workers.h"
#include "TrayIconControlled.h"

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QCheckBox>

#include <string>


int CustomSlider::idProvider = 0;

CustomSlider::CustomSlider(
	QWidget* parent = nullptr, 
	std::string handle_color="#1795f6",
	std::string lower_fill="pink",
	std::string upper_fill="gray") :

	QWidget(parent),
	_id(++idProvider),
	handle(handle_color),
	lower_fill(lower_fill),
	upper_fill(upper_fill)
{

	this->setup();

}

CustomSlider::CustomSlider(
	QWidget* parent = nullptr,
	std::string handle_color = "#1795f6",
	std::string lower_fill = "pink",
	std::string upper_fill = "gray",
	bool trayIcon=false) :

	QWidget(parent),
	_id(++idProvider),
	handle(handle_color),
	lower_fill(lower_fill),
	upper_fill(upper_fill)
{

	this->setup();

}


CustomSlider::CustomSlider(QWidget* parent) :
	QWidget(parent),
	_id(++idProvider)
{
	
	this->setup();

}

CustomSlider::CustomSlider(QWidget* parent, bool trayIcon) :
	QWidget(parent),
	_id(++idProvider),
	trayIcon(trayIcon)
{

	this->setup();

	if (trayIcon)
	{
		icon = new TrayIconControlled(this, 0, QColor(255, 255, 255), 0, 100, 10);

		connect(&Linker::getInstance(), &Linker::emit_mouse_update, icon, &TrayIconControlled::mouse_over);
		connect(icon, &TrayIconControlled::value_changed, &Linker::getInstance(), &Linker::receive_value_update);

		connect(&Linker::getInstance(), &Linker::emit_value_update, icon, &TrayIconControlled::update_value);

		
		connect(this, &CustomSlider::slider_changed_value, &Linker::getInstance(), &Linker::receive_value_update);
		connect(&Linker::getInstance(), &Linker::emit_value_update, this, &CustomSlider::set_slider_value);

		QString idIcon = QString("trayIcon_%1").arg(_id);
		QString idSlider = QString("sliderControl_%1").arg(_id);

		icon->setObjectName(idIcon);
		this->setObjectName(idSlider);

		icon->show();

	}

}


CustomSlider::CustomSlider(QWidget* parent, bool trayIcon, QColor color) :
	QWidget(parent),
	_id(++idProvider),
	trayIcon(trayIcon)
{

	lower_fill = color.name().toUtf8();
	
	this->setup();

	if (trayIcon)
	{
		icon = new TrayIconControlled(this, 0, color, 0, 100, 10);

		connect(&Linker::getInstance(), &Linker::emit_mouse_update, icon, &TrayIconControlled::mouse_over);
		connect(icon, &TrayIconControlled::value_changed, &Linker::getInstance(), &Linker::receive_value_update);

		connect(&Linker::getInstance(), &Linker::emit_value_update, icon, &TrayIconControlled::update_value);


		connect(this, &CustomSlider::slider_changed_value, &Linker::getInstance(), &Linker::receive_value_update);
		connect(&Linker::getInstance(), &Linker::emit_value_update, this, &CustomSlider::set_slider_value);

		QString idIcon = QString("trayIcon_%1").arg(_id);
		QString idSlider = QString("sliderControl_%1").arg(_id);

		icon->setObjectName(idIcon);
		this->setObjectName(idSlider);


		icon->show();

	}

}

CustomSlider::CustomSlider(QWidget* parent, bool trayIcon, QColor color, uint16_t code) :
	QWidget(parent),
	_id(++idProvider),
	trayIcon(trayIcon),
	code(code)
{

	lower_fill = color.name().toUtf8();

	this->setup();

	if (trayIcon)
	{
		icon = new TrayIconControlled(this, 0, color, 0, 100, 10);

		connect(&Linker::getInstance(), &Linker::emit_mouse_update, icon, &TrayIconControlled::mouse_over);
		connect(icon, &TrayIconControlled::value_changed, &Linker::getInstance(), &Linker::receive_value_update);

		connect(&Linker::getInstance(), &Linker::emit_value_update, icon, &TrayIconControlled::update_value);


		connect(this, &CustomSlider::slider_changed_value, &Linker::getInstance(), &Linker::receive_value_update);
		connect(&Linker::getInstance(), &Linker::emit_value_update, this, &CustomSlider::set_slider_value);

		QString idIcon = QString("trayIcon_%1").arg(_id);
		QString idSlider = QString("sliderControl_%1").arg(_id);

		icon->setObjectName(idIcon);
		this->setObjectName(idSlider);


		icon->show();

	}

}

CustomSlider::~CustomSlider()
{

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

void CustomSlider::setup()
{
	QVBoxLayout* mainVLayout = new QVBoxLayout;
	QHBoxLayout* mainHLayout = new QHBoxLayout;
	QVBoxLayout* buttonLayout = new QVBoxLayout;
	
	
	slider = new QSlider();
	slider->setMinimum(0);
	slider->setMaximum(100);


	std::string style = std::format("QSlider::groove:vertical {{background: red; position: absolute; left: {}px; right: {}px; }} QSlider::handle:vertical {{ height: 10px; background: {}; margin: 0 0px; }} QSlider::add-page:vertical {{ background: {}; }} QSlider::sub-page:vertical {{  background: {}; }}", width, width, handle, lower_fill, upper_fill);


	slider->setStyleSheet(QString::fromStdString(style));

	connect(slider, &QSlider::valueChanged, this, &CustomSlider::slider_changed);
	//connect(this, &CustomSlider::slider_changed_value, &Linker::get_instance(), &Linker::receive_signal);


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

	if (trayIcon)
	{
		QCheckBox* cbtoggleTray = new QCheckBox();
		cbtoggleTray->setCheckState(Qt::Checked);

		mainVLayout->addWidget(cbtoggleTray);

		connect(cbtoggleTray, &QCheckBox::checkStateChanged, this, &CustomSlider::toggle_trayIcon);	
	}

	mainHLayout->addWidget(slider);
	mainHLayout->addLayout(buttonLayout);

	mainVLayout->addLayout(mainHLayout);

	this->setLayout(mainVLayout);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

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
		QString senderType = id_components.value(id_components.length() - 2);
		int sender_id = (id_components.value(id_components.length()-1)).toInt();

		//qDebug() << "Slider " << this->objectName() << " Updated from: " << senderName;
		//qDebug() << "Sender: " << senderType << " .. " << sender_id << " Receiver: " << " .. " << _id << receiverType;
		//qDebug() << " Sended Value: " << value << "\n";

		if (sender_id == _id && current_value != value)
		{
			slider->setValue(value);
		}

	}	
}

void CustomSlider::slider_changed()
{
	current_value = slider->value();

	qDebug() << " Receiver: " << " .. " << _id << receiverType << " --> " << current_value;
	
	emit slider_changed_value(current_value);
	emit send_monitor_signal(code, current_value);
}