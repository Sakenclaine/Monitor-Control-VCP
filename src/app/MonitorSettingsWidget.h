#pragma once
#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>

#include "MonitorHandler.h"
#include "CustomSlider.h"


class MonitorWidget : public QWidget
{
	Q_OBJECT

private:
	Monitor* monitor = nullptr;
	QMap<QString, CustomSlider*> customSliders;

public:
	MonitorWidget(Monitor* monitor);
	MonitorWidget();


private:
	QGroupBox* settings_discrete;
	QGroupBox* settings_continous;

	QHBoxLayout* hSliderLayout;


public:
	void add_slider(uint16_t code, bool btrayIcon);
	void add_slider(uint16_t code, QColor color, bool btrayIcon);
	void add_contextMenu(QMenu* menu);

};


class PlaceholderWidget : public QWidget
{
	Q_OBJECT

public:
	PlaceholderWidget();
};
