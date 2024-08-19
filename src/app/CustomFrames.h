#pragma once

#include <QWidget>
#include <QFrame>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QScrollArea;
class QPushButton;
class QComboBox;
QT_END_NAMESPACE

class ScrollFrame : public QFrame
{
	Q_OBJECT

private:
	QWidget* innerContent;
	QScrollArea* scrollArea;
	QPushButton* addButton;

private:
	enum { rimX = 8, rimY = 11 };

public:
	ScrollFrame(QWidget* parent = nullptr);

	void resizeEvent(QResizeEvent* evnt);


public:
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;

	void setLayout(QLayout* layout);
	QPushButton* get_button();

};



class ComboBoxFrame : public QFrame
{
	Q_OBJECT

private:
	QFrame* innerContent;

private:
	enum { rimX = 0, rimY = 11 };

public:
	QComboBox* monitorBox;

public:
	ComboBoxFrame(QWidget* parent = nullptr);

	void resizeEvent(QResizeEvent* evnt);


public:
	void setLayout(QLayout* layout);
	void addItem(QString key, QVariant value);

	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;


private slots:
	void comboBoxItemChanged_(int itemID);

signals:
	void comboBoxItemChanged(QString& itemName, int& itemID);

};