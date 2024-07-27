#include "SettingsWidget.h"

#include <QHBoxLayout>
#include <QPushButton>


SettingsWidget::SettingsWidget()
{
	QHBoxLayout* layout = new QHBoxLayout(this);

	QPushButton* button = new QPushButton("Press");

	layout->addWidget(button);
}

SettingsWidget::SettingsWidget(QString path)
{
	QSettings settings(path, QSettings::IniFormat);
}
