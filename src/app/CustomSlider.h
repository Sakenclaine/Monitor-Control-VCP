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
#include "MonitorHandler.h"

class CustomSlider : public QWidget
{
	Q_OBJECT

public:
	CustomSlider(QWidget *parent, uint16_t code);
	CustomSlider(QWidget* parent, QColor color, uint16_t code);
	
	~CustomSlider();

public:
	void set_colors(QColor col_handle, QColor col_fill, QColor col_background);
	void set_tooltip(QString tooltip);

	void add_trayIcon();

	void set_contextMenu(QMenu&);
	void refresh_value();

	//TrayIconControlled* get_trayIcon();
	QSlider* get_slider();


private:
	int _id;
	static int idProvider;
	QString receiverType = "sliderControl";

	bool trayIcon = false;

	QColor color = QColor("pink");
	uint16_t code = 0x00;

	QHBoxLayout* mainLayout;
	QVBoxLayout* mainVLayout;
	QHBoxLayout* mainHLayout;
	QVBoxLayout* buttonLayout;


	QSlider* slider;
	TrayIconControlled* icon;


	std::string width = std::to_string(0);
	std::string handle = "#1795f6";
	std::string lower_fill = "pink";
	std::string upper_fill = "gray";


	int current_value;


private:
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


