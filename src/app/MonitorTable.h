#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QStringList>
#include <QCheckBox>

#include "MonitorHandler.h"

class MonitorTable : public QTableWidget
{
	Q_OBJECT

private:
	QStringList tableHeader;
	QList<QCheckBox*> checkBoxes;

private:
	QWidget* create_checkbox(); // Create checkbox to add to first table column

public:
	MonitorTable(QWidget* parent = nullptr, QStringList tableHeader = QStringList{});

public:
	void add_monitor();
	void add_monitor(Monitor* monitor);

private slots:
	void monitor_selection_changed_();


signals:
	void monitor_selection_changed(QString&, int&);

};