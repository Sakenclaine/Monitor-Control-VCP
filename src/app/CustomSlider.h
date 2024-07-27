#pragma once

#include <QWidget>
#include <QSizePolicy>

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
class QSlider;
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE

#include "TrayIconControlled.h"

class CustomSlider : public QWidget
{
	Q_OBJECT

public:
	CustomSlider(QWidget *parent = nullptr);
	CustomSlider(QWidget* parent, std::string handle_color, std::string lower_fill, std::string upper_fill);
	CustomSlider(QWidget* parent, std::string handle_color, std::string lower_fill, std::string upper_fill, bool trayIcon);
	CustomSlider(QWidget* parent, bool trayIcon);
	CustomSlider(QWidget* parent, bool trayIcon, QColor color);
	CustomSlider(QWidget* parent, bool trayIcon, QColor color, uint16_t code);
	

	~CustomSlider();

	void set_contextMenu(QMenu&);
	void refresh_value();

	TrayIconControlled* get_trayIcon();
	QSlider* get_slider();


private:
	int _id;
	QString receiverType = tr("sliderControl");
	static int idProvider;

	bool trayIcon = false;
	int current_value;
	uint16_t code = 0x00;

	QSlider* slider;

	QHBoxLayout* mainLayout;
	QVBoxLayout* buttonLayout;

	TrayIconControlled* icon;


	std::string width = std::to_string(0);
	std::string handle = "#1795f6";
	std::string lower_fill = "pink";
	std::string upper_fill = "gray";

	void setup();

private slots:
	void value_changed();
	void buttonClick();

public slots:
	void set_slider_value(int &, QObject&);
	void toggle_trayIcon(bool toggle);
	void slider_changed();


signals:
	void request_value(uint16_t code);
	void slider_changed_value(int& value);
	void send_monitor_signal(uint16_t& code, int& value);
};


