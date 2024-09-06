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
#include <QComboBox>

QT_BEGIN_NAMESPACE
class ApplicationManager;
QT_END_NAMESPACE


class SettingsWidget : public QWidget
{
	Q_OBJECT

private:
	ApplicationManager* appMngr;

	QListWidget* categoryWidget;
	QStackedWidget* settingsWidget;


public:
	SettingsWidget(QWidget* parent = nullptr, ApplicationManager* appMngr = nullptr);

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
	ApplicationManager* appMngr;
	SettingsWidget* settingsWdgt;

public:
	SettingsDialog(QWidget* parent = nullptr, ApplicationManager* appMngr = nullptr);

private:
	void setup();

public:
	void add_settings_page(QString name, QWidget* settings);

};



class GeneralSettings : public QWidget
{
	Q_OBJECT

private:
	ApplicationManager* appMngr;

	QGridLayout* mainLayout;
	QGridLayout* subLayout;

	QLabel* autoStartLabel;
	QComboBox* langComboBox;


public:
	GeneralSettings(QWidget* parent = nullptr, ApplicationManager* appMngr = nullptr);

private slots:
	void write_settings();

	void toggle_autostart(bool state);
	void change_language(int);

};