#pragma once
#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>

#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QScrollArea>
#include <QScrollBar>

#include "MonitorHandler.h"
#include "CustomSlider.h"








class CustomFrame : public QFrame
{
	Q_OBJECT

private:
	QFrame* innerFrame;

	QScrollArea* scrollArea;
	QPushButton* add_button;

	QHBoxLayout* mainLayout;

private:
	enum { rimX = 0, rimY = 10 };

public:
	CustomFrame(QWidget* parent = nullptr) : QFrame(parent) 
	{
		//setFrameStyle(QFrame::Panel);
		//setFocusPolicy(Qt::StrongFocus);
		//setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		
		scrollArea = new QScrollArea();
		scrollArea->setBackgroundRole(QPalette::Dark);
		scrollArea->setWidgetResizable(true);
		scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scrollArea->verticalScrollBar()->setEnabled(false);

		add_button = new QPushButton("Add", this);
		add_button->show();

		innerFrame = new QFrame(this);
		innerFrame->setFrameShape(QFrame::Panel);
		
		mainLayout = new QHBoxLayout();
		mainLayout->setContentsMargins(0, 10, 0, 0);

		innerFrame->setLayout(mainLayout);

		mainLayout->addWidget(scrollArea);

		int x, y, xW, yW;
		QRect outer_frame = frameRect();
		outer_frame.getRect(&x, &y, &xW, &yW);

		int newX = xW - 2 * rimX;
		int newY = yW - 2 * rimY;

		if (newX < 0) newX = 0;
		if (newY < 0) newY = 0;

		innerFrame->setMinimumSize(newX, newY);
		innerFrame->setMaximumSize(newX, newY);
		innerFrame->move(x + rimX, y + rimY);

		int x_, y_, xW_, yW_;
		QRect inner_frame = innerFrame->frameRect();
		inner_frame.getRect(&x_, &y_, &xW_, &yW_);

		innerFrame->show();
	}

	void resizeEvent(QResizeEvent* evnt)
	{
		QSize outerSize = evnt->size();

		int newX = outerSize.width() - 2 * rimX;
		int newY = outerSize.height() - 1 * rimY;

		if (newX <= rimX) 
		{ 
			setMinimumWidth(2 * rimX + 5);
			newX = 5;
		}

		if (newY <= rimY)
		{
			setMinimumHeight(2 * rimY + 5);
			newY = 5;
		}
		
		innerFrame->setMinimumSize(newX, newY);
		innerFrame->setMaximumSize(newX, newY);
	}



public:
	void setLayout(QLayout* layout)
	{
		scrollArea->setLayout(layout);

		//layout->setSpacing(0);
		//layout->setContentsMargins(0, 0, 0, 0);
	}

	//QSize sizeHint() const
	//{
	//	return QSize(innerFrame->width(), innerFrame->height());
	//}

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
