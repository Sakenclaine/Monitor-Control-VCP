#include "MonitorSettingsWidget.h"

#include <QGroupBox>
#include <QHBoxLayout>


MonitorWidget::MonitorWidget(Monitor* monitor) :
	monitor(monitor)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	
	settings_discrete = new QGroupBox("Boxes");
	settings_continous = new QGroupBox("Slider");

	layout->addWidget(settings_discrete);
	layout->addWidget(settings_continous);

}
