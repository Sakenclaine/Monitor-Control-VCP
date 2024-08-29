#pragma once

#pragma comment(lib, "Dxva2.lib")
#pragma comment(lib, "user32.lib")

#include <Windows.h>
#include <WinUser.h>
#include <lowlevelmonitorconfigurationapi.h>
#include <highlevelmonitorconfigurationapi.h>
#include <physicalmonitorenumerationapi.h>





#include <QtCore>
#include <QString>
#include <QObject>
#include <QMap>
#include <QList>
#include <QDebug>


#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <initializer_list>





class Monitor;




struct VCP_COMM
{
	QString name = "None";
	QString description = "None";

	uint16_t vcp_code = 0x00;
	uint16_t min = 0, max = 0xFF;

	bool read_only = false, write_only = false;
	bool continous = false;
	bool all_range = false;

	QMap<QString, QString> possible_values;
};



//https://milek7.pl/ddcbacklight/mccs.pdf
class VCP_COMMANDS
{
private:
	VCP_COMM comm;

public:
	VCP_COMMANDS& new_command(uint16_t code);
	VCP_COMMANDS& set_name(QString name);
	VCP_COMMANDS& set_desc(QString desc);
	VCP_COMMANDS& write_only();
	VCP_COMMANDS& read_only();
	VCP_COMMANDS& continous();
	VCP_COMMANDS& all_range();

	VCP_COMMANDS& possible_values(std::initializer_list<uint16_t> vals, std::initializer_list<QString> descs);
	void add_command();

	void add_allowed_values(uint16_t code, std::initializer_list<uint16_t> codeList, std::initializer_list<QString> descList);


public:
	QMap<QString, VCP_COMM> commands;

public:	
	VCP_COMMANDS();
};

extern VCP_COMMANDS VCP_FEATURES;



struct MonitorRects
{
	std::vector<HMONITOR> monitorHandles;

	static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
	{
		MonitorRects* pThis = reinterpret_cast<MonitorRects*>(pData);
		pThis->monitorHandles.push_back(hMon);

		return TRUE;
	}

	MonitorRects()
	{
		EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
	}
};


// Methods to get (physical) monitors in Windows
bool get_connected_monitors(QList<Monitor*>& monitors);



struct monitor_vcp
{
	QList<uint16_t> possible_values;
	bool enabled = false;

	uint16_t current_value = 0;
	uint16_t max_value = 255;
};



// Monitor Class
class Monitor : public QObject
{
	Q_OBJECT

private:
	QString name = "None";
	PHYSICAL_MONITOR monitor_;

	bool dummy = true;
	bool status = false;

	int _id;
	static int idProvider;


public:
	QMap<QString, monitor_vcp> features;


public:
	Monitor(PHYSICAL_MONITOR monitor, QString name);
	Monitor(QString name);
	~Monitor();

public:
	void init();

	int get_ID();
	QString get_name();
	bool get_status();

	bool add_check_feature(uint16_t code, bool addChk);


public slots:
	void set_status(bool chk);

	//void set_discrete_feature(uint16_t code, int value);
	bool set_feature(uint16_t code, int value);

signals:
	void send_status(bool chk);

};


void DumpDevice(const DISPLAY_DEVICE& dd, size_t nSpaceCount);

void list_devices();

