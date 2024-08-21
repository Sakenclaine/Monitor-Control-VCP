#include "Dialogs.h"

#include <QValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <QFontMetrics>


explicit AddSliderDialog::AddSliderDialog(QWidget* parent) : QDialog(parent)
{
	this->setWindowTitle(tr("Add Slider"));

	layout = new QFormLayout(this);

	colDialog = new QColorDialog(this);
	colDialog->adjustSize();


	QValidator* validator = new QRegularExpressionValidator(QRegularExpression("0x[0-9A-Fa-f][0-9A-Fa-f]{1,1}"));

	QString plcHolder = tr("HEX VCP Code, e.g. 0x10");
	QFontMetrics fontMtrcs = QWidget::fontMetrics();

	lineCode = new QLineEdit();
	lineCode->setPlaceholderText(plcHolder);
	lineCode->setValidator(validator);
	lineCode->setMinimumWidth(fontMtrcs.boundingRect(plcHolder).width() + 50);

	colorBtn = new QPushButton();
	colorBtn->setStyleSheet(colBtnStyle);

	connect(colorBtn, &QPushButton::clicked, this, &AddSliderDialog::color_dialog);
	connect(colDialog, &QColorDialog::colorSelected, this, &AddSliderDialog::set_color);

	trayIconCheck = new QCheckBox();
	trayIconCheck->setChecked(true);

	acceptBtn = new QPushButton("OK");
	cancelBtn = new QPushButton("Cancel");

	connect(acceptBtn, &QPushButton::clicked, this, &QDialog::accept);
	connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);


	layout->addRow(lineCode, colorBtn);
	layout->addRow(tr("&Icon:"), trayIconCheck);
	layout->addRow(acceptBtn, cancelBtn);


}

QList<QVariant> AddSliderDialog::get_input(QWidget* parent, bool* ok)
{
	AddSliderDialog* dialog = new AddSliderDialog(parent);
	dialog->adjustSize();

	QList<QVariant> list;
	const int ret = dialog->exec();

	if (ok)
		*ok = !!ret;

	if (ret) {
		QString txt = dialog->lineCode->text();

		bool flag;
		uint16_t c = txt.toUInt(&flag, 16);

		if (flag) list.append(QVariant(c));
		else list.append(QVariant(-1));

		list.append(QVariant(dialog->color));
		list.append(QVariant(dialog->trayIconCheck->isChecked()));
	}

	dialog->deleteLater();

	return list;
}

void AddSliderDialog::set_color_btn(QColor col)
{
	colBtnStyle = QString(
		"background-color: %1;"
		"border - radius: 20px;").arg(col.name());

	colorBtn->setStyleSheet(colBtnStyle);
}

void AddSliderDialog::color_dialog() {
	this->colDialog->exec();
}

void AddSliderDialog::set_color(const QColor& col)
{
	if (color != col)
	{
		color = col;
		this->set_color_btn(col);
	}
}