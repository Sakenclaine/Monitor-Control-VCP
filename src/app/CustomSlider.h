#pragma once
#include <QtCore>
#include <QSlider>
#include <QWidget>
#include <QString>
#include <QColor>
#include <QCheckBox>
#include <QMenu>

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QVBoxLayout;

class QPushButton;
class TrayIconControlled;
class CustomSlider;
QT_END_NAMESPACE

class CustomSlider : public QSlider
{
	Q_OBJECT

private:
	bool locked = false;
	QString receiverType = "slider";

public:
	CustomSlider(QWidget* parent = nullptr);

	void set_style(QString& style);


public slots:
	void set_lock(bool lock);
	void lock();
	void unlock();

	void set_value(int value);
	void update_value(int& val, QObject& senderObj);


protected:
	void wheelEvent(QWheelEvent* e);

	bool event(QEvent* e);
};



class SliderWidget : public QWidget
{
	Q_OBJECT

public:
	SliderWidget(QWidget* parent, uint16_t code);
	SliderWidget(QWidget* parent, QColor color, uint16_t code);

	~SliderWidget();

private:
	int _id;
	static int idProvider;
	bool trayIcon = false;
	bool locked = false;

	uint16_t code = 0x00;
	QString name = "";
	QString cde_str = "";
	QColor color = QColor("pink");

	QString width = QString::number(0);
	QString handle = "#1795f6";
	QString lower_fill = "pink";
	QString upper_fill = "gray";


	QHBoxLayout* mainLayout;
	QVBoxLayout* mainVLayout;
	QHBoxLayout* mainHLayout;
	QVBoxLayout* buttonLayout;

	TrayIconControlled* icon;
	CustomSlider* slider;
	QCheckBox* trayChk;
	QPushButton* deleteBtn;

private:
	void setup();

public:
	void set_color(QColor col);
	void set_value(int value);
	void add_trayIcon();
	void set_contextMenu(QMenu* menu);

	CustomSlider* get_slider();
	TrayIconControlled* get_icon();

	const QColor& get_color();
	const int& get_ID();
	const bool& get_trayCheck();
	const uint16_t& get_code();

private slots:
	void buttonClick();
	void toggleTray(bool chk);

	void update_ToolTip(int val);

	void slider_changed();

	void intern_delete_slider();

public slots:
	void set_lock(bool lock);
	void lock();
	void unlock();

signals:
	void value_changed(QString&, int);
	void delete_slider();

};