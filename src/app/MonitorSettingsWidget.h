#pragma once
#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QScrollArea>
#include <QScrollBar>
#include <QList>
#include <QFont>

#include "MonitorHandler.h"
#include "CustomSlider.h"



class CustomFrame : public QFrame
{
	Q_OBJECT

private:
	QWidget* innerContent;
	QScrollArea* scrollArea;
	QPushButton* addButton;

private:
	enum { rimX = 8, rimY = 11 };

public:
	CustomFrame(QWidget* parent = nullptr) : QFrame(parent) 
	{
		//setFrameStyle(QFrame::Panel);

		innerContent = new QWidget(this);

		scrollArea = new QScrollArea(this);
		scrollArea->setBackgroundRole(QPalette::Dark);
		scrollArea->setWidgetResizable(true);
		scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scrollArea->verticalScrollBar()->setEnabled(false);
		scrollArea->setWidget(innerContent);

		scrollArea->resize(QSize(width() - rimX, height() - rimY));
		scrollArea->move(rimX, rimY);

		QFont btnFont = QFont("Arial", 15);
		btnFont.setBold(true);

		addButton = new QPushButton("+", this);
		addButton->setFont(btnFont);
		addButton->setStyleSheet(QString("QPushButton{background-color: rgb(150, 150, 150); padding: 0px; min-width: 1.25em; min-height: 0.5em; border-radius: 0.4em;} QPushButton::hover{background-color: rgb(128, 128, 128); padding: 0px; min-width: 1.5em; min-height: 1em; border-radius: 0.65em;}"));
		addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
		addButton->move(0, 0);

		addButton->show();	
	}

	void resizeEvent(QResizeEvent* evnt)
	{	
		scrollArea->resize(QSize(evnt->size().width() - rimX, evnt->size().height() - rimY));

		updateGeometry();
		innerContent->updateGeometry();
		scrollArea->updateGeometry();
		
		//QList<QWidget*> widgets = this->findChildren<QWidget*>();
		//foreach (QWidget* elem, widgets) updateGeometry();
	}


public:
	void setLayout(QLayout* layout)
	{
		layout->setContentsMargins(0, 4, 10, 0);
		innerContent->setLayout(layout);
	}

	QSize sizeHint() const override
	{
		return innerContent->sizeHint();
	}

	QSize minimumSizeHint() const override
	{
		return QSize(innerContent->minimumSizeHint().width(), innerContent->minimumSizeHint().height() + 20);
	}
	

public:
	QPushButton* get_button()
	{
		return addButton;
	}


};


class MonitorWidget : public QWidget
{
	Q_OBJECT

private:
	Monitor* monitor = nullptr;
	QMap<QString, CustomSlider*> customSliders;

public:
	MonitorWidget();


private:
	QGroupBox* settings_discrete;
	QGroupBox* settings_continous;

	QScrollArea* scrollArea;
	CustomFrame* settingsFrame;

	QHBoxLayout* hSliderLayout;
	QHBoxLayout* settings_continous_layout;


public:
	void add_slider(uint16_t code, bool btrayIcon);
	void add_slider(uint16_t code, QColor color, bool btrayIcon);
	void add_contextMenu(QMenu* menu);

};


class PlaceholderWidget : public QWidget
{
	Q_OBJECT

public:
	PlaceholderWidget();
};
