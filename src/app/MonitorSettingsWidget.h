#pragma once
#include <QWidget>

#include "MonitorHandler.h"


class MonitorWidget : public QWidget
{
	Q_OBJECT

private:
	Monitor monitor;

public:
	MonitorWidget(Monitor& monitor);

};
