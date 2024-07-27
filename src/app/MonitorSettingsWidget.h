#pragma once
#include <QWidget>
#include <QGroupBox>

#include "MonitorHandler.h"
#include "CustomSlider.h"


class MonitorWidget : public QWidget
{
	Q_OBJECT

private:
	Monitor* monitor = nullptr;

public:
	MonitorWidget(Monitor* monitor);


private:
	QGroupBox* settings_discrete;
	QGroupBox* settings_continous;


private:
	void add_slider(QMenu* contextMenu);

};
