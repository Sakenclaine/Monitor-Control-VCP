#pragma once

#include <QWidget>
#include <QDialog>
#include <QMenu>
#include <QListWidget>
#include <QStackedWidget>
#include <QGroupBox>
#include <QGridLayout>

#include <QLabel>
#include <QCheckBox>
#include <QFormLayout>


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

private:
	QGridLayout* mainLayout;
	QGridLayout* subLayout;
	

public:
	GeneralSettings(QWidget* parent = nullptr);

private slots:
	void write_settings();

	void toggle_autostart(bool state);

};