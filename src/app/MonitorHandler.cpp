#include <vector>

#include "MonitorHandler.h"
#include "helpers.h"
#include "SignalLinker.h"
#include "utilities.h"

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
bool get_connected_monitors(QList<Monitor*>& monitors)
{

#if defined(Q_OS_WIN) && !defined(QT_DEBUG)
    QMap<int, QList<QString>> metadata;

    displayDevices(metadata);

    std::vector<PHYSICAL_MONITOR> mons;
    get_physical_monitors_WIN(mons);

    if (mons.empty()) return false;

    qDebug() << "\n";

    int i = 0;

    for (auto& elem_mon : mons)
    {
        QString name = QString::fromWCharArray(elem_mon.szPhysicalMonitorDescription);
        Monitor* mon = new Monitor(elem_mon, metadata[i][0]);

        qDebug() << "Monitor " << name << "( " << metadata[i][0] << ")" << " init ...";
        mon->init();

        monitors.append(mon);

        i++;
    }

    qDebug() << "\n\n";

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


// Get physical monitors connected to the device
void get_physical_monitors_WIN(std::vector<PHYSICAL_MONITOR>& monitors)
{
    MonitorRects monitorStruct;

    std::vector<HMONITOR> monitors_ = monitorStruct.monitorHandles;
    std::vector<HANDLE> unique_handles;

    qDebug() << "Found (" << monitors_.size() << ") HMONITORs";

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

                        qDebug() << "Monitor Name: " << name;

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




void get_monitor_features_WIN(QMap<QString, monitor_vcp>& features, PHYSICAL_MONITOR& monitor)
{
    auto get_monitor_capabilities_WIN = [](
        PHYSICAL_MONITOR& monitor, 
        std::vector<std::string>& kwrds, 
        std::vector<std::string>& vals, 
        std::map<std::string, std::string>& capabilities_dict
        )

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

                    qDebug() << "------\n" << szCapabilitiesString << "\n------\n";
                }

                // Free the string buffer.
                free(szCapabilitiesString);
            }
        };

    std::vector<std::string> kwrds, vals;
    std::map<std::string, std::string> capabilities_dict;

    get_monitor_capabilities_WIN(monitor, kwrds, vals, capabilities_dict);

    std::map<std::string, std::string>::iterator it = capabilities_dict.begin();

    for (const auto& [k, v] : capabilities_dict) qDebug() << k << " :  " << v;

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
            

            uint16_t cde = std::stoul("0x" + elem, nullptr, 0);

            bool bRet = GetVCPFeatureAndVCPFeatureReply(monitor.hPhysicalMonitor, cde, &code_type, &current_value, &max_value);
            qDebug() << "\tCurrent Value " << VCP_FEATURES.commands[elem_].name << " (" << n2hexstr(cde, 2) << ") : " << current_value;

            if (bRet) mon_feature.current_value = current_value;

            features[elem_] = mon_feature;
        }
    }

    qDebug() << "\n";
}

// END ----------------------------------------------------------------


///////////////////////////////////// ENUM DISPLAY DEVICES /////////////////////////////////////////////////////
//
// List (display) devices by going through the adapters of the GPU and the devices connected to it.
// This uses EnumDisplayDevices and has no direct correspondence to the physical monitors from MonitorEnum. 
// https://stackoverflow.com/questions/63095216/how-to-associate-physical-monitor-with-monitor-deviceid
void list_devices()
{
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    DWORD deviceNum = 0;
    while (EnumDisplayDevices(NULL, deviceNum, &dd, 0)) {
        DumpDevice(dd, 0);
        qDebug() << "-----------------------------------------------------";

        DISPLAY_DEVICE newdd = { 0 };
        newdd.cb = sizeof(DISPLAY_DEVICE);
        DWORD monitorNum = 0;
        while (EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
        {
            DumpDevice(newdd, 4);
            monitorNum++;
        }
        qDebug() << "===================================\n";

        deviceNum++;
    }
}

void DumpDevice(const DISPLAY_DEVICE& dd, size_t nSpaceCount)
{
    std::wstring dName = dd.DeviceName;
    std::wstring dString = dd.DeviceString;
    DWORD dFlags = dd.StateFlags;
    std::wstring dID = dd.DeviceID;
    std::wstring dKey = dd.DeviceKey + 42;

    qDebug() << "Device Name: " << "" << dName;
    qDebug() << "Device String: " << "" << dString;
    qDebug() << "Device Flags: " << "" << dFlags;
    qDebug() << "Device ID: " << "" << dID;
    qDebug() << "Device Key: " << "" << dKey;
}

bool displayDevices()
// https://stackoverflow.com/questions/20060584/get-the-name-of-a-monitor
// https://learn.microsoft.com/de-de/windows/win32/api/winuser/nf-winuser-displayconfiggetdeviceinfo
{
    std::vector<DISPLAYCONFIG_PATH_INFO> paths;
    std::vector<DISPLAYCONFIG_MODE_INFO> modes;
    UINT32 flags = QDC_ONLY_ACTIVE_PATHS | QDC_VIRTUAL_MODE_AWARE;
    LONG result = ERROR_SUCCESS;

    do
    {
        // Determine how many path and mode structures to allocate
        UINT32 pathCount, modeCount;
        result = GetDisplayConfigBufferSizes(flags, &pathCount, &modeCount);

        //if (result != ERROR_SUCCESS)
        //{
        //    return false;
        //    // return HRESULT_FROM_WIN32(result);
        //}

        // Allocate the path and mode arrays
        paths.resize(pathCount);
        modes.resize(modeCount);

        // Get all active paths and their modes
        result = QueryDisplayConfig(flags, &pathCount, paths.data(), &modeCount, modes.data(), nullptr);

        // The function may have returned fewer paths/modes than estimated
        paths.resize(pathCount);
        modes.resize(modeCount);

        // It's possible that between the call to GetDisplayConfigBufferSizes and QueryDisplayConfig
        // that the display state changed, so loop on the case of ERROR_INSUFFICIENT_BUFFER.
    } while (result == ERROR_INSUFFICIENT_BUFFER);

    if (result != ERROR_SUCCESS)
    {
        return false;
        //return HRESULT_FROM_WIN32(result);
    }

    // For each active path
    for (auto& path : paths)
    {
        // Find the target (monitor) friendly name
        DISPLAYCONFIG_TARGET_DEVICE_NAME targetName = {};
        targetName.header.adapterId = path.targetInfo.adapterId;
        targetName.header.id = path.targetInfo.id;
        targetName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
        targetName.header.size = sizeof(targetName);
        result = DisplayConfigGetDeviceInfo(&targetName.header);

        if (result != ERROR_SUCCESS)
        {
            return false;
            //return HRESULT_FROM_WIN32(result);
        }

        // Find the adapter device name
        DISPLAYCONFIG_ADAPTER_NAME adapterName = {};
        adapterName.header.adapterId = path.targetInfo.adapterId;
        adapterName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_ADAPTER_NAME;
        adapterName.header.size = sizeof(adapterName);

        result = DisplayConfigGetDeviceInfo(&adapterName.header);

        if (result != ERROR_SUCCESS)
        {
            return false;
            //return HRESULT_FROM_WIN32(result);
        }

        std::wstring name = (targetName.flags.friendlyNameFromEdid ? targetName.monitorFriendlyDeviceName : L"Unknown");
        std::wstring adapter = adapterName.adapterDevicePath;
        int target = path.targetInfo.id;

        qDebug()
            << "Monitor Name:\t"
            << name
            << "\nAdapter:\t"
            << adapter
            << "\nTarget:\t"
            << target
            << "\n";

    }

    return true;
}

bool displayDevices(QMap<int, QList<QString>>& monitorData)
// https://stackoverflow.com/questions/20060584/get-the-name-of-a-monitor
// https://learn.microsoft.com/de-de/windows/win32/api/winuser/nf-winuser-displayconfiggetdeviceinfo
{
    std::vector<DISPLAYCONFIG_PATH_INFO> paths;
    std::vector<DISPLAYCONFIG_MODE_INFO> modes;
    UINT32 flags = QDC_ONLY_ACTIVE_PATHS | QDC_VIRTUAL_MODE_AWARE;
    LONG result = ERROR_SUCCESS;

    do
    {
        // Determine how many path and mode structures to allocate
        UINT32 pathCount, modeCount;
        result = GetDisplayConfigBufferSizes(flags, &pathCount, &modeCount);

        //if (result != ERROR_SUCCESS)
        //{
        //    return false;
        //    // return HRESULT_FROM_WIN32(result);
        //}

        // Allocate the path and mode arrays
        paths.resize(pathCount);
        modes.resize(modeCount);

        // Get all active paths and their modes
        result = QueryDisplayConfig(flags, &pathCount, paths.data(), &modeCount, modes.data(), nullptr);

        // The function may have returned fewer paths/modes than estimated
        paths.resize(pathCount);
        modes.resize(modeCount);

        // It's possible that between the call to GetDisplayConfigBufferSizes and QueryDisplayConfig
        // that the display state changed, so loop on the case of ERROR_INSUFFICIENT_BUFFER.
    } while (result == ERROR_INSUFFICIENT_BUFFER);

    if (result != ERROR_SUCCESS)
    {
        return false;
        //return HRESULT_FROM_WIN32(result);
    }

    int i = 0;
    // For each active path
    for (auto& path : paths)
    {
        // Find the target (monitor) friendly name
        // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-displayconfig_path_target_info
        DISPLAYCONFIG_TARGET_DEVICE_NAME targetName = {};
        targetName.header.adapterId = path.targetInfo.adapterId;
        targetName.header.id = path.targetInfo.id;
        targetName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
        targetName.header.size = sizeof(targetName);
        result = DisplayConfigGetDeviceInfo(&targetName.header);

        if (result != ERROR_SUCCESS)
        {
            return false;
            //return HRESULT_FROM_WIN32(result);
        }

        // Find the adapter device name
        DISPLAYCONFIG_ADAPTER_NAME adapterName = {};
        adapterName.header.adapterId = path.targetInfo.adapterId;
        adapterName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_ADAPTER_NAME;
        adapterName.header.size = sizeof(adapterName);

        result = DisplayConfigGetDeviceInfo(&adapterName.header);

        if (result != ERROR_SUCCESS)
        {
            return false;
            //return HRESULT_FROM_WIN32(result);
        }

        std::wstring name = (targetName.flags.friendlyNameFromEdid ? targetName.monitorFriendlyDeviceName : L"Unknown");
        std::wstring adapter = adapterName.adapterDevicePath;
        int target = path.targetInfo.id;

        qDebug()
            << "Monitor Name:\t"
            << name
            << "\nAdapter:\t"
            << adapter
            << "\nTarget:\t"
            << target
            << "\n";

        monitorData[i] = QList{QString::fromStdWString(name)};

        i++;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////






bool setBrightness(HANDLE hndl, QList<QVariant> inpt)
{
    if (inpt.isEmpty()) return false;

    uint16_t value = inpt[0].toUInt();

    
    bool bChk = SetMonitorBrightness(hndl, value);

    return bChk;
}

bool setContrast(HANDLE hndl, QList<QVariant> inpt)
{
    if (inpt.isEmpty()) return false;

    uint16_t value = inpt[0].toUInt();

    bool bChk = SetMonitorContrast(hndl, value);

    return bChk;
}


QMap<QString, std::function<bool(HANDLE, QList<QVariant>)>> vcp_funcs_WIN = { 
    {"Brightness", setBrightness}, 
    {"Contrast", setContrast} 
};


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
    if (!dummy) {
        bool bChk = DestroyPhysicalMonitor(monitor_.hPhysicalMonitor);

        if (bChk) qDebug() << "Closing HANDLE of " << name;
    }
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

bool Monitor::add_check_feature(uint16_t code, bool addChk)
{
    QString cde_str = n2hexstr(code, 2);

    if (!(VCP_FEATURES.commands.contains(cde_str))) return false;

    if (features.contains(cde_str)) return true;

    else if (!(features.contains(cde_str))) {
        if (!dummy) {
#ifdef Q_OS_WIN
            unsigned long current_value = 0;
            unsigned long max_value = 0;
            MC_VCP_CODE_TYPE code_type;


            bool bChk = GetVCPFeatureAndVCPFeatureReply(monitor_.hPhysicalMonitor, code, &code_type, &current_value, &max_value);

            if (bChk) {
                if (addChk) {
                    monitor_vcp newFeature;
                    newFeature.current_value = current_value;
                    newFeature.max_value = max_value;
                    newFeature.enabled = true;
                }
                
                return true;
            }
            else return false;


#elif Q_OS_UNIX
            qDebug() << "No UNIX Handling yet.";

            return false;

#endif
        }

        else {
            if (addChk) {
                monitor_vcp newFeature;
                newFeature.current_value = 0;
                newFeature.max_value = 100;
                newFeature.enabled = true;
            }

            return true;
        }
    }

    else return false;

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

bool Monitor::set_feature(uint16_t code, int value)
{
    QString cde_str = n2hexstr(code, 2);
    QString cde_name = VCP_FEATURES.commands[cde_str].name;

    qDebug() << name << " -- trying to set " << cde_str << " (" << cde_name << ") ...";
    qDebug() << monitor_.hPhysicalMonitor << " -> " << monitor_.szPhysicalMonitorDescription;


    if (features.contains(cde_str)) {
        if (features[cde_str].current_value == value) return false;

        if (!dummy) {
            if (vcp_funcs_WIN.contains(cde_name))
            {
                QVariant val = QVariant(value);

                bool bChk = vcp_funcs_WIN[cde_name](monitor_.hPhysicalMonitor, QList{ val });

                if (bChk) {
                    qDebug() << cde_name << " set to " << value << " ... success\n";
                    features[cde_str].current_value = value;
                }

                return bChk;
            }

            else {
                bool bChk = SetVCPFeature(monitor_.hPhysicalMonitor, code, value);

                if (bChk) {
                    qDebug() << cde_name << " set to " << value << " ... success\n";
                    features[cde_str].current_value = value;
                }

                return bChk;
            }

        }

        else {
            qDebug() << cde_name << " set to " << value << " ... success\n";
            features[cde_str].current_value = value;
        }
    }
}



