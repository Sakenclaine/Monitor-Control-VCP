#pragma once

#include <QWidget>
#include <QDialog>
#include <QMenu>
#include <QListWidget>
#include <QStackedWidget>
#include <QGroupBox>


class SettingsWidget : public QWidget
{
	Q_OBJECT

private:
	QListWidget* categoryWidget;
	QStackedWidget* settingsWidget;


public:
	SettingsWidget(QWidget* parent = nullptr);

private:
	void setup();

public:
	void add_category(QString name, QWidget* wdgt);

private slots:
	void category_changed(QListWidgetItem* current, QListWidgetItem* previous);

};

class SettingsDialog : public QDialog
{
	Q_OBJECT

private:
	SettingsWidget* settingsWdgt;

public:
	SettingsDialog(QWidget* parent = nullptr);

private:
	void setup();

public:
	void add_settings_page(QString name, QWidget* settings);

};


class GeneralSettings : public QWidget
{
	Q_OBJECT

public:
	GeneralSettings(QWidget* parent = nullptr) : QWidget(parent) {		
		QGroupBox* frame = new QGroupBox(tr("General"), this);




	}


};