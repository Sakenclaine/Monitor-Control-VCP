#pragma once

#include <QWidget>
#include <QDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QFrame>
#include <QPixMap>
#include <QPainter>
#include <QIcon>
#include <QRect>
#include <QPoint>
#include <QPainterPath>
#include <QList>
#include <QVariant>
#include <QValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSizePolicy>
#include <QFontMetrics>
#include <QCheckBox>

#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>


// https://stackoverflow.com/questions/17512542/getting-multiple-inputs-from-qinputdialog-in-qt

class Dialog_AddSlider : public QDialog
{
	Q_OBJECT

private:
	QColor color = QColor("red");
	uint16_t code = 0;
	QString colBtnStyle = "background-color: red;";



private:
	QFormLayout* layout;

	QLineEdit* lineCode;
	QPushButton* colorBtn;
	QPushButton* acceptBtn;
	QPushButton* cancelBtn;

	QCheckBox* trayIconCheck;

	QColorDialog* colDialog;

private:
	void set_color_btn(QColor col)
	{
		//colBtnStyle = QString("QPushButton {"
		//	"background-color: %1;" 
		//	"border-style: outset;"
		//	"border-width: 3px;" 
		//	"border-color: white;"
		//	"border-radius: 10px;" 
		//	"min-width: 1em;"
		//	"min-height: 1em;"
		//	"}").arg(col.name());

		colBtnStyle = QString(
			"background-color: %1;"
			"border - radius: 20px;").arg(col.name());
	}


public:
	explicit Dialog_AddSlider(QWidget* parent = nullptr) : QDialog(parent) 
	{
		this->setWindowTitle(tr("Add Slider"));
		set_color_btn(QColor("beige"));

		layout = new QFormLayout(this);

		colDialog = new QColorDialog();
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

		connect(colorBtn, &QPushButton::clicked, this, &Dialog_AddSlider::color_dialog);
		connect(colDialog, &QColorDialog::colorSelected, this, &Dialog_AddSlider::set_color);

		acceptBtn = new QPushButton("OK");
		cancelBtn = new QPushButton("Cancel");

		connect(acceptBtn, &QPushButton::clicked, this, &QDialog::accept);
		connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

		trayIconCheck = new QCheckBox();
		trayIconCheck->setChecked(true);

		layout->addRow(lineCode, colorBtn);
		layout->addRow(tr("&Icon:"), trayIconCheck);
		layout->addRow(acceptBtn, cancelBtn);
		

	}

	static QList<QVariant> get_input(QWidget* parent, bool* ok = nullptr)
	{
		Dialog_AddSlider* dialog = new Dialog_AddSlider(parent);
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

private slots:
	void color_dialog() {
		colDialog->exec();
	}

	void set_color(const QColor& col)
	{
		color = col;
		this->set_color_btn(col);
		colorBtn->setStyleSheet(colBtnStyle);
	}


};

