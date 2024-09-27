#include "Dialogs.h"
#include "MonitorHandler.h"
#include "helpers.h"

#include <QValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <QFontMetrics>
#include <QCompleter>
#include <QMenu>




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

	QWidget* code_input = new QWidget(this);
	QHBoxLayout* input_layout = new QHBoxLayout();
	input_layout->setContentsMargins(0, 0, 0, 0);
	input_layout->setSpacing(0);
	code_input->setLayout(input_layout);

	possibleCodes = new QComboBox;
	possibleCodes->addItem("Test");

	QMenu* codes = new QMenu(this);

	for (auto elem : VCP_FEATURES.commands)
	{
		if (elem.continous)
		{
			QAction* act = new QAction(elem.name);
			act->setData(QVariant(n2hexstr(elem.vcp_code, 2)));
			codes->addAction(act);

			connect(act, &QAction::triggered, this, &AddSliderDialog::set_code);
		}
	}

	codesMenu = new QToolButton;
	codesMenu->setPopupMode(QToolButton::InstantPopup);
	codesMenu->setArrowType(Qt::ArrowType::DownArrow);
	codesMenu->setMenu(codes);


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

	input_layout->addWidget(lineCode);
	input_layout->addWidget(codesMenu);

	layout->addRow(code_input, colorBtn);
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

void AddSliderDialog::set_code()
{
	QAction* obj = qobject_cast<QAction*>(sender());

	qDebug() << "Set Code Invoked " << obj;

	if (obj != nullptr)
	{
		QString cde_str = QString("0x") + obj->data().toString();

		qDebug() << "Code to set: " << cde_str;

		lineCode->setText(cde_str);
	}

	
}


void AddSliderDialog::set_color_btn(QColor col)
{
	colBtnStyle = QString(
		"background-color: %1;"
		"border-style: outset;"
		"border-color: beige;"
		"border-width: 2px;"
		"border-radius: 5px;").arg(col.name());

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