#pragma once
#include <QSystemTrayIcon>

struct mouseSignal;


class TrayIconControlled : public QSystemTrayIcon
{
	Q_OBJECT

public:
	TrayIconControlled(QObject* parent, int initial_value, QColor color, int min_val, int max_val, int step);
	
	~TrayIconControlled();

public:
	int get_value();

	void set_identifier(int id, QString code);
	void set_step(int new_step);
	void set_color(QColor);

private:
	int receiver_id;
	QString receiver_code;
	QString receiverType = "trayIcon";

	QColor font_color;

	int value;
	int min_val, max_val, step;

	bool locked = false;

private:
	void update_icon();

public slots:
	void set_lock(bool lock);
	void lock();
	void unlock();

	void update_value(int&, QObject&);
	void mouse_over(const struct mouseSignal&);

signals:
	void value_changed(int& value);
};
