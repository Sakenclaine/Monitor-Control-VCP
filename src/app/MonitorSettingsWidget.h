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
#include <QComboBox>
#include <QAbstractItemView>
#include <QStackedWidget>

#include "MonitorHandler.h"
#include "CustomSlider.h"

#include <QSlider>


class Slider : public QSlider
{
	Q_OBJECT

private:
	bool locked = false;

public:
	Slider(QWidget* parent = nullptr) : QSlider(parent) {
		setTracking(false);
	};

private slots:
	void check_move(int& val) {
		qDebug() << val;
	}

public slots:
	void set_lock(bool lock) {
		locked = lock;
	}

	void lock() {
		locked = true;
	}

	void unlock() {
		locked = false;
	}

	void set_value(int value) {
		if (!locked)
		{
			setValue(value);
		}
	}

	void wheelEvent(QWheelEvent* e) {
		if (!locked) QSlider::wheelEvent(e);
	}

	void keyPressEvent(QKeyEvent* e) {
		if (!locked) QSlider::keyPressEvent(e);
	}

	void timerEvent(QTimerEvent* e) {
		qDebug() << e;
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

class ComboBoxFrame : public QFrame
{
	Q_OBJECT

private:
	QFrame* innerContent;
	QScrollArea* scrollArea;

private:
	enum { rimX = 0, rimY = 11 };

public:
	QComboBox* monitorBox;

public:
	ComboBoxFrame(QWidget* parent = nullptr) : QFrame(parent)
	{
		//setFrameStyle(QFrame::Panel);

		innerContent = new QFrame(this);
		innerContent->setFrameStyle(QFrame::Panel);
		innerContent->resize(QSize(width() - rimX, height() - rimY));
		innerContent->move(rimX, rimY);
		innerContent->show();

		monitorBox = new QComboBox(this);
		monitorBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		monitorBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
		monitorBox->move(0, 0);
		monitorBox->setWindowOpacity(0);
		
		monitorBox->show();

		connect(monitorBox, &QComboBox::currentIndexChanged, this, &ComboBoxFrame::comboBoxItemChanged_);
	}

	void resizeEvent(QResizeEvent* evnt)
	{
		innerContent->resize(QSize(evnt->size().width() - rimX, evnt->size().height() - rimY));

		updateGeometry();
		innerContent->updateGeometry();
	}


public:
	void setLayout(QLayout* layout)
	{
		layout->setContentsMargins(0, 4, 10, 0);
		innerContent->setLayout(layout);
		monitorBox->adjustSize();
	}

	QSize sizeHint() const override
	{
		return innerContent->sizeHint();
	}

	QSize minimumSizeHint() const override
	{
		return QSize(innerContent->minimumSizeHint().width(), innerContent->minimumSizeHint().height() + 20);
	}


private slots:
	void comboBoxItemChanged_(int itemID)
	{
		//qDebug() << "Item Changed: " << itemID << " --> " << monitorBox->currentText();

		QString temp = monitorBox->currentText();
		emit comboBoxItemChanged(temp, itemID);
	}

signals:
	void comboBoxItemChanged(QString& itemName, int& itemID);

};

class MonitorWidget : public QWidget
{
	Q_OBJECT

private:
	QMap<QString, CustomSlider*> customSliders;

public:
	MonitorWidget();


private:
	ComboBoxFrame* settings_discrete;

	QScrollArea* scrollArea;
	CustomFrame* settingsFrame;

	QHBoxLayout* settings_continous_layout;
	QVBoxLayout* discreteLayout;

	QStackedWidget* stackedWidget;




private:
	void setup_discrete_settings();
	void init_slider(QString name);


public:
	void add_slider(uint16_t code, bool btrayIcon);
	void add_slider(uint16_t code, QColor color, bool btrayIcon);
	void add_contextMenu(QMenu* menu);
	void chk_add_discrete_feature(QString monName, QString qsft);
	void chk_add_discrete_feature(Monitor* mon, QString qsft);



public slots:
	void receive_add_slider(uint16_t&, QColor&, bool&);
	void receive_slider_init(Monitor* monitor);

private slots:
	void cb_monitor_change(QString& name, int& ids);

	void discrete_setting_changed(int index);

signals:
	void send_discrete_setting(int& monitorID, QString& cde_str, uint16_t& value);
};




class PlaceholderWidget : public QWidget
{
	Q_OBJECT

public:
	PlaceholderWidget();
};
