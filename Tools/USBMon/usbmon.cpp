/**
 * @file usbmon.cpp
 * @brief Implementation of USBMon for monitoring USB device changes.
 *
 * Overview
 * - Creates a message-only hidden window (HWND_MESSAGE) to receive
 *   device notifications via WM_DEVICECHANGE for GUID_DEVINTERFACE_USB_DEVICE.
 * - Logs Connect/Disconnect events with a human-readable timestamp and
 *   enriched details: friendly name, manufacturer, and high-level category.
 * - Enrichment logic combines deterministic VID/PID parsing with
 *   best-effort SetupAPI queries (friendly name, manufacturer, class).
 *
 * Logging format
 *   [ Tue Sep 09 12:34:56 2025 ] USB Connected: <Name> | Type: <Category> | Manufacturer: <Vendor>
 *   [ Tue Sep 09 12:35:12 2025 ] USB Disconnected: <Name> | Type: <Category> | Manufacturer: <Vendor>
 *
 * Design notes
 * - Caching: We cache resolved info on arrival keyed by the device interface
 *   path so that removal events can show the same friendly details even when
 *   the device is no longer present in the SetupAPI set.
 * - Determinism: Prefer VID/PID lookups for Vendor/Product when available.
 *   Fall back to SetupAPI SPDRP_FRIENDLYNAME/DEVICEDESC and SPDRP_MFG.
 * - Category mapping: Use USB class codes from SPDRP_HARDWAREID when possible;
 *   otherwise map SPDRP_CLASS/CLASSGUID to a reasonable category.
 * - Threading: All work happens on the single UI thread running the message
 *   loop; no background threads are used.
 * - Unicode: Message payloads are UTF-16 (wide); we convert to UTF-8 for logs.
 *
 * Performance & reliability
 * - SetupAPI enumeration and property queries are bounded and only performed
 *   on arrival/removal events; overhead is minimal for typical usage.
 * - Memory allocated for interface details is freed promptly; handles are
 *   destroyed via SetupDiDestroyDeviceInfoList.
 */
#include "usbmon.h"
#include <cstdio>
#include <cstring>

USBMon usbMon;

/**
 * @brief Constructs a USBMon instance with logging disabled and lookup tables initialized.
 *
 * Rationale: Populate lookup tables upfront so first arrival events can be
 * resolved without incurring one-time initialization latency.
 */
USBMon::USBMon()
    : isLogging(false)
{
        initLookupTables();
}

USBMon::~USBMon() {
    stopLogging();
}

/**
 * @brief Starts USB monitoring log and writes a header with start time.
 * @param filename Path to the log file to write USB events.
 * @return true if the log was opened and monitoring enabled; false otherwise.
 *
 * Behavior
 * - Opens the log file (truncating if it exists) and writes a header line
 *   using ctime()-style timestamps to match ClipMon.
 * - Does not create parent directories; the caller must ensure the path’s
 *   directory exists.
 * - On failure, shows an error message box and returns false.
 */
bool USBMon::startLogging(const string &filename)
{
    usbLog.open(filename, ios::out | ios::trunc);

    if (!usbLog.is_open()) {
        string msg = string("Failed to open log file. Error: ") + to_string(GetLastError());
        MessageBoxA(NULL, msg.c_str(), "USBMon Failed", MB_ICONERROR | MB_OK);

        return false;
    }

    isLogging = true;

    usbLog << "=== USBMon Started at: " << getCurrentTime() << " ===\n";

    return true;
}

/**
 * @brief Stops logging and writes a footer with stop time.
 *
 * Idempotent semantics
 * - If logging is active, append a footer with the current time, flush/close
 *   the file, and mark logging inactive. If already inactive, no-op.
 */
void USBMon::stopLogging()
{
    if (isLogging) {
        usbLog << "\n=== USBMon Stopped at: " << getCurrentTime() << " ===\n";

        usbLog.close();

        isLogging = false;
    }
}

/**
 * @brief Logs a device change (arrival/removal) with resolved details.
 * @param eventType WM_DEVICECHANGE wParam indicating the change type (DBT_DEVICEARRIVAL or DBT_DEVICEREMOVECOMPLETE).
 * @param eventData WM_DEVICECHANGE lParam providing device interface details (PDEV_BROADCAST_DEVICEINTERFACE).
 *
 * Flow
 * 1) Validate notification type (DEVICEINTERFACE) and extract the interface
 *    path (UTF-16), converting to UTF-8 for cache/log keys.
 * 2) On arrival:
 *    - Resolve info using resolveDeviceInfo (VID/PID + SetupAPI).
 *    - Cache the resolved details keyed by the interface path for later use.
 * 3) On removal:
 *    - Prefer cached details; if missing, attempt a best-effort resolve.
 * 4) Emit a single log line with timestamp, action, friendly name, category,
 *    and manufacturer (if available). Flush immediately to minimize data loss.
 *
 * Notes
 * - The interface path is stable across the pair of arrival/removal events
 *   within a single plug/unplug cycle, making it a good cache key.
 */
void USBMon::logDeviceChange(WPARAM eventType, LPARAM eventData)
{
    if (!isLogging) {
        return;
    }

    if (eventType == DBT_DEVICEARRIVAL ||
        eventType == DBT_DEVICEREMOVECOMPLETE)
    {
        auto pHdr = reinterpret_cast<PDEV_BROADCAST_HDR>(eventData);
        if (pHdr->dbch_devicetype != DBT_DEVTYP_DEVICEINTERFACE) {
            return;
        }

    auto pDev = reinterpret_cast<PDEV_BROADCAST_DEVICEINTERFACE>(pHdr);
    // Raw interface path (e.g., \\?\USB#VID_XXXX&PID_YYYY#...)
        wstring devPathW(pDev->dbcc_name);
        string devPath = wideToUTF8(devPathW);

    string friendly;
    string devType;
    string manufacturer;

        if (eventType == DBT_DEVICEARRIVAL) {
            auto info = resolveDeviceInfo(devPathW);
            friendly = info.friendly.empty() ? devPath : info.friendly;
            devType  = info.category.empty() ? string("USB Device") : info.category;
            manufacturer = info.manufacturer;
            // Cache for later removal event; ensures stable details even when
            // the device is no longer queryable via SetupAPI.
            deviceCache[devPath] = {friendly, devType, manufacturer};
        } else {
            // removal: device might not be present; use cache first
            auto it = deviceCache.find(devPath);
            if (it != deviceCache.end()) {
                friendly = it->second.friendly;
                devType  = it->second.category;
                manufacturer = it->second.manufacturer;
                deviceCache.erase(it);
            } else {
                // best-effort resolve even if not present
                auto info = resolveDeviceInfo(devPathW);
                friendly = info.friendly.empty() ? devPath : info.friendly;
                devType  = info.category.empty() ? string("USB Device") : info.category;
                manufacturer = info.manufacturer;
            }
        }

    // Single-line, structured log record. Keep order stable for downstream parsers.
    usbLog << "[ " << getCurrentTime() << " ] "
               << (eventType == DBT_DEVICEARRIVAL
                   ? "USB Connected: "
                   : "USB Disconnected: ")
               << friendly
               << " | Type: " << devType
               << (manufacturer.empty() ? "" : string(" | Manufacturer: ") + manufacturer)
               << "\n";
        usbLog.flush();
    }
}

/**
 * @brief Returns current time formatted as a human-readable string via ctime().
 * @return Current time string without the trailing newline from ctime().
 *
 * Implementation note: ctime() returns a pointer to a static buffer and is not
 * thread-safe. This program uses a single-threaded message loop, so it’s safe
 * in this context. The trailing newline is removed for cleaner log records.
 */
string USBMon::getCurrentTime()
{
    time_t now = time(0);

    char *timeStr = ctime(&now);

    string currentTime(timeStr);

    currentTime.pop_back();

    return currentTime;
}

/**
 * @brief Converts wide string (UTF-16) to UTF-8.
 * @param wstr Input wide string to convert.
 * @return UTF-8 encoded string representation.
 *
 * Uses WideCharToMultiByte in two passes to compute the required size and then
 * perform the conversion. Input is not assumed to be null-terminated beyond
 * the provided length.
 */
string USBMon::wideToUTF8(const wstring &wstr)
{
    if (wstr.empty()) {
        return string();
    }

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, 
        wstr.c_str(), (int) wstr.size(), 
        nullptr, 0, nullptr, nullptr);
    
    string utf8(sizeNeeded, 0);

    WideCharToMultiByte(CP_UTF8, 0,
        wstr.c_str(), (int) wstr.size(),
        &utf8[0], sizeNeeded, nullptr, nullptr);

    return utf8;
}

/**
 * @brief Resolves friendly name, category, and manufacturer for an interface path.
 * @param interfacePathW Wide-character device interface path from WM_DEVICECHANGE.
 * @return DeviceInfo with any available attributes set; empty strings otherwise.
 *
 * Strategy (best-first):
 * 1) Parse VID/PID from the interface path and consult in-memory lookup tables
 *    for vendor (manufacturer), specific product names, and category overrides.
 * 2) Enumerate current USB device interfaces via SetupAPI to find the matching
 *    item by DevicePath and query the following registry-backed properties:
 *    - SPDRP_FRIENDLYNAME (preferred human name) or SPDRP_DEVICEDESC fallback
 *    - SPDRP_MFG (manufacturer), if not already set from VID
 *    - SPDRP_HARDWAREID (REG_MULTI_SZ) to extract USB class code and infer
 *      category (e.g., Mass Storage for class 0x08).
 *    - SPDRP_CLASS or SPDRP_CLASSGUID if class code isn’t present.
 *
 * Memory & resources: Allocates a detail buffer per device interface and frees
 * it immediately after use. The HDEVINFO handle is destroyed before return.
 */
USBMon::DeviceInfo USBMon::resolveDeviceInfo(const wstring &interfacePathW)
{
    DeviceInfo result; // friendly, category, manufacturer
    string path = wideToUTF8(interfacePathW);

    // First try VID/PID parse and lookups
    unsigned vid = 0, pid = 0;
    if (parseVidPid(path, vid, pid)) {
        auto vIt = vendorNames.find(vid);
        if (vIt != vendorNames.end()) {
            result.manufacturer = vIt->second;
        }
        auto pOuter = productNames.find(vid);
        if (pOuter != productNames.end()) {
            auto pIt = pOuter->second.find(pid);
            if (pIt != pOuter->second.end()) {
                result.friendly = pIt->second;
            }
        }
        auto cOuter = categoryOverrides.find(vid);
        if (cOuter != categoryOverrides.end()) {
            auto cIt = cOuter->second.find(pid);
            if (cIt != cOuter->second.end()) {
                result.category = cIt->second;
            }
        }
    }

    // Enumerate currently present USB device interfaces and match by path.
    HDEVINFO hDevInfo = SetupDiGetClassDevs(
        &GUID_DEVINTERFACE_USB_DEVICE,
        nullptr, nullptr,
        DIGCF_DEVICEINTERFACE | DIGCF_PRESENT
    );

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        return result;
    }

    SP_DEVICE_INTERFACE_DATA ifaceData = {};
    ifaceData.cbSize = sizeof(ifaceData);

    for (DWORD idx = 0; SetupDiEnumDeviceInterfaces(
             hDevInfo, nullptr, &GUID_DEVINTERFACE_USB_DEVICE, idx, &ifaceData); ++idx) {
        DWORD reqSize = 0;
    // First call to obtain required buffer size for detail data
    SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifaceData, nullptr, 0, &reqSize, nullptr);
        if (reqSize == 0) continue;
        auto detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(reqSize);
        if (!detail) continue;
        detail->cbSize = sizeof(*detail);
        SP_DEVINFO_DATA devInfo = {};
        devInfo.cbSize = sizeof(devInfo);

    if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifaceData, detail, reqSize, nullptr, &devInfo)) {
            wstring thisPathW(detail->DevicePath);
            if (thisPathW == interfacePathW) {
                // Friendly name or device description
                WCHAR buf[256];
                if (SetupDiGetDeviceRegistryPropertyW(
                        hDevInfo, &devInfo, SPDRP_FRIENDLYNAME,
                        nullptr, (PBYTE)buf, sizeof(buf), nullptr)) {
                    result.friendly = wideToUTF8(buf);
                } else if (SetupDiGetDeviceRegistryPropertyW(
                        hDevInfo, &devInfo, SPDRP_DEVICEDESC,
                        nullptr, (PBYTE)buf, sizeof(buf), nullptr)) {
                    result.friendly = wideToUTF8(buf);
                }

                // Manufacturer (fallback if not found via VID)
                if (result.manufacturer.empty() && SetupDiGetDeviceRegistryPropertyW(
                        hDevInfo, &devInfo, SPDRP_MFG,
                        nullptr, (PBYTE)buf, sizeof(buf), nullptr)) {
                    result.manufacturer = wideToUTF8(buf);
                }

                // Category: derive from USB class code in hardware IDs or from Class
                // Try SPDRP_HARDWAREID (REG_MULTI_SZ)
                WCHAR multi[1024];
                if (SetupDiGetDeviceRegistryPropertyW(
                        hDevInfo, &devInfo, SPDRP_HARDWAREID,
                        nullptr, (PBYTE)multi, sizeof(multi), nullptr)) {
                    // Iterate REG_MULTI_SZ: sequence of null-terminated strings, ended by an extra null
                    for (WCHAR *p = multi; *p; p += wcslen(p) + 1) {
                        wstring id(p);
                        // Look for "USB\\Class_XX" or "USB\\Class_XX&SubClass_YY&Prot_ZZ"
                        size_t pos = id.find(L"USB\\Class_");
                        if (pos != wstring::npos) {
                            if (pos + 11 + 2 <= id.size()) {
                                wstring clsHex = id.substr(pos + 11, 2);
                                unsigned cls = wcstoul(clsHex.c_str(), nullptr, 16);
                                switch (cls) {
                                case 0x08: result.category = "Mass Storage"; break;
                                case 0x03: result.category = "HID"; break;
                                case 0x0A: result.category = "CDC-Data"; break;
                                case 0x02: result.category = "Communications"; break;
                                case 0x0E: result.category = "USB Video"; break;
                                case 0x0B: result.category = "Smart Card"; break;
                                case 0x0D: result.category = "Content Security"; break;
                                case 0x09: result.category = "Hub"; break;
                                case 0x0F: result.category = "Personal Healthcare"; break;
                                case 0xE0: result.category = "Wireless Controller"; break;
                                case 0xEF: result.category = "Miscellaneous"; break;
                                default: result.category = "USB Device"; break;
                                }
                            }
                            break;
                        }
                    }
                }

                // If still unknown, use Class (e.g., "USB", "Net", etc.)
                if (result.category.empty()) {
                    if (SetupDiGetDeviceRegistryPropertyW(
                            hDevInfo, &devInfo, SPDRP_CLASS,
                            nullptr, (PBYTE)buf, sizeof(buf), nullptr)) {
                        string cls = wideToUTF8(buf);
                        // Map some common classes to friendlier categories
                        if (cls == "USB") result.category = "USB Device";
                        else if (cls == "Net") result.category = "Network Adapter";
                        else if (cls == "HIDClass") result.category = "HID";
                        else if (cls == "DiskDrive") result.category = "Mass Storage";
                        else result.category = cls;
                    } else if (SetupDiGetDeviceRegistryPropertyW(
                            hDevInfo, &devInfo, SPDRP_CLASSGUID,
                            nullptr, (PBYTE)buf, sizeof(buf), nullptr)) {
                        string guid = wideToUTF8(buf);
                        // Known class GUID mappings (subset)
                        if (guid == "{4d36e972-e325-11ce-bfc1-08002be10318}") result.category = "Network Adapter"; // Net
                        else if (guid == "{4d36e967-e325-11ce-bfc1-08002be10318}") result.category = "Disk Drive"; // DiskDrive
                        else if (guid == "{36fc9e60-c465-11cf-8056-444553540000}") result.category = "USB Device"; // USB
                        else result.category = "USB Device";
                    }
                }

                free(detail);
                break;
            }
        }
        free(detail);
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    // If no explicit product name, synthesize a friendly label for readability
    if (result.friendly.empty()) {
        if (!result.manufacturer.empty() && !result.category.empty()) {
            result.friendly = result.manufacturer + string(" ") + result.category;
        } else if (!result.manufacturer.empty()) {
            result.friendly = result.manufacturer + string(" USB Device");
        }
    }
    return result;
}

/**
 * @brief Parses VID and PID from a device interface path string.
 * @param path Device interface path (UTF-8), typically like: "\\\\?\\USB#VID_1234&PID_ABCD#...".
 * @param vid Output vendor ID (hex value).
 * @param pid Output product ID (hex value).
 * @return true if both values were successfully parsed.
 *
 * Expectations & limitations
 * - Relies on the conventional "VID_XXXX" and "PID_YYYY" substrings
 *   embedded in the interface path.
 * - Case-sensitive search (matches uppercase). Paths with lowercase labels
 *   (e.g., "vid_"/"pid_") are not currently handled.
 */
bool USBMon::parseVidPid(const string &path, unsigned &vid, unsigned &pid)
{
    // Expect something like: \\?\USB#VID_1005&PID_B128#...
    vid = pid = 0;
    const char *p = path.c_str();
    // Simple scan for VID_ and PID_
    const char *v = strstr(p, "VID_");
    const char *d = strstr(p, "PID_");
    if (!v || !d) return false;
    unsigned vtmp = 0, ptmp = 0;
    if (sscanf(v, "VID_%4x", &vtmp) != 1) return false;
    if (sscanf(d, "PID_%4x", &ptmp) != 1) return false;
    vid = vtmp; pid = ptmp; return true;
}

/**
 * @brief Initializes VID->Vendor, VID:PID->Product, and category override tables.
 *
 * Purpose
 * - Provide deterministic naming/category for common vendors and devices.
 * - Allow quick recognition of Mass Storage vs. Network Adapter vs. other classes.
 *
 * Notes
 * - Lists are not exhaustive; many entries are representative PIDs seen in the wild.
 * - Category overrides take precedence over class-derived categories.
 * - To extend: add vendorNames[VID] = "Vendor"; productNames[VID][PID] = "Product";
 *   categoryOverrides[VID][PID] = "Category".
 */
void USBMon::initLookupTables()
{
    // Populate known vendors
    vendorNames[0x1005] = "Apacer Technology (pendrive)";
    vendorNames[0x13FE] = "Phison Electronics";
    vendorNames[0x046D] = "Logitech";
    vendorNames[0x05AC] = "Apple";
    vendorNames[0x12D1] = "Huawei";
    vendorNames[0x0BDA] = "Realtek";
    vendorNames[0x2357] = "TP-Link";
    vendorNames[0x03F0] = "HP Inc.";
    vendorNames[0x0781] = "SanDisk";
    vendorNames[0x125F] = "ADATA Technology";
    vendorNames[0x0951] = "Kingston";
    vendorNames[0x8564] = "Transcend";
    vendorNames[0x05DC] = "Lexar Media";
    vendorNames[0x154B] = "PNY";
    vendorNames[0x1058] = "Western Digital";
    vendorNames[0x0BC2] = "Seagate";
    vendorNames[0x04E8] = "Samsung";
    vendorNames[0x0930] = "Toshiba";
    vendorNames[0x054C] = "Sony";
    vendorNames[0x058F] = "Alcor Micro";          // common USB flash disk controllers
    vendorNames[0x090C] = "Silicon Motion";       // common USB flash disk controllers
    vendorNames[0x1B1C] = "Corsair";
    vendorNames[0x1E3D] = "Chipsbank";            // common low-cost flash controllers
    vendorNames[0x1F75] = "Innostor";             // common flash controllers

    // Populate product names for specific VIDs
    productNames[0x1005][0xB128] = "USB Flash Drive (generic)";
    // Common Phison controller generic mass-storage PIDs (example set)
    productNames[0x13FE][0x4100] = "USB Flash Disk";
    productNames[0x13FE][0x4200] = "USB Flash Disk";
    productNames[0x2357][0x010C] = "TP-Link USB Adapter";
    // SanDisk (generic)
    productNames[0x0781][0x5567] = "SanDisk USB Flash Drive"; // seen in some models
    // HP (generic)
    productNames[0x03F0][0x5607] = "HP USB Flash Drive";      // example PID seen in the wild
    // ADATA (generic)
    productNames[0x125F][0x312B] = "ADATA USB Flash Drive";   // example PID
    // Kingston (generic)
    productNames[0x0951][0x1666] = "Kingston USB Flash Drive"; // example PID
    // Transcend (generic)
    productNames[0x8564][0x1000] = "Transcend USB Flash Drive"; // example PID
    // Lexar (generic)
    productNames[0x05DC][0xA81D] = "Lexar USB Flash Drive";     // example PID
    // PNY (generic)
    productNames[0x154B][0x00F6] = "PNY USB Flash Drive";       // example PID
    // Western Digital external storage
    productNames[0x1058][0x25E2] = "WD External Drive";         // example PID
    // Seagate external storage
    productNames[0x0BC2][0x2322] = "Seagate External Drive";    // example PID
    // Samsung (generic storage/MTP appear under different interfaces)
    productNames[0x04E8][0x6860] = "Samsung USB Device";        // MTP interface PID example
    // Toshiba external storage
    productNames[0x0930][0x6545] = "Toshiba External Drive";    // example PID
    // Sony (generic)
    productNames[0x054C][0x05BA] = "Sony USB Storage";          // example PID
    // Controller vendors often present as "USB Flash Disk"
    productNames[0x058F][0x6387] = "USB Flash Disk";            // Alcor Micro
    productNames[0x090C][0x1000] = "USB Flash Disk";            // Silicon Motion
    productNames[0x1E3D][0x2096] = "USB Flash Disk";            // Chipsbank
    productNames[0x1F75][0x0621] = "USB Flash Disk";            // Innostor

    // Category overrides where we know exact type
    categoryOverrides[0x1005][0xB128] = "Mass Storage";
    categoryOverrides[0x13FE][0x4100] = "Mass Storage";
    categoryOverrides[0x13FE][0x4200] = "Mass Storage";
    categoryOverrides[0x2357][0x010C] = "Network Adapter";
    // Assume mass storage for common flash-controller presentations
    categoryOverrides[0x058F][0x6387] = "Mass Storage"; // Alcor
    categoryOverrides[0x090C][0x1000] = "Mass Storage"; // SMI
    categoryOverrides[0x1E3D][0x2096] = "Mass Storage"; // Chipsbank
    categoryOverrides[0x1F75][0x0621] = "Mass Storage"; // Innostor
    // External HDD/SSD
    categoryOverrides[0x1058][0x25E2] = "Mass Storage"; // WD
    categoryOverrides[0x0BC2][0x2322] = "Mass Storage"; // Seagate
    // Realtek common Wi-Fi adapter example
    categoryOverrides[0x0BDA][0xB812] = "Network Adapter"; // example RTL88x2
    // Vendor-specific flash drives
    categoryOverrides[0x0781][0x5567] = "Mass Storage"; // SanDisk
    categoryOverrides[0x03F0][0x5607] = "Mass Storage"; // HP
    categoryOverrides[0x125F][0x312B] = "Mass Storage"; // ADATA
    categoryOverrides[0x0951][0x1666] = "Mass Storage"; // Kingston
    categoryOverrides[0x8564][0x1000] = "Mass Storage"; // Transcend
    categoryOverrides[0x05DC][0xA81D] = "Mass Storage"; // Lexar
    categoryOverrides[0x154B][0x00F6] = "Mass Storage"; // PNY
}

/**
 * @brief Window procedure to subscribe for USB device notifications and forward changes.
 *
 * Behavior
 * - On WM_CREATE, registers for device interface notifications for USB devices
 *   using RegisterDeviceNotification with GUID_DEVINTERFACE_USB_DEVICE.
 * - On WM_DEVICECHANGE, forwards the event to USBMon::logDeviceChange.
 * - All other messages are handled by DefWindowProc.
 */
LRESULT CALLBACK DeviceProc(HWND hwnd, UINT msg,
                            WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE: {
        DEV_BROADCAST_DEVICEINTERFACE filter = {};
        filter.dbcc_size      = sizeof(filter);
        filter.dbcc_devicetype= DBT_DEVTYP_DEVICEINTERFACE;
        filter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
        RegisterDeviceNotification(
            hwnd, &filter,
            DEVICE_NOTIFY_WINDOW_HANDLE
        );
        break;
    }
    case WM_DEVICECHANGE:
        usbMon.logDeviceChange(wParam, lParam);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/**
 * @brief Entry point: creates a message-only window and runs message loop.
 *
 * Steps
 * 1) Persist the current thread ID to ThreadId/usbMonThread.id (for external control).
 * 2) Register a lightweight window class and create a message-only window
 *    (no taskbar/UI) to receive device notifications.
 * 3) Initialize logging to the provided log path or default to Logs/usb.log.
 * 4) Run the message loop until a WM_STOP custom message is received.
 *
 * Exit codes
 * - 0 on normal shutdown
 * - 1 on setup failures (window creation or logging initialization)
 */
int main(int argc, char *argv[])
{
    // save thread id
    ofstream("ThreadId/usbMonThread.id")
        << GetCurrentThreadId();

    HINSTANCE hInst = GetModuleHandle(nullptr);
    const wchar_t CLASS_NAME[] = L"USBMon";

    WNDCLASS wc = {};
    wc.lpfnWndProc   = DeviceProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"USB Monitor",
        0,0,0,0,0,
        HWND_MESSAGE, nullptr, hInst, nullptr
    );
    if (!hwnd) {
        MessageBoxA(NULL,
            ("CreateWindowEx failed. Error: " +
             to_string(GetLastError()))
            .c_str(),
            "USBMon Failed",
            MB_ICONERROR | MB_OK);
        return 1;
    }

    string logFile = (argc > 1) ? argv[1] : "Logs/usb.log";
    if (!usbMon.startLogging(logFile)) {
        MessageBoxA(NULL,
            "Failed to start logging",
            "USBMon Failed",
            MB_ICONERROR | MB_OK);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_STOP) {
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    usbMon.stopLogging();
    
    return 0;
}