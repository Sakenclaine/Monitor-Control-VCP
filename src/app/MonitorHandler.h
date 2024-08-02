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
	QString name = "None";
	QString description = "None";

	uint16_t vcp_code = 0x00;
	bool read_only = false, write_only = false;
	bool continous = false;
	bool all_range = false; 

	std::vector<uint16_t> possible_values{0};
	std::vector<QString> possible_values_desc{ "" };
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

	VCP_COMMANDS& possible_values(std::vector<uint16_t> vals);
	VCP_COMMANDS& possible_values_desc(std::vector<QString> valDesc);
	void add_command();

	void add_allowed_values(uint16_t code, std::vector<uint16_t> vals, std::vector<QString> desc);
	void add_allowed_values(uint16_t code, std::initializer_list<uint16_t> codeList, std::initializer_list<QString> descList);

public:
	std::map<QString, VCP_COMM> commands;

public:	
	VCP_COMMANDS();
};

extern VCP_COMMANDS VCP_FEATURES;




struct monitor_vcp
{
	std::vector<uint16_t> possible_values;
	bool enabled = false;
	uint16_t current_value = 0;
};


class Monitor : public QObject
{
	Q_OBJECT

private:
	QString name = "None";
	PHYSICAL_MONITOR monitor_;

	bool dummy = true;
	bool status = false;

public:
	std::map<QString, monitor_vcp> features;

private:
	void get_feature_WIN(uint16_t code);
	void set_feature_WIN(uint16_t code, int value);

	void get_feature_UNIX(uint16_t code);
	void set_feature_UNIX(uint16_t code, int value);

public:
	Monitor(PHYSICAL_MONITOR monitor, QString name);
	Monitor(QString name);
	~Monitor();

public:
	void monitor_init();

	void get_feature(uint16_t code);
	void set_feature(uint16_t code, int value);


	bool get_status();

	QString get_name();


public slots:
	void set_status(bool bval);
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


void get_monitor_capabilities_WIN(PHYSICAL_MONITOR& monitor, std::vector<std::string>& kwrds, std::vector<std::string>& vals, std::map<std::string, std::string>& capabilities_dict);

void get_monitor_features_WIN(std::map<QString, monitor_vcp>& features, PHYSICAL_MONITOR& monitor);

void get_physical_monitors_WIN(std::vector<PHYSICAL_MONITOR>& monitors);

void DumpDevice(const DISPLAY_DEVICE& dd, size_t nSpaceCount);

void list_devices();

