#pragma once

#include <QWidget>
#include <QList>
#include <QMenu>

QT_BEGIN_NAMESPACE
class ComboBoxFrame;
class ScrollFrame;
class QScrollArea;
class QHBoxLayout;
class QVBoxLayout;
class QStackedWidget;

class Monitor;
class CustomSlider;
class SliderWidget;
QT_END_NAMESPACE


class MonitorWidget : public QWidget
{
	Q_OBJECT

private:
	QMap<QString, SliderWidget*> sliders;

public:
	MonitorWidget(QWidget* parent = nullptr);


private:
	ComboBoxFrame* settings_discrete;
	ScrollFrame* settings_continous;

	ScrollFrame* settingsFrame;

	QHBoxLayout* layout;
	QHBoxLayout* continousLayout;
	QVBoxLayout* discreteLayout;

	QVBoxLayout* subVLayout;
	QHBoxLayout* subHLayout;
	

	QStackedWidget* stackedWidget;
	QMenu* contextMenu = nullptr;


private:
	void setup_discrete_settings();
	void setup_continous_settings();
//	void init_slider(QString name);
//
//
public:
//	void add_slider(uint16_t code, bool btrayIcon);
//	void add_slider(uint16_t code, QColor color, bool btrayIcon);
	void add_contextMenu(QMenu* menu);

	void chk_add_discrete_feature(Monitor* mon, QString qsft);


public slots:
	void receive_checked_monitors(QList<int>);
//	void receive_add_slider(uint16_t&, QColor&, bool&);
//	void receive_slider_init(Monitor* monitor);

private slots:
	void cb_monitor_change(QString& name, int& ids);
	void discrete_setting_changed(int index);

	void add_slider();


signals:
	void send_discrete_setting(int& monitorID, QString& cde_str, uint16_t& value);
};
