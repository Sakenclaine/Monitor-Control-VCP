#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QStringList>
#include <QCheckBox>

#include <vector>

#include "MonitorHandler.h"

class MonitorTable : public QTableWidget
{
	Q_OBJECT

private:
	QStringList tableHeader;
	QList<QCheckBox*> checkBoxes;

private:
	QWidget* create_checkbox();

public:
	MonitorTable(QWidget* parent = 0, QStringList tableHeader = QStringList{});

public:
	void add_monitor();
	void add_monitor(Monitor* monitor);
};