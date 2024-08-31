#include "SettingsWidget.h"
#include "utilities.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QScreen>
#include <QFont>
#include <QVariant>
#include <QList>

SettingsWidget::SettingsWidget(QWidget* parent) :
	QWidget(parent)
{
	setup();
}

void SettingsWidget::setup()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	
	categoryWidget = new QListWidget(this);
	categoryWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	QFont font = categoryWidget->font();
	font.setPointSize(12);
	categoryWidget->setFont(font);
	

	settingsWidget = new QStackedWidget(this);

	mainLayout->addWidget(categoryWidget);
	mainLayout->addWidget(settingsWidget);

	connect(categoryWidget, &QListWidget::currentItemChanged, this, &SettingsWidget::category_changed);

}

void SettingsWidget::add_category(QString name, QWidget* wdgt)
{
	new QListWidgetItem(name, categoryWidget);
	settingsWidget->addWidget(wdgt);

	categoryWidget->setMinimumWidth(categoryWidget->sizeHintForColumn(0) + 15);
	categoryWidget->setMaximumWidth(std::min(100, categoryWidget->sizeHintForColumn(0) + 25));

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
	QScreen* screen = this->screen(); 
	QRect  screenGeometry = screen->geometry();
	int height = screenGeometry.height();
	int width = screenGeometry.width();

	qDebug() << "Settings Size: " << static_cast<int>(width / 2) << " x " << static_cast<int>(height / 2);
	
	setWindowTitle(tr("Monitor Control - Settings"));
	resize(static_cast<int>(width / 3), static_cast<int>(height / 3));

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


GeneralSettings::GeneralSettings(QWidget* parent) : 
	QWidget(parent) 
{

	mainLayout = new QGridLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setAlignment(Qt::AlignTop);
	setLayout(mainLayout);

	subLayout = new QGridLayout();


	QGroupBox* frame = new QGroupBox(tr("General"));
	frame->setLayout(subLayout);

	mainLayout->addWidget(frame, 0, 0);

	QFormLayout* formLayout = new QFormLayout();

	QCheckBox* autoStart = new QCheckBox();
	QLabel* autoStartLabel = new QLabel(tr("Autostart at login"));

	bool autoStartStatus = SettingsManager::getInstance().readSetting("Settings", "autostart").toBool();
	autoStart->setChecked(autoStartStatus);

	connect(autoStart, &QCheckBox::checkStateChanged, this, &GeneralSettings::toggle_autostart);

	formLayout->addRow(autoStart, autoStartLabel);

	subLayout->addLayout(formLayout, 0, 0);
}

void GeneralSettings::write_settings()
{

}

void GeneralSettings::toggle_autostart(bool state)
{
	qDebug() << "\n--> Autostart toggled to " << state << "\n";

	setAppToStartAutomatically(state);

	SettingsManager::getInstance().writeSettingInGroup("Settings", "autostart", QVariant(state));
}
