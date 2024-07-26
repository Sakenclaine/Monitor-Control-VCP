#pragma once

#include <QSystemTrayIcon>

#include "RawData.h"


class TrayIconControlled : public QSystemTrayIcon
{
	Q_OBJECT
public:
	TrayIconControlled(const QIcon& icon, QObject* parent, int initial_value, QColor color, int min_val, int max_val, int step);
	TrayIconControlled(QObject* parent, int initial_value, QColor color, int min_val, int max_val, int step);
	
	virtual ~TrayIconControlled();

	int get_value();


private:
	int receiver_id;
	QString receiverType = tr("trayIcon");

	int value;
	int min_val, max_val, step;

	QColor font_color;

	void update_icon();

public slots:
	void update_value(int&, QObject&);
	void mouse_over(const struct inSignal&);

signals:
	void value_changed(int& value);
};
