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



class CustomFrame2 : public QWidget
{
	Q_OBJECT

private:
	QFrame* frame;
	QHBoxLayout* mainLayout;

	QScrollArea* scrollArea;


private:
	enum { rimX = 0, rimY = 20 };

public:
	CustomFrame2(QWidget* parent = nullptr) : QWidget(parent)
	{
		mainLayout = new QHBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(mainLayout);
		
		frame = new QFrame();
		frame->setFrameStyle(QFrame::Panel);


		QImage image(":/images/placeholder.png");
		QLabel* imageLabel = new QLabel;
		imageLabel->setBackgroundRole(QPalette::Base);
		//imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		imageLabel->setScaledContents(true);
		imageLabel->setPixmap(QPixmap::fromImage(image));
		imageLabel->resize(0.5 * imageLabel->pixmap(Qt::ReturnByValue).size());


	
		scrollArea = new QScrollArea();
		scrollArea->setBackgroundRole(QPalette::Dark);
		//scrollArea->setWidgetResizable(true);
		//scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		//scrollArea->verticalScrollBar()->setEnabled(false);
		scrollArea->setWidget(imageLabel);


		mainLayout->addWidget(scrollArea);
	}

};



class CustomFrame : public QFrame
{
	Q_OBJECT

private:
	QWidget* innerContent;
	QScrollArea* scrollArea;
	QPushButton* addButton;

private:
	enum { rimX = 0, rimY = 11 };

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

		scrollArea->resize(QSize(width(), height() - rimY));
		scrollArea->move(0, rimY);

		QFont btnFont = QFont("Arial", 17);
		btnFont.setBold(true);

		addButton = new QPushButton("+", this);
		addButton->setFont(btnFont);
		addButton->setStyleSheet(QString("QPushButton{background-color: rgb(150, 150, 150); padding: 0px; min-width: 1.5em; min-height: 1em; border-radius: 0.65em;} QPushButton::hover{background-color: rgb(128, 128, 128); padding: 0px; min-width: 1.5em; min-height: 1em; border-radius: 0.65em;}"));
		addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
		addButton->move(15, 0);

		addButton->show();	
	}

	void resizeEvent(QResizeEvent* evnt)
	{	
		scrollArea->resize(QSize(evnt->size().width(), evnt->size().height() - rimY));

		updateGeometry();
		innerContent->updateGeometry();
		scrollArea->updateGeometry();
		
		//QList<QWidget*> widgets = this->findChildren<QWidget*>();
		//foreach (QWidget* elem, widgets) updateGeometry();
	}


public:
	void setLayout(QLayout* layout)
	{
		layout->setContentsMargins(0, 11, 0, 0);
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
