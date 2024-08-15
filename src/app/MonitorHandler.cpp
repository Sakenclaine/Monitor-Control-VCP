#include "MonitorHandler.h"
#include "helpers.h"
#include "errors.h"

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
    add_allowed_values(0x14, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x05, 0x006, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D }, 
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

    //add_command("Display Scaling", 0x86, false, false);
    //add_allowed_values(0x86, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A }, { "No Scaling", "Max Image", "Max Vt 1", "Max Hz 1", "Max Vt 2", "Max Hz 2", "Full Mode", "Zoom Mode", "Squeeze Mode", "Variable" });

    //add_command("Horizontal Frequency", 0xAC, true, false);
    //add_command("Vertical Frequency", 0xAE, true, false);

    //add_command("Flat Panel Sub-Pixel Layout", "Indicates the type of LCD sub-pixel structure.", 0xB2, true, false);
    //add_allowed_values(0xB2, { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 }, { "Not Defined", "RGB Vertical Stripe", "RGB Horizontal Stripe", "BGR Vertical Stripe", "BGR Horizontal Stripe", "Quad-Pixel 1", "Quad-Pixel 2", "Delta", "Mosaic" });

    //add_command("Display Technology Type", "Indicates the base technology type.", 0xB6, true, false);
    //add_allowed_values(0xB6, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }, { "CRT (shadow mask)", "CRT (aperture grill)", "LCD", "LCoS", "Plasma", "OLED", "EL", "Dynamic MEM", "Static MEM" });
}

void VCP_COMMANDS::add_allowed_values(uint16_t code, std::vector<uint16_t> vals, std::vector<QString> desc)
{
    QString str_code = n2hexstr(code, 2);
    std::vector<bool> status(vals.size(), false);

    commands[str_code].possible_values = vals;
    commands[str_code].possible_values_desc = desc;
}

void VCP_COMMANDS::add_allowed_values(uint16_t code, std::initializer_list<uint16_t> codeList, std::initializer_list<QString> descList)
{
    QString str_code = n2hexstr(code, 2);

    commands[str_code].possible_values = codeList;
    commands[str_code].possible_values_desc = descList;
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

VCP_COMMANDS& VCP_COMMANDS::possible_values(std::vector<uint16_t> vals)
{
    comm.possible_values = vals;

    return *this;
}

VCP_COMMANDS& VCP_COMMANDS::possible_values_desc(std::vector<QString> valDesc)
{
    comm.possible_values_desc = valDesc;

    return *this;
}

void VCP_COMMANDS::add_command()
{
    QString str_code = n2hexstr(comm.vcp_code, 2);
    commands[str_code] = comm;
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


// BEGIN --------------------------------------------------------------
// Functions for getting physical monitors and reading as well as parsing their capability string
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

void get_monitor_features_WIN(std::map<QString, monitor_vcp>& features, PHYSICAL_MONITOR& monitor)
{
    std::vector<std::string> kwrds, vals;
    std::map<std::string, std::string> capabilities_dict;

    get_monitor_capabilities_WIN(monitor, kwrds, vals, capabilities_dict);

    std::map<std::string, std::string>::iterator it = capabilities_dict.begin();

    std::vector<std::string> vcp_split = split(capabilities_dict["vcp_only"], " ");
    std::vector<std::string> vcp_with_options = split(capabilities_dict["vcps_with_options"], " ");

    for (auto& elem : vcp_split)
    {
        QString elem_ = QString::fromStdString(elem);
        
        if (VCP_FEATURES.commands.find(elem_) != VCP_FEATURES.commands.end())
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
                            std::vector<uint16_t>::iterator it;
                            std::vector<uint16_t> tVec = VCP_FEATURES.commands[elem_].possible_values;

                            if (std::find(tVec.begin(), tVec.end(), ul) != tVec.end())
                            {
                                mon_feature.possible_values.push_back(ul);
                            }
                        }
                    }
                }
            }

            unsigned long current_value = 0;
            unsigned long max_value = 0;
            MC_VCP_CODE_TYPE code_type;

            uint16_t test = elem_.toUInt();

            bool bRet = GetVCPFeatureAndVCPFeatureReply(monitor.hPhysicalMonitor, test, &code_type, &current_value, &max_value);
            qDebug() << "Current Value (" << int(test) << "): " << current_value;

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

// END ----------------------------------------------------------------


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

    else
    {
        monitor_vcp t1;
        t1.enabled = true;
        
        features["10"] = t1;

        monitor_vcp clrProfile;
        clrProfile.enabled = true;
        clrProfile.possible_values = std::vector<uint16_t>{0x01, 0x02, 0x03};
        clrProfile.current_value = 0x01;

        features["14"] = clrProfile;

        monitor_vcp monInput;
        monInput.enabled = true;
        monInput.possible_values = std::vector<uint16_t>{ 0x01, 0x02, 0x03 };
        monInput.current_value = 0x02;

        features["60"] = monInput;
    }
}


// BEGIN --------------------------------------------------------------
// Definitions for getting and setting features

void Monitor::get_feature_WIN(uint16_t code, uint16_t& ret_code)
{
    unsigned long current_value = 0;
    unsigned long max_value = 0;
    MC_VCP_CODE_TYPE code_type;

    if (!dummy)
    {   
        GetVCPFeatureAndVCPFeatureReply(monitor_.hPhysicalMonitor, code, &code_type, &current_value, &max_value);
        qDebug() << "Current Value (" << int(code) << "): " << current_value;

        ret_code = current_value;
    }

    else
    {
        qDebug() << "Current Value (" << int(code) << "): " << "Dummy Monitor";

    }    
}

void Monitor::set_feature_WIN(uint16_t code, int value, bool& bSet)
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
        //unsigned long ulMinContrast;
        //unsigned long ulCurrContrast;
        //unsigned long ulMaxContrast;

        //bool bGet = GetMonitorBrightness(monitor_.hPhysicalMonitor, &ulMinContrast, &ulCurrContrast, &ulMaxContrast);

        //if (bGet && ulCurrContrast != value)
        //{
        //    unsigned long ulNewValue = ulCurrContrast + (ulCurrContrast - ulCurrContrast) / (100 - 0) * (value - 0);

        //    qDebug() << "Min: " << ulCurrContrast << " Max: " << ulMaxContrast;
        //    qDebug() << "New Value: " << ulNewValue;

        //    if (ulNewValue <= ulMaxContrast)
        //    {
        //        SetMonitorContrast(monitor_.hPhysicalMonitor, ulNewValue);
        //    }
        //}
    }


    else
    {
        bSet = SetVCPFeature(monitor_.hPhysicalMonitor, code, value);
    }
}

void Monitor::get_feature_UNIX(uint16_t code)
{
    qDebug() << "Not implemented";
    throw std::exception(NotImplemented());
}

void Monitor::set_feature_UNIX(uint16_t code, int value, bool& bSet)
{
    qDebug() << "Not implemented";
    throw std::exception(NotImplemented());
}

const uint16_t& Monitor::get_feature(uint16_t code, bool fromMonitor)
{
    QString code_str = n2hexstr(code);

    // Check if code is supported by monitor
    if (features.find(code_str) != features.end())
    {
        if (!fromMonitor){
            return features[code_str].current_value;
        }

        else {
            uint16_t curr_value;

        #ifdef Q_OS_WIN
            get_feature_WIN(code, curr_value);

        #elif Q_OS_UNIX
            get_feature_UNIX(code);
        #endif

            return curr_value;
        }
    }

    else
    {
        qDebug() << "Code not supported";
    }

}

bool Monitor::set_feature(uint16_t code, uint16_t value)
{
    qDebug() << "Set Feature ...";
    
    QString code_str = n2hexstr(code);

    if (features.contains(code_str) && !dummy)
    {
        if (features[code_str].current_value == value) return false;
        if (!(features[code_str].enabled)) return false;

        bool bSet;

    #ifdef Q_OS_WIN
        set_feature_WIN(code, value, bSet);
    #elif Q_OS_UNIX
        set_feature_UNIX(code, value, bSet);
    #endif

        if (bSet) features[code_str].current_value = value;

        return true;

    }

    else if (!(features.contains(code_str)))
    {
        uint16_t cVal, maxVal;
        bool bChk = check_feature(code, cVal, maxVal);
     
        if (bChk)
        {
            monitor_vcp new_feature;
            new_feature.enabled = true;
            new_feature.current_value = cVal;
            new_feature.max_value = maxVal;

            FeatureWarning dlg(code, name, bChk);
            int ret = dlg.exec();

            switch (ret) {
            case QMessageBox::Yes:
                features[code_str] = new_feature;

                bool bSet;
                #ifdef Q_OS_WIN
                set_feature_WIN(code, value, bSet);
                #elif Q_OS_UNIX
                set_feature_UNIX(code, value, bSet);
                #endif

                if (bSet) features[code_str].current_value = value;

                return true;

                break;
            case QMessageBox::Abort:
                qDebug() << "Abort";
                new_feature.enabled = false;
                features[code_str] = new_feature;

                break;
            default:
                // should never be reached
                break;
            }
        }
    }

    return false;
}

void Monitor::check_feature_WIN(uint16_t code, bool& checkRet, uint16_t& cVal, uint16_t& maxVal)
{
    unsigned long current_value = 0;
    unsigned long max_value = 0;
    MC_VCP_CODE_TYPE code_type;

    if (!dummy) {
        checkRet = GetVCPFeatureAndVCPFeatureReply(monitor_.hPhysicalMonitor, code, &code_type, &current_value, &max_value);

        cVal = current_value;
        maxVal = max_value;
    }

    else {
        checkRet = true;         
        cVal = current_value;
        maxVal = max_value;
    }
}

bool Monitor::check_feature(uint16_t code, uint16_t& current_value, uint16_t& max_value)
{
    bool checkRet;
#ifdef Q_OS_WIN
    check_feature_WIN(code, checkRet, current_value, max_value);
#elif Q_OS_UNIX
    qDebug() << "Not Implemented";
#endif

    return checkRet;
}

bool Monitor::check_feature(uint16_t code)
{
    bool checkRet;
    uint16_t current_value, max_value;

#ifdef Q_OS_WIN
    check_feature_WIN(code, checkRet, current_value, max_value);
#elif Q_OS_UNIX
    qDebug() << "Not Implemented";
#endif

    return checkRet;
}

bool Monitor::add_feature(uint16_t code)
{
    bool check = check_feature(code);

    if (check)
    {
        monitor_vcp monVCP;

    }

    return false;
}

// END ----------------------------------------------------------------


// BEGIN --------------------------------------------------------------
// Get functions for variable values
QString Monitor::get_name()
{
    return name;
}

bool Monitor::get_status()
{
    return status;
}

const int Monitor::get_ID()
{
    return _id;
}
// END ----------------------------------------------------------------


// BEGIN --------------------------------------------------------------
// Slots
void Monitor::set_status(bool bVal)
{
    status = bVal;
    qDebug() << "Status Monitor" << name << ": " << bVal;

    emit send_status(bVal);
}

void Monitor::receive_signal(uint16_t code, int value)
{
    if (status)
    {
        qDebug() << "--------------------------------------\n" << "Monitor: " << name << "\nCode: " << code << " Value : " << value;
        qDebug() << "--------------------------------------\n";

        QString code_str = n2hexstr(code, 2);

        if (features.contains(code_str))
        {
            if (features[code_str].enabled) set_feature(code, value);
        }
    }
}

void Monitor::receive_value_request(uint16_t code)
{

}

// END ----------------------------------------------------------------














//void VCP_COMMANDS::add_command(QString name, QString desc, uint16_t code)
//{
//    VCP_COMM comm;
//    comm.name = name;
//    comm.description = desc;
//    comm.vcp_code = code;
//
//    std::string str_code = n2hexstr(code, 2);
//
//    commands[str_code] = comm;
//}
//
//void VCP_COMMANDS::add_command(QString name, QString desc, uint16_t code, bool read, bool write)
//{
//    VCP_COMM comm;
//    comm.name = name;
//    comm.description = desc;
//    comm.vcp_code = code;
//    comm.read_only = read;
//    comm.write_only = write;
//
//    std::string str_code = n2hexstr(code, 2);
//
//    commands[str_code] = comm;
//}
//
//void VCP_COMMANDS::add_command(QString name, uint16_t code, bool read, bool write)
//{
//    VCP_COMM comm;
//    comm.name = name;
//    comm.description = " ";
//    comm.vcp_code = code;
//    comm.read_only = read;
//    comm.write_only = write;
//
//    std::string str_code = n2hexstr(code, 2);
//
//    commands[str_code] = comm;
//}
//
//void VCP_COMMANDS::add_command(QString name, uint16_t code, bool read, bool write, bool range)
//{
//    VCP_COMM comm;
//    comm.name = name;
//    comm.description = " ";
//    comm.vcp_code = code;
//    comm.read_only = read;
//    comm.write_only = write;
//    comm.all_range = range;
//
//    std::string str_code = n2hexstr(code, 2);
//
//    commands[str_code] = comm;
//}





