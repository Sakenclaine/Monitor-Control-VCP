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


void MonitorWidget::add_slider(QMenu* contextMenu)
{
    //CustomSlider* cSlider = new CustomSlider(NULL, true, color, code);

    //cSlider->set_contextMenu(*trayIconMenu);

    //cSlider->get_trayIcon()->setToolTip(name);
    //cSlider->setToolTip(name);
    //sliderLayout->addWidget(cSlider);

    //connect(cSlider->get_trayIcon(), &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    //for (auto& monitor : registered_monitors)
    //{
    //    connect(cSlider, &CustomSlider::send_monitor_signal, monitor, &Monitor::receive_signal);
    //}
}