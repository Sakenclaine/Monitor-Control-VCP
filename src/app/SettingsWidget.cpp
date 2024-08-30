#include "SettingsWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

SettingsWidget::SettingsWidget(QWidget* parent) :
	QWidget(parent)
{
	setup();
}

void SettingsWidget::setup()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	
	categoryWidget = new QListWidget(this);
	settingsWidget = new QStackedWidget(this);

	mainLayout->addWidget(categoryWidget);
	mainLayout->addWidget(settingsWidget);

	connect(categoryWidget, &QListWidget::currentItemChanged, this, &SettingsWidget::category_changed);

}

void SettingsWidget::add_category(QString name, QWidget* wdgt)
{
	new QListWidgetItem(name, categoryWidget);
	settingsWidget->addWidget(wdgt);
}



void SettingsWidget::category_changed(QListWidgetItem* current, QListWidgetItem* previous)
{
	QString name = current->text();
	settingsWidget->setCurrentIndex(categoryWidget->currentRow());

	qDebug() << "New category: " << name;

}



SettingsDialog::SettingsDialog(QWidget* parent) :
	QDialog(parent)
{
	setup();
}

void SettingsDialog::setup()
{
	setWindowTitle(tr("Monitor Control - Settings"));

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	settingsWdgt = new SettingsWidget(this);

	mainLayout->addWidget(settingsWdgt);



	GeneralSettings* gen = new GeneralSettings();
	QWidget* col = new QWidget(this);
	QWidget* aut = new QWidget(this);

	add_settings_page(QString(tr("General")), gen);
	settingsWdgt->add_category(QString(tr("Colours")), col);
	settingsWdgt->add_category(QString(tr("Automation")), aut);
}

void SettingsDialog::add_settings_page(QString name, QWidget* settings)
{
	settingsWdgt->add_category(name, settings);
}
