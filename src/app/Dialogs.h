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

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>


class Dialog_AddSlider : public QDialog
{
	Q_OBJECT

private:
	QColor color = QColor("red");
	QString colBtnStyle = "background-color: red;";

private:
	QVBoxLayout* mainLayout;
	QHBoxLayout* buttonLayout;
	QHBoxLayout* inputLayout;

	QLineEdit* lineCode;
	QPushButton* colorBtn;
	QPushButton* acceptBtn;
	QPushButton* cancelBtn;

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
	Dialog_AddSlider(QWidget* parent = nullptr) : QDialog(parent) 
	{
		this->setWindowTitle(tr("Add Slider"));
		set_color_btn(QColor("beige"));

		mainLayout = new QVBoxLayout(this);
		buttonLayout = new QHBoxLayout();
		inputLayout = new QHBoxLayout();

		colDialog = new QColorDialog();

		lineCode = new QLineEdit();
		colorBtn = new QPushButton();
		colorBtn->setStyleSheet(colBtnStyle);

		connect(colorBtn, &QPushButton::clicked, this, &Dialog_AddSlider::color_dialog);
		connect(colDialog, &QColorDialog::colorSelected, this, &Dialog_AddSlider::set_color);

		acceptBtn = new QPushButton("OK");
		cancelBtn = new QPushButton("Cancel");

		connect(acceptBtn, &QPushButton::clicked, this, &QDialog::accept);
		connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

		mainLayout->addLayout(inputLayout);
		mainLayout->addLayout(buttonLayout);

		inputLayout->addWidget(lineCode);
		inputLayout->addWidget(colorBtn);
		
		buttonLayout->addWidget(acceptBtn);
		buttonLayout->addWidget(cancelBtn);
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

signals:

};

