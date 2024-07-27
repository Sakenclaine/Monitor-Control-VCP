#include "MonitorHandler.h"
#include "helpers.h"

///////////////////////////////////// VCP FEATURE SETUP /////////////////////////////////////////////////////

VCP_COMMANDS VCP_FEATURES; // VCP_COMMANDS object, extern object in header file

// Add all commands, which should be supported to the VCP_COMMANDS object
VCP_COMMANDS::VCP_COMMANDS() 
{
    add_command("Degauss", "Degauss monitor", 0x01, false, true); 

    add_command("New Control Value", "Indicates change of register value", 0x02, false, false); 
    add_allowed_values(0x02, { 0x01 }, { "Force Reset" });

    add_command("Soft Control", "Controls Monitor Buttons", 0x03, true, false); 
    add_allowed_values(0x03, { 0x10, 0x11 }, { "Power", "Brightness UP"});

    add_command("Restore Factory Defaults", "Restore all factory presets", 0x04, false, true); 
    add_command("Restore Factory Luminance/ Contrast Defaults", "Restores factory defaults for luminance and contrast adjustments.", 0x05, false, true); 

    add_command("Restore Factory Color Defaults", "Restore factory defaults for color settings", 0x08, false, true); 

    add_command("User Color Temperature", "", 0x0C, false, false); 

    add_command("Luminance", "Increasing/Decreasing the Luminance", 0x10, false, false); 
    add_command("Contrast", "Change Contrast", 0x12, false, false); 

    add_command("Select Color Preset", "", 0x14, false, false); 
    add_allowed_values(0x14, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x05, 0x006, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D }, 
        { "sRGB", "Native", "4000K", "5000K", "6500K", "7500K", "8200K", "9300K", "10000K", "11500K", "User 1", "User 2", "User 3"});

    add_command("Video Gain (Drive): Red", "Increasing (decreasing) this value will increase (decrease) the luminance of red pixels.", 0x16, false, false);
    add_command("User Color Vision Compensation", "", 0x17, false, false);
    add_command("Video Gain (Drive): Green", "Increasing (decreasing) this value will increase (decrease) the luminance of green pixels.", 0x18, false, false);
    add_command("Video Gain (Drive): Blue", "Increasing (decreasing) this value will increase (decrease) the luminance of blue pixels.", 0x1A, false, false);
    
    add_command("Focus", "Increasing (decreasing) this value will adjust the focus of the image.", 0x1C, false, false);
    
    add_command("Auto Setup", "Perform auto setup function", 0x1E, false, false);
    add_allowed_values(0x1E, { 0x00, 0x01, 0x02 }, { "Not Active", "Perform/performing auto setup", "Enable continuous / periodic auto setup" });


    add_command("Auto Color Setup", "Perform auto color setup function", 0x1F, false, false);
    add_allowed_values(0x1F, { 0x00, 0x01, 0x02 }, { "Not Active", "Perform/performing auto setup", "Enable continuous / periodic auto setup" });

    add_command("Active Control", "", 0x52, true, false);

    add_command("Input Select", "", 0x60, false, false);
    add_allowed_values(0x60, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12 }, {"Analog Video 1", "Analog Video 2", "DVI 1", "DVI 2", "Composite Video 1", "Composite Video 2", "S-Video 1", "S-Video 2", "Tuner 1", "Tuner 2", "Tuner 3", "Component Video 1", "Component Video 2", "Component Video 3", "Display Port 1", "Display Port 2", "HDMI 1", "HDMI 2"});

    add_command("Speaker Volume", 0x62, false, false, true);
    add_allowed_values(0x60, { 0x00, 0xFF, 0x01 }, { "Fixed (Default) level", "Mute", "01->FE Range" });

    add_command("Display Scaling", 0x86, false, false);
    add_allowed_values(0x86, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A }, { "No Scaling", "Max Image", "Max Vt 1", "Max Hz 1", "Max Vt 2", "Max Hz 2", "Full Mode", "Zoom Mode", "Squeeze Mode", "Variable" });

    add_command("Horizontal Frequency", 0xAC, true, false);
    add_command("Vertical Frequency", 0xAE, true, false);

    add_command("Flat Panel Sub-Pixel Layout", "Indicates the type of LCD sub-pixel structure.", 0xB2, true, false);
    add_allowed_values(0xB2, { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 }, { "Not Defined", "RGB Vertical Stripe", "RGB Horizontal Stripe", "BGR Vertical Stripe", "BGR Horizontal Stripe", "Quad-Pixel 1", "Quad-Pixel 2", "Delta", "Mosaic" });

    add_command("Display Technology Type", "Indicates the base technology type.", 0xB6, true, false);
    add_allowed_values(0xB6, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }, { "CRT (shadow mask)", "CRT (aperture grill)", "LCD", "LCoS", "Plasma", "OLED", "EL", "Dynamic MEM", "Static MEM" });


}

void VCP_COMMANDS::add_command(QString name, QString desc, uint16_t code)
{
    VCP_COMM comm;
    comm.name = name;
    comm.description = desc;
    comm.vcp_code = code;

    std::string str_code = n2hexstr(code, 2);

    commands[str_code] = comm;
}

void VCP_COMMANDS::add_command(QString name, QString desc, uint16_t code, bool read, bool write)
{
    VCP_COMM comm;
    comm.name = name;
    comm.description = desc;
    comm.vcp_code = code;
    comm.read_only = read;
    comm.write_only = write;

    std::string str_code = n2hexstr(code, 2);

    commands[str_code] = comm;
}

void VCP_COMMANDS::add_command(QString name, uint16_t code, bool read, bool write)
{
    VCP_COMM comm;
    comm.name = name;
    comm.description = " ";
    comm.vcp_code = code;
    comm.read_only = read;
    comm.write_only = write;

    std::string str_code = n2hexstr(code, 2);

    commands[str_code] = comm;
}

void VCP_COMMANDS::add_command(QString name, uint16_t code, bool read, bool write, bool range)
{
    VCP_COMM comm;
    comm.name = name;
    comm.description = " ";
    comm.vcp_code = code;
    comm.read_only = read;
    comm.write_only = write;
    comm.all_range = range;

    std::string str_code = n2hexstr(code, 2);

    commands[str_code] = comm;
}

void VCP_COMMANDS::add_allowed_values(uint16_t code, std::vector<uint16_t> vals, std::vector<std::string> desc)
{
    std::string str_code = n2hexstr(code, 2);
    std::vector<bool> status(vals.size(), false);

    commands[str_code].possible_values = vals;
    commands[str_code].possible_values_desc = desc;
}

void VCP_COMMANDS::add_allowed_values(uint16_t code, std::initializer_list<uint16_t> codeList, std::initializer_list<std::string> descList)
{
    std::string str_code = n2hexstr(code, 2);

    commands[str_code].possible_values = codeList;
    commands[str_code].possible_values_desc = descList;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////




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
    while (EnumDisplayDevices(NULL, deviceNum, &dd, 0)) 
    {
        DumpDevice(dd, 0);
        DISPLAY_DEVICE newdd = { 0 };
        newdd.cb = sizeof(DISPLAY_DEVICE);
        DWORD monitorNum = 0;
        while (EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
        {
            DumpDevice(newdd, 4);
            monitorNum++;
        }
        puts("");
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
    qDebug() << "Device Flags: "  << "" << dFlags;
    qDebug() << "Device ID: "  << "" << dID;
    qDebug() << "Device Key: "  << "" << dKey << "\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void get_physical_monitors_WIN(std::vector<PHYSICAL_MONITOR>& monitors)
{
    MonitorRects monitorStruct;

    std::vector<HMONITOR> monitors_ = monitorStruct.monitorHandles;
    std::vector<HANDLE> unique_handles;


    for (auto& hmonitor : monitors_)
    {
        DWORD countPhysicalMonitors;
        LPPHYSICAL_MONITOR pPhysicalMonitors = NULL;

        bool nSuccess = GetNumberOfPhysicalMonitorsFromHMONITOR(hmonitor, &countPhysicalMonitors);

        qDebug() << "Number of physical monitors per HMONITOR: " << countPhysicalMonitors;

        if (nSuccess)
        {
            // Allocate the array of PHYSICAL_MONITOR structures.
            pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(
                countPhysicalMonitors * sizeof(PHYSICAL_MONITOR));

            if (pPhysicalMonitors != NULL)
            {
                bool physicalBool = GetPhysicalMonitorsFromHMONITOR(hmonitor, countPhysicalMonitors, pPhysicalMonitors);

                if (physicalBool)
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

void parse_capability_string(std::string s, std::vector<std::string>& keywords, std::vector<std::string>& parsed_string, std::map<std::string, std::string>& vcp_dict)
{
    std::vector<int> inds; // vector containing the inices of paired brackets
    std::string vcps_with_options = "";
    bracket_pair_finder(s, inds);

    // Iterate through index positions of open brackets
    for (size_t i = 0; i < inds.size() - 2; i += 2)
    {
        // Get substrings of capability string corresponding to the content inbetween a pair of brackets
        std::string temp_value = s.substr(inds[i] + 1, (inds[i + 1] - inds[i]) - 1);
        parsed_string.push_back(temp_value);

        // Get term before the brackets
        std::string temp_string("");

        // Iterate from current index inds[i] - 1 (one position before an open bracket in the string) backwards through the string
        // until another bracket or a space appears in the string
        for (size_t k = inds[i] - 1; k >= 0; k--)
        {
            if (s[k] == ')' || s[k] == '(' || s[k] == ' ') { break; }
            temp_string += s[k];
        }
        
        std::reverse(temp_string.begin(), temp_string.end());
        keywords.push_back(temp_string);

        vcp_dict[temp_string] = temp_value;


        if (std::all_of(temp_string.begin(), temp_string.end(), ::isxdigit)) {
            //qDebug() << temp_string << " contains only hexadecimal digits";
            vcps_with_options += temp_string + " ";
        }
    }

    // Get only vcp codes
    std::string vcps = vcp_dict["vcp"];
    std::vector<int> inds_2;
    bool found_brackets = bracket_pair_finder(vcps, inds_2);
    int stop = 0;

    while (found_brackets)
    {
        vcps = vcps.erase(inds_2[0], (inds_2[1] - inds_2[0]) + 1);
        found_brackets = bracket_pair_finder(vcps, inds_2);

        stop++;

        if (stop > 1000) { break; }
    }

    vcp_dict["vcp_only"] = vcps;
    vcp_dict["vcps_with_options"] = vcps_with_options;
}

void get_monitor_capabilities_WIN(PHYSICAL_MONITOR& monitor, std::vector<std::string>& kwrds, std::vector<std::string>& vals, std::map<std::string, std::string>& capabilities_dict)
{
    // Get physical monitor handle.
    HANDLE hPhysicalMonitor = monitor.hPhysicalMonitor;

    DWORD capLength = 0;
    bool bSuccess = GetCapabilitiesStringLength(hPhysicalMonitor, &capLength);

    if (bSuccess)
    {
        // Allocate the string buffer.
        LPSTR szCapabilitiesString = (LPSTR)malloc(capLength);
        if (szCapabilitiesString != NULL)
        {
            // Get the capabilities string.
            bSuccess = CapabilitiesRequestAndCapabilitiesReply(
                hPhysicalMonitor,
                szCapabilitiesString,
                capLength);

            parse_capability_string(szCapabilitiesString, kwrds, vals, capabilities_dict);
        }

        // Free the string buffer.
        free(szCapabilitiesString);
    }
}


void get_monitor_features_WIN(std::map<std::string, monitor_vcp>& features, PHYSICAL_MONITOR& monitor)
{
    std::vector<std::string> kwrds, vals;
    std::map<std::string, std::string> capabilities_dict;

    get_monitor_capabilities_WIN(monitor, kwrds, vals, capabilities_dict);

    std::map<std::string, std::string>::iterator it = capabilities_dict.begin();

    std::vector<std::string> vcp_split = split(capabilities_dict["vcp_only"], " ");
    std::vector<std::string> vcp_with_options = split(capabilities_dict["vcps_with_options"], " ");

    for (auto& elem : vcp_split)
    {
        if (VCP_FEATURES.commands.find(elem) != VCP_FEATURES.commands.end())
        {
            struct monitor_vcp mon_feature;
            mon_feature.enabled = true;

            if (std::find(vcp_with_options.begin(), vcp_with_options.end(), elem) != vcp_with_options.end())
            {
                for (auto& el : split(capabilities_dict[elem], " "))
                {
                    if (!(el == " " || el == "")) {
                        uint16_t ul = std::stoul("0x" + el, nullptr, 0);

                        if (!(VCP_FEATURES.commands[elem].possible_values.empty()))
                        {
                            std::vector<uint16_t>::iterator it;
                            std::vector<uint16_t> tVec = VCP_FEATURES.commands[elem].possible_values;

                            if (std::find(tVec.begin(), tVec.end(), ul) != tVec.end())
                            {
                                mon_feature.possible_values.push_back(ul);
                            }
                        }
                    }
                }
            }

            features[elem] = mon_feature;
        }
    }
}

Monitor::Monitor(PHYSICAL_MONITOR monitor, QString name) :
    monitor_(monitor),
    name(name)
{
    dummy = false;
}

Monitor::Monitor(QString name) :
    name(name)
{
}

Monitor::~Monitor()
{
}


void Monitor::monitor_init()
{
    if (!dummy)
    {
        #ifdef Q_OS_WIN
            get_monitor_features_WIN(features, monitor_);

        #elif Q_OS_UNIX
            qDebug() << "No UNIX Handling yet.";

        #endif
       
    }
}

QString Monitor::get_name()
{
    return name;
}

bool Monitor::get_enabled()
{
    return status;
}

void Monitor::get_feature_WIN(uint16_t code)
{
    unsigned long current_value = 0;
    unsigned long max_value = 0;
    MC_VCP_CODE_TYPE code_type;

    uint16_t test = 0x10;

    if (!dummy)
    {   
        GetVCPFeatureAndVCPFeatureReply(monitor_.hPhysicalMonitor, test, &code_type, &current_value, &max_value);
        qDebug() << "Current Value (" << int(test) << "): " << current_value;
    }

    else
    {
        qDebug() << "Current Value (" << int(test) << "): " << "None -> Dummy Monitor";
    }    
}


void Monitor::set_feature_WIN(uint16_t code, int value)
{
    if (code == 0x10)
    {
        unsigned long ulMinBrightness;
        unsigned long ulCurrBrightness;
        unsigned long ulMaxBrightness;
        
        bool bGet = GetMonitorBrightness(monitor_.hPhysicalMonitor, &ulMinBrightness, &ulCurrBrightness, &ulMaxBrightness);

        if (bGet && ulCurrBrightness != value)
        {
            unsigned long ulNewValue = ulMinBrightness + (ulMaxBrightness - ulMinBrightness) / (100 - 0) * (value - 0);

            qDebug() << "Min: " << ulMinBrightness << " Max: " << ulMaxBrightness;
            qDebug() << "New Value: " << ulNewValue;

            if (ulNewValue <= ulMaxBrightness)
            {
                SetMonitorBrightness(monitor_.hPhysicalMonitor, ulNewValue);
            }
        }
    }

    else if (code == 0x12)
    {
        unsigned long ulMinContrast;
        unsigned long ulCurrContrast;
        unsigned long ulMaxContrast;

        bool bGet = GetMonitorBrightness(monitor_.hPhysicalMonitor, &ulMinContrast, &ulCurrContrast, &ulMaxContrast);

        if (bGet && ulCurrContrast != value)
        {
            unsigned long ulNewValue = ulCurrContrast + (ulCurrContrast - ulCurrContrast) / (100 - 0) * (value - 0);

            qDebug() << "Min: " << ulCurrContrast << " Max: " << ulMaxContrast;
            qDebug() << "New Value: " << ulNewValue;

            if (ulNewValue <= ulMaxContrast)
            {
                SetMonitorContrast(monitor_.hPhysicalMonitor, ulNewValue);
            }
        }
    }

}



int Monitor::get_brightness()
{
    return 0;
}

int Monitor::get_contrast()
{
    return 0;
}

int Monitor::get_R()
{
	return 0;
}

int Monitor::get_G()
{
	return 0;
}

int Monitor::get_B()
{
	return 0;
}


void Monitor::set_enabled(bool bVal)
{
    status = bVal;

    qDebug() << "Status Monitor" << name << ": " << bVal;

    emit send_status(bVal);
}


void Monitor::receive_signal(uint16_t code, int value)
{
    if (status)
    {
        std::string code_str = n2hexstr(code, 2);

        if (features.find(code_str) != features.end())
        {
            qDebug() << "\n--------------------------------------\n" << "Monitor: " << name << "\nCode: " << code << " Value : " << value;

            get_feature_WIN(code);
            set_feature_WIN(code, value);

            qDebug() << "--------------------------------------\n";
        }
    }
}


void Monitor::receive_value_request(uint16_t code)
{

}











///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



//void Monitor::old_load()
//{
//    for (auto& elem : vcp_split)
//    {
//
//        //qDebug() << "Feature request: " << elem;
//
//        if (features.commands.find(elem) != features.commands.end())
//        {
//            //qDebug() << "Feature was found -> " << features.commands[elem].name;
//            features.commands[elem].enable = true;
//
//            if (std::find(vcp_with_options.begin(), vcp_with_options.end(), elem) != vcp_with_options.end())
//            {
//                qDebug() << "Feature " << elem << " has options.";
//
//            }
//
//        }
//
//        else { qDebug() << "Feature " << elem << " not existent."; }
//    }
//
//    DWORD current_value;
//    DWORD max_value;
//    MC_VCP_CODE_TYPE code_type;
//
//    unsigned char test = 0x14;
//    GetVCPFeatureAndVCPFeatureReply(monitor_->hPhysicalMonitor, test, &code_type, &current_value, &max_value);
//
//    qDebug() << "Current Value (" << int(test) << "): " << current_value;
//}