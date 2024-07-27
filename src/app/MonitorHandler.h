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


#include <vector>
#include <string>
#include <map>
#include <initializer_list>

#include "helpers.h"


struct VCP_COMM
{
	QString name;
	QString description;

	uint16_t vcp_code;
	bool read_only, write_only; // , enable;

	std::vector<uint16_t> possible_values;
	std::vector<std::string> possible_values_desc;
	//std::vector<bool> possible_values_status;
};

struct monitor_vcp
{
	std::vector<uint16_t> possible_values;
	bool enabled = false;
};


//https://milek7.pl/ddcbacklight/mccs.pdf
class VCP_COMMANDS
{
private:
	void add_command(QString name, QString desc, uint16_t code, bool read, bool write);
	void add_command(QString name, uint16_t code, bool read, bool write);
	void add_allowed_values(uint16_t code, std::vector<uint16_t> vals, std::vector<std::string> desc);
	void add_allowed_values(uint16_t code, std::initializer_list<uint16_t> codeList, std::initializer_list<std::string> descList);

public:
	std::map<std::string, VCP_COMM> commands;

public:	
	VCP_COMMANDS();
};

extern VCP_COMMANDS VCP_FEATURES;


class Monitor : public QObject
{
	Q_OBJECT

private:
	QString name = tr("None");
	//PHYSICAL_MONITOR monitor ;
	//LPPHYSICAL_MONITOR monitor_ = nullptr;
	PHYSICAL_MONITOR monitor_;
	bool dummy = true;

	int curr_brightness = 0;
	int curr_contrast = 0;
	int curr_r = 0;
	int curr_g = 0;
	int curr_b = 0;

	bool status = false;

	//VCP_COMMANDS features;
	std::map<std::string, monitor_vcp> features;

public:
	//Monitor(PHYSICAL_MONITOR& monitor, QString name, bool enabled);
	//Monitor(PHYSICAL_MONITOR* monitor, QString name, bool status);
	Monitor(PHYSICAL_MONITOR monitor, QString name, bool status);
	Monitor(QString name, bool status);
	~Monitor();

	void monitor_init();
	void get_feature(uint16_t code);
	void set_feature(uint16_t code, int value);

	QString get_name();
	bool get_enabled();

	int get_brightness();
	int get_contrast();
	int get_R();
	int get_G();
	int get_B();


public slots:
	void set_enabled(bool status);
	void receive_signal(uint16_t code, int value);
	void receive_value_request(uint16_t code);

signals:
	void send_status(const bool&);
	
};


struct MonitorRects
{
	//std::vector<RECT> rcMonitors;
	std::vector<HMONITOR> monitorHandles;

	static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
	{
		MonitorRects* pThis = reinterpret_cast<MonitorRects*>(pData);
		//pThis->rcMonitors.push_back(*lprcMonitor);
		pThis->monitorHandles.push_back(hMon);

		return TRUE;
	}

	MonitorRects()
	{
		EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
	}
};


void get_physical_monitors(std::vector<PHYSICAL_MONITOR>& monitors);


void DumpDevice(const DISPLAY_DEVICE& dd, size_t nSpaceCount);


void list_devices();

