#include <vector>

#include "MonitorHandler.h"
#include "helpers.h"
#include "SignalLinker.h"

///////////////////////////////////// VCP FEATURE SETUP /////////////////////////////////////////////////////

VCP_COMMANDS VCP_FEATURES; // VCP_COMMANDS object, extern object in header file


// Add all commands, which should be supported to the VCP_COMMANDS object
VCP_COMMANDS::VCP_COMMANDS() 
{
    new_command(0x01).set_name("Degauss").write_only().add_command();

    new_command(0x02).set_name("New Control Value").set_desc("Indicates change of register value").add_command();
    add_allowed_values(0x02, { 0x01 }, { "Force Reset" });

    new_command(0x03).set_name("Soft Control").set_desc("Controls Monitor Buttons").read_only().add_command();
    add_allowed_values(0x03, { 0x10, 0x11 }, { "Power", "Brightness UP"});

    new_command(0x04).set_name("Restore Factory Defaults").set_desc("Restore all factory preset").write_only().add_command();

    new_command(0x05).set_name("Restore Factory Luminance/ Contrast Defaults").set_desc("Restores factory defaults for luminance and contrast adjustments.t").write_only().add_command();

    new_command(0x08).set_name("Restore Factory Color Defaults").set_desc("Restore factory defaults for color settings").write_only().add_command();

    new_command(0x0C).set_name("User Color Temperature").add_command(); 

    new_command(0x10).set_name("Luminance").set_desc("Increasing/Decreasing the Luminance").continous().all_range().add_command();
    new_command(0x12).set_name("Contrast").set_desc("Change Contrast").continous().all_range().add_command();

    new_command(0x14).set_name("Select Color Preset").add_command();
    add_allowed_values(0x14, 
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D }, 
        { "sRGB", "Native", "4000K", "5000K", "6500K", "7500K", "8200K", "9300K", "10000K", "11500K", "User 1", "User 2", "User 3"});

    new_command(0x16).set_name("Video Gain (Drive): Red").set_desc("Increasing (decreasing) this value will increase (decrease) the luminance of red pixels.").continous().all_range().add_command();

    new_command(0x17).set_name("User Color Vision Compensation").add_command();
    
    new_command(0x18).set_name("Video Gain (Drive): Green").set_desc("Increasing (decreasing) this value will increase (decrease) the luminance of green pixels.").continous().all_range().add_command();
    new_command(0x1A).set_name("Video Gain (Drive): Blue").set_desc("Increasing (decreasing) this value will increase (decrease) the luminance of blue pixels.").continous().all_range().add_command();
  
    //add_command("Focus", "Increasing (decreasing) this value will adjust the focus of the image.", 0x1C, false, false);
  
    new_command(0x1E).set_name("Auto Setup").set_desc("Perform auto setup function").add_command();
    add_allowed_values(0x1E, { 0x00, 0x01, 0x02 }, { "Not Active", "Perform/performing auto setup", "Enable continuous / periodic auto setup" });

    new_command(0x1F).set_name("Auto Color Setup").set_desc("Perform auto color setup function").add_command();
    add_allowed_values(0x1F, { 0x00, 0x01, 0x02 }, { "Not Active", "Perform/performing auto setup", "Enable continuous / periodic auto setup" });

    new_command(0x52).set_name("Active Control").read_only().add_command();

    new_command(0x60).set_name("Input Select").add_command();
    add_allowed_values(0x60, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12 }, {"Analog Video 1", "Analog Video 2", "DVI 1", "DVI 2", "Composite Video 1", "Composite Video 2", "S-Video 1", "S-Video 2", "Tuner 1", "Tuner 2", "Tuner 3", "Component Video 1", "Component Video 2", "Component Video 3", "Display Port 1", "Display Port 2", "HDMI 1", "HDMI 2"});

    new_command(0x62).set_name("Speaker Volume").continous().all_range().add_command();
    add_allowed_values(0x62, { 0x00, 0xFF }, { "Fixed (Default) level", "Mute" });

    ////add_command("Display Scaling", 0x86, false, false);
    ////add_allowed_values(0x86, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A }, { "No Scaling", "Max Image", "Max Vt 1", "Max Hz 1", "Max Vt 2", "Max Hz 2", "Full Mode", "Zoom Mode", "Squeeze Mode", "Variable" });

    ////add_command("Horizontal Frequency", 0xAC, true, false);
    ////add_command("Vertical Frequency", 0xAE, true, false);

    ////add_command("Flat Panel Sub-Pixel Layout", "Indicates the type of LCD sub-pixel structure.", 0xB2, true, false);
    ////add_allowed_values(0xB2, { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 }, { "Not Defined", "RGB Vertical Stripe", "RGB Horizontal Stripe", "BGR Vertical Stripe", "BGR Horizontal Stripe", "Quad-Pixel 1", "Quad-Pixel 2", "Delta", "Mosaic" });

    ////add_command("Display Technology Type", "Indicates the base technology type.", 0xB6, true, false);
    ////add_allowed_values(0xB6, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }, { "CRT (shadow mask)", "CRT (aperture grill)", "LCD", "LCoS", "Plasma", "OLED", "EL", "Dynamic MEM", "Static MEM" });
}


VCP_COMMANDS& VCP_COMMANDS::new_command(uint16_t code)
{
    comm.vcp_code = code;
    return *this;
}

VCP_COMMANDS& VCP_COMMANDS::set_name(QString name)
{
    comm.name = name;
    return *this;
}

VCP_COMMANDS& VCP_COMMANDS::set_desc(QString desc)
{
    comm.description = desc;
    return *this;
}

VCP_COMMANDS& VCP_COMMANDS::write_only()
{
    comm.write_only = true;

    return *this;
}

VCP_COMMANDS& VCP_COMMANDS::read_only()
{
    comm.read_only = true;

    return *this;
}

VCP_COMMANDS& VCP_COMMANDS::continous()
{
    comm.continous = true;

    return *this;
}

VCP_COMMANDS& VCP_COMMANDS::all_range()
{
    comm.all_range = true;

    return *this;
}

VCP_COMMANDS& VCP_COMMANDS::possible_values(std::initializer_list<uint16_t> vals, std::initializer_list<QString> descs)
{
    QList keys = vals;
    QList elems = descs;
    
    for (size_t i = 0; i < keys.size(); i++)
    {       
        comm.possible_values[n2hexstr(keys.at(i), 2)] = elems.at(i);
    }

    return *this;
}

void VCP_COMMANDS::add_command()
{
    QString str_code = n2hexstr(comm.vcp_code, 2);
    commands[str_code] = comm;
}


void VCP_COMMANDS::add_allowed_values(uint16_t code, std::initializer_list<uint16_t> codeList, std::initializer_list<QString> descList)
{
    QString cde_str = n2hexstr(code, 2);
    
    QList keys = codeList;
    QList elems = descList;

    if (commands.contains(cde_str))
    {
        for (size_t i = 0; i < keys.size(); i++)
        {
            commands[cde_str].possible_values[n2hexstr(keys.at(i), 2)] = elems.at(i);
        }
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// BEGIN --------------------------------------------------------------
// Functions for getting physical monitors and reading as well as parsing their capability string
// Get physical monitors connected to the device
void get_physical_monitors_WIN(std::vector<PHYSICAL_MONITOR>& monitors)
{
    MonitorRects monitorStruct;

    std::vector<HMONITOR> monitors_ = monitorStruct.monitorHandles;
    std::vector<HANDLE> unique_handles;

    for (auto& hmonitor : monitors_)
    {
        DWORD countPhysicalMonitors;
        LPPHYSICAL_MONITOR pPhysicalMonitors = NULL;

        bool bChk;
        bChk = GetNumberOfPhysicalMonitorsFromHMONITOR(hmonitor, &countPhysicalMonitors);

        if (bChk)
        {
            qDebug() << "Number of physical monitors per HMONITOR: " << countPhysicalMonitors;

            // Allocate the array of PHYSICAL_MONITOR structures.
            pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(countPhysicalMonitors * sizeof(PHYSICAL_MONITOR));

            if (pPhysicalMonitors != NULL)
            {
                bChk = GetPhysicalMonitorsFromHMONITOR(hmonitor, countPhysicalMonitors, pPhysicalMonitors);

                if (bChk)
                {
                    for (int i = 0; i < static_cast<int>(countPhysicalMonitors); i++)
                    {
                        std::wstring name = pPhysicalMonitors[i].szPhysicalMonitorDescription;
                        HANDLE temp_handle = pPhysicalMonitors[i].hPhysicalMonitor;

                        if (std::find(unique_handles.begin(), unique_handles.end(), temp_handle) == unique_handles.end())
                        {
                            monitors.push_back(pPhysicalMonitors[i]);
                            unique_handles.push_back(temp_handle);
                        }
                    }
                }
            }
            // Free the array.
            free(pPhysicalMonitors);
        }
    }
}

bool get_connected_monitors(QList<Monitor*>& monitors)
{

#if defined(Q_OS_WIN) && !defined(QT_DEBUG)
    std::vector<PHYSICAL_MONITOR> mons;
    get_physical_monitors_WIN(mons);

    if (mons.empty()) return false;

    for (auto& elem_mon : mons)
    {
        QString name = QString::fromWCharArray(elem_mon.szPhysicalMonitorDescription);
        Monitor* mon = new Monitor(elem_mon, name);

        mon->init();

        monitors.append(mon);
    }

    return true;

#elif defined(QT_DEBUG)
    Monitor* mon1 = new Monitor("Monitor 1");
    mon1->init();
    monitors.append(mon1);

    Monitor* mon2 = new Monitor("Monitor 2");
    mon2->init();
    monitors.append(mon2);

    return true;

#endif
}



void get_monitor_features_WIN(QMap<QString, monitor_vcp>& features, PHYSICAL_MONITOR& monitor)
{
    auto get_monitor_capabilities_WIN = [](PHYSICAL_MONITOR& monitor, std::vector<std::string>& kwrds, std::vector<std::string>& vals, std::map<std::string, std::string>& capabilities_dict)
        {
            // Get physical monitor handle.
            HANDLE hPhysicalMonitor = monitor.hPhysicalMonitor;
            DWORD capLength = 0;

            bool bChk = GetCapabilitiesStringLength(hPhysicalMonitor, &capLength);

            if (bChk)
            {
                // Allocate the string buffer.
                LPSTR szCapabilitiesString = (LPSTR)malloc(capLength);
                if (szCapabilitiesString != NULL)
                {
                    // Get the capabilities string.
                    bChk = CapabilitiesRequestAndCapabilitiesReply(
                        hPhysicalMonitor,
                        szCapabilitiesString,
                        capLength);

                    if (bChk) parse_capability_string(szCapabilitiesString, kwrds, vals, capabilities_dict);
                }

                // Free the string buffer.
                free(szCapabilitiesString);
            }
        };

    std::vector<std::string> kwrds, vals;
    std::map<std::string, std::string> capabilities_dict;

    get_monitor_capabilities_WIN(monitor, kwrds, vals, capabilities_dict);

    std::map<std::string, std::string>::iterator it = capabilities_dict.begin();

    std::vector<std::string> vcp_split = split(capabilities_dict["vcp_only"], " ");
    std::vector<std::string> vcp_with_options = split(capabilities_dict["vcps_with_options"], " ");

    for (auto& elem : vcp_split)
    {
        QString elem_ = QString::fromStdString(elem);

        if (VCP_FEATURES.commands.contains(elem_))
        {
            struct monitor_vcp mon_feature;
            mon_feature.enabled = true;

            if (std::find(vcp_with_options.begin(), vcp_with_options.end(), elem) != vcp_with_options.end())
            {
                for (auto& el : split(capabilities_dict[elem], " "))
                {
                    if (!(el == " " || el == "")) {
                        uint16_t ul = std::stoul("0x" + el, nullptr, 0);

                        if (!(VCP_FEATURES.commands[elem_].possible_values.empty()))
                        {
                            if (VCP_FEATURES.commands[elem_].possible_values.contains(QString::fromStdString(el)))
                            {
                                mon_feature.possible_values.push_back(ul);
                            }
                        }
                    }
                }
            }

            // Set current value in Monitor Class to actual current value of monitor
            unsigned long current_value = 0;
            unsigned long max_value = 0;
            MC_VCP_CODE_TYPE code_type;

            uint16_t cde = elem_.toUInt();

            bool bRet = GetVCPFeatureAndVCPFeatureReply(monitor.hPhysicalMonitor, cde, &code_type, &current_value, &max_value);
            qDebug() << "Current Value (" << int(cde) << "): " << current_value;

            if (bRet) mon_feature.current_value = current_value;

            features[elem_] = mon_feature;
        }
    }
}

// END ----------------------------------------------------------------


// BEGIN --------------------------------------------------------------
// Constructors and Desctructor for Monitor class
int Monitor::idProvider = 0;

Monitor::Monitor(PHYSICAL_MONITOR monitor, QString name) :
    monitor_(monitor),
    name(name),
    _id(++idProvider)
{
    dummy = false;
}

Monitor::Monitor(QString name) :
    name(name),
    _id(++idProvider)
{
}

Monitor::~Monitor()
{

}


void Monitor::init()
{
    if (!dummy)
    {
#ifdef Q_OS_WIN
        get_monitor_features_WIN(features, monitor_);

#elif Q_OS_UNIX
        qDebug() << "No UNIX Handling yet.";

#endif

    }

    else
    {
        monitor_vcp t1;
        t1.enabled = true;
        t1.current_value = 10;

        features["10"] = t1;

        monitor_vcp contrast;
        contrast.enabled = true;
        contrast.current_value = 50;

        features["12"] = contrast;

        monitor_vcp clrProfile;
        clrProfile.enabled = true;
        clrProfile.possible_values = QList<uint16_t>{ 0x01, 0x02, 0x03 };
        clrProfile.current_value = 0x01;

        features["14"] = clrProfile;

        monitor_vcp monInput;
        monInput.enabled = true;
        monInput.possible_values = QList<uint16_t>{ 0x01, 0x02, 0x03 };
        monInput.current_value = 0x02;

        features["60"] = monInput;
    }

}

int Monitor::get_ID()
{
    return _id;
}

QString Monitor::get_name()
{
    return name;
}

bool Monitor::get_status()
{
    return status;
}


// SLOTS
void Monitor::set_status(bool chk)
{
    if (chk != status) { 
        emit send_status(chk);
        status = chk; 
        Linker::getInstance().receive_lock(chk);
    }
}





