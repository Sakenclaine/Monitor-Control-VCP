#include "CustomSlider.h"
#include "TrayIconControlled.h"
#include "MonitorHandler.h"
#include "SignalLinker.h"
#include "helpers.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSizePolicy>


CustomSlider::CustomSlider(QWidget* parent) : 
	QSlider(parent) 
{ 
	setTracking(false); 
}

void CustomSlider::set_lock(bool lock)
{
	locked = lock;
}

void CustomSlider::lock() {
	locked = true;
}

void CustomSlider::unlock() {
	locked = false;
}

void CustomSlider::set_value(int value) {
	if (!locked)
	{
		setValue(value);
	}
}

void CustomSlider::update_value(int& val, QObject& senderObj)
{
	int receiver_id = -1;
	QString receiver_code = "None";

	if (!locked)
	{
		QString senderName = senderObj.objectName();

		if (senderName != "")
		{
			QStringList id_components_receiver = (objectName()).split("_");


			if (id_components_receiver.length() > 1)
			{
				receiverType = id_components_receiver.value(id_components_receiver.length() - 3);
				receiver_id = (id_components_receiver.value(id_components_receiver.length() - 2)).toInt();
				receiver_code = id_components_receiver.value(id_components_receiver.length() - 1);
			}


			QStringList id_components_sender = senderName.split("_");
			QString senderType = id_components_sender.value(id_components_sender.length() - 3);
			int sender_id = (id_components_sender.value(id_components_sender.length() - 2)).toInt();
			QString sender_code = id_components_sender.value(id_components_sender.length() - 1);

			if (sender_id == receiver_id && senderType != receiverType)
			{
				if (value() != val)
				{
					setValue(val);
				}
			}
		}
	}
}

void CustomSlider::wheelEvent(QWheelEvent* e) 
{
	if (!locked) QSlider::wheelEvent(e);
}

bool CustomSlider::event(QEvent* e) 
{
	bool eventBool = false;
	
	if (!locked) {
		eventBool = QSlider::event(e);
		return eventBool;
	}

	else {
		if (e->type() == QEvent::Paint || 
			e->type() == QEvent::Resize || 
			e->type() == QEvent::StyleChange ||
			e->type() == QEvent::Polish ||
			e->type() == QEvent::PolishRequest ||
			e->type() == QEvent::Show ||
			e->type() == QEvent::ShowToParent ||
			e->type() == QEvent::UpdateLater ||
			e->type() == QEvent::Move ) {
			eventBool = QSlider::event(e);
			return eventBool;
		}

		else {
			e->ignore();
			return eventBool;
		}
	}
}

void CustomSlider::set_style(QString& style)
{
	setStyleSheet(style);

	//QString st = "QSlider::groove:vertical{background: red;	position: absolute;	left: 4px; right: 4px;}";

	//setStyleSheet(st);
}


int SliderWidget::idProvider = 0;


SliderWidget::SliderWidget(QWidget* parent, uint16_t code) :
	QWidget(parent),
	_id(++idProvider),
	code(code)
{
	cde_str = n2hexstr(code, 2);
	setup();
}



SliderWidget::SliderWidget(QWidget* parent, QColor color, uint16_t code) :
	QWidget(parent),
	_id(++idProvider),
	color(color),
	code(code)
{
	cde_str = n2hexstr(code, 2);
	setup();
}

SliderWidget::~SliderWidget()
{

}

void SliderWidget::setup()
{
	if (VCP_FEATURES.commands.contains(cde_str))
	{
		name = VCP_FEATURES.commands[cde_str].name;
	}

	qDebug() << "Init slider for " << cde_str << " (" << name << ")";
	
	mainVLayout = new QVBoxLayout();
	mainVLayout->setContentsMargins(0, 5, 0, 0);
	mainHLayout = new QHBoxLayout();
	buttonLayout = new QVBoxLayout();

	this->setLayout(mainVLayout);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

	slider = new CustomSlider();
	slider->setMinimum(0);
	slider->setMaximum(100);

	QString idSlider = QString("slider_%1_%2").arg(_id).arg(cde_str);
	slider->setObjectName(idSlider);

	if (VCP_FEATURES.commands.contains(cde_str))
	{
		slider->setToolTip(QString("%1\n%2").arg(name).arg(0));
	}

	lower_fill = color.name();

	QString style = QString(
		"QSlider::groove:vertical {background: %2; position: absolute; left: %1px; right: %1px; }"
		"QSlider::handle:vertical { height: 10px; background: %4; margin: 0px; }"
		"QSlider::add-page:vertical { background: %3; }"
		"QSlider::sub-page:vertical {  background: %2; }"
	).arg(width).arg(upper_fill).arg(lower_fill).arg(handle);

	slider->setStyleSheet(style);

	connect(slider, &CustomSlider::valueChanged, this, &SliderWidget::update_ToolTip);
	connect(slider, &CustomSlider::valueChanged, &Linker::getInstance(), &Linker::receive_value_update);
	connect(&Linker::getInstance(), &Linker::send_value_update, slider, &CustomSlider::update_value);
	connect(&Linker::getInstance(), &Linker::send_lock, this, &SliderWidget::set_lock);

	connect(slider, &CustomSlider::valueChanged, this, &SliderWidget::slider_changed);


	for (int i = 0; i < 21; i++)
	{
		QString name = QString::number(100 - i * 5);

		QPushButton* pButton = new QPushButton(name, this);
		pButton->setStyleSheet(QString("padding: 2px; min-width: 2em;"));

		QSize minSize = pButton->minimumSizeHint();

		connect(pButton, &QPushButton::clicked, this, &SliderWidget::buttonClick);

		pButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
		buttonLayout->addWidget(pButton);

	}

	trayChk = new QCheckBox();

	QSizePolicy sp_retain = trayChk->sizePolicy();
	sp_retain.setRetainSizeWhenHidden(true);
	trayChk->setSizePolicy(sp_retain);

	mainHLayout->addWidget(slider);
	mainHLayout->addLayout(buttonLayout);

	mainVLayout->addLayout(mainHLayout);
	mainVLayout->addWidget(trayChk);
	mainVLayout->setAlignment(trayChk, Qt::AlignHCenter);

	if (!trayIcon) trayChk->hide();


}

void SliderWidget::set_value(int value)
{
	slider->setValue(value);

	if (trayIcon) icon->set_value(value);
}

void SliderWidget::add_trayIcon()
{
	trayIcon = true;
	trayChk->show();
	trayChk->setCheckState(Qt::Checked);

	icon = new TrayIconControlled(this, slider->value(), color, 0, 100, 5);
	icon->set_identifier(_id, cde_str);

	connect(icon, &TrayIconControlled::value_changed, &Linker::getInstance(), &Linker::receive_value_update);
	connect(&Linker::getInstance(), &Linker::send_value_update, icon, &TrayIconControlled::update_value);

	icon->show();

	connect(trayChk, &QCheckBox::checkStateChanged, this, &SliderWidget::toggleTray);

}

void SliderWidget::set_contextMenu(QMenu* menu)
{
	if (trayIcon)
	{
		icon->setContextMenu(menu);
	}
}

void SliderWidget::set_color(QColor col)
{
	color = col;
	lower_fill = col.name();

	QString style = QString(
		"QSlider::groove:vertical {background: %2; position: absolute; left: %1px; right: %1px; }"
		"QSlider::handle:vertical { height: 10px; background: %4; margin: 0px; }"
		"QSlider::add-page:vertical { background: %3; }"
		"QSlider::sub-page:vertical {  background: %2; }"
	).arg(width).arg(upper_fill).arg(lower_fill).arg(handle);

	slider->setStyleSheet(style);

	if (trayIcon) icon->set_color(col);

}


CustomSlider* SliderWidget::get_slider()
{
	return slider;
}

TrayIconControlled* SliderWidget::get_icon()
{
	return icon;
}


void SliderWidget::buttonClick()
{
	auto obj = qobject_cast<QPushButton*>(sender());

	if (nullptr != obj)
	{
		QPushButton* buttonSender = qobject_cast<QPushButton*>(sender()); // retrieve the button you have clicked
		QString buttonText = buttonSender->text(); // retrive the text from the button clicked

		slider->set_value(buttonText.toInt());
	}
}

void SliderWidget::update_ToolTip(int val)
{
	slider->setToolTip(QString("%1\n%2").arg(name).arg(val));
}

void SliderWidget::toggleTray(bool chk)
{
	if (trayIcon)
	{
		if (chk) icon->show();
		else if (!chk) icon->hide();
	}
}


void SliderWidget::set_lock(bool lock)
{
	locked = lock;
	slider->set_lock(locked);

	if (trayIcon) icon->set_lock(lock);
}

void SliderWidget::lock() {
	locked = true;
	slider->set_lock(true);

	if (trayIcon) icon->set_lock(true);
}

void SliderWidget::unlock() {
	locked = false;
	slider->set_lock(false);

	if (trayIcon) icon->set_lock(false);
}

void SliderWidget::slider_changed()
{
	emit value_changed(cde_str, slider->value());
}