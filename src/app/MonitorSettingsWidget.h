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
	QList<CustomSlider*> customSliders;

public:
	MonitorWidget(Monitor* monitor);
	MonitorWidget();


private:
	QGroupBox* settings_discrete;
	QGroupBox* settings_continous;

	QHBoxLayout* hSliderLayout;


public:
	void add_slider(bool btrayIcon);
	void add_contextMenu(QMenu* menu);

};


class PlaceholderWidget : public QWidget
{
	Q_OBJECT

public:
	PlaceholderWidget();
};
