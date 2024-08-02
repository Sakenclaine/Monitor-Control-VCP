#pragma once

#include <QSystemTrayIcon>

#include "RawData.h"


class TrayIconControlled : public QSystemTrayIcon
{
	Q_OBJECT

public:
	TrayIconControlled(QObject* parent, int initial_value, QColor color, int min_val, int max_val, int step);
	
	~TrayIconControlled();

public:
	int get_value();
	void set_step(int new_step);

private:
	int receiver_id;
	QString receiver_code;
	QString receiverType = "trayIcon";

	QColor font_color;

	int value;
	int min_val, max_val, step;

private:
	void update_icon();

public slots:
	void update_value(int&, QObject&);
	void mouse_over(const struct inSignal&);

signals:
	void value_changed(int& value);
};
