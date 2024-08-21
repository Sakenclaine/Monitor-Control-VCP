#pragma once

#include <QWidget>
#include <QDialog>
#include <QColorDialog>

#include <QList>
#include <QStringList>
#include <QVariant>

#include <QSizePolicy>

#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>


// https://stackoverflow.com/questions/17512542/getting-multiple-inputs-from-qinputdialog-in-qt

class AddSliderDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddSliderDialog(QWidget* parent = nullptr, QStringList autocomplete = QStringList{});

private:
	QColor color = QColor("red");
	uint16_t code = 0;

	QString colBtnStyle = QString(
		"background-color: %1;"
		"border - radius: 20px;").arg(color.name());

	QStringList wordList;


private:
	QFormLayout* layout;

	QColorDialog* colDialog;
	QLineEdit* lineCode;
	QCheckBox* trayIconCheck;

	QPushButton* colorBtn;
	QPushButton* acceptBtn;
	QPushButton* cancelBtn;


private:
	void set_color_btn(QColor col);

	void setup();

public:
	static QList<QVariant> get_input(QWidget* parent = nullptr, bool* ok = nullptr);

	QList<QVariant> get_values();

private slots:
	void color_dialog();

	void set_color(const QColor& col);
};

