#include "errors.h"
#include "helpers.h"

#include <QString>

NotImplemented::NotImplemented() : 
	std::logic_error("Function not yet implemented") 
{ };


FeatureWarning::FeatureWarning(uint16_t code)
{
	QString text = QString("VCP Code %1 was not reported as supported from the monitor.").arg(QString::fromStdString(uIntToString(code)));
	
	setText(text);
	setInformativeText("Do you want to proceed?");
	setStandardButtons(QMessageBox::Yes | QMessageBox::Abort);
	setDefaultButton(QMessageBox::Abort);

	adjustSize();
}