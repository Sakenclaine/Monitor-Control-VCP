#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QSettings>



class SettingsWidget : public QWidget
{
	Q_OBJECT

private:
	int i = 0;

public:
	SettingsWidget();
	SettingsWidget(QString path);

};