#include "Dialogs.h"

#include <QValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <QFontMetrics>
#include <QCompleter>



AddSliderDialog::AddSliderDialog(QWidget* parent, QStringList autocomplete) :
	QDialog(parent),
	wordList(autocomplete)
{
	setup();
}

void AddSliderDialog::setup()
{
	this->setWindowTitle(tr("Add Slider"));

	layout = new QFormLayout(this);

	colDialog = new QColorDialog(this);
	// Weird layout problems of predefined Qt ColorDialog solved with manual resize.
	colDialog->adjustSize();
	colDialog->layout()->setSizeConstraint(QLayout::SetNoConstraint);
	colDialog->setFixedWidth(colDialog->sizeHint().width() + 135);
	colDialog->setFixedHeight(colDialog->sizeHint().height());


	QValidator* validator = new QRegularExpressionValidator(QRegularExpression("0x[0-9A-Fa-f][0-9A-Fa-f]{1,1}"));

	QString plcHolder = tr("HEX VCP Code, e.g. 0x10");
	QFontMetrics fontMtrcs = QWidget::fontMetrics();

	lineCode = new QLineEdit();
	lineCode->setPlaceholderText(plcHolder);
	lineCode->setValidator(validator);
	lineCode->setMinimumWidth(fontMtrcs.boundingRect(plcHolder).width() + 50);

	if (!wordList.empty())
	{
		QCompleter* completer = new QCompleter(wordList, this);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		lineCode->setCompleter(completer);
	}
	

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

	adjustSize();
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

QList<QVariant> AddSliderDialog::get_values()
{
	QList<QVariant> list;

	QString txt = this->lineCode->text();

	bool flag;
	uint16_t c = txt.toUInt(&flag, 16);

	if (flag) list.append(QVariant(c));
	else list.append(QVariant(-1));

	list.append(QVariant(this->color));
	list.append(QVariant(this->trayIconCheck->isChecked()));

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