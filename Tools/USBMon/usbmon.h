/**
 * @file usbmon.h
 * @brief Header for USBMon - USB device arrival/removal monitoring and logging.
 *
 * Provides the USBMon class that listens for Windows device change notifications
 * and logs USB connect/disconnect events with human-friendly details. It can
 * derive vendor/product/type from VID/PID and Windows SetupAPI data.
 *
 * @author SmartXm Development Team
 * @date 2025
 * @version 1.0
 */

#ifndef USBMON_H
#define USBMON_H

#define UNICODE
#define _UNICODE

#include <fstream>
#include <string>
#include <unordered_map>
#include <windows.h>
#include <ctime>
#include <dbt.h>
#include <initguid.h>
#include <devguid.h>
#include <usbiodef.h>
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "setupapi")
#pragma comment(lib, "user32")

using namespace std;

/// @brief Custom message to stop the USB monitor
#define WM_STOP (WM_USER + 1)

/**
 * @class USBMon
 * @brief Monitors USB device changes and logs vendor/product/type information.
 *
 * USBMon creates a message-only window, registers for device interface
 * notifications, and writes timestamped logs whenever USB devices are
 * connected or disconnected. It enriches logs by parsing VID/PID and
 * querying SetupAPI for friendly names and classes.
 */
class USBMon {
private:
    /// Output file stream to store USB connect/disconnect logs
    ofstream usbLog;

    /// Flag indicating whether USB logging is currently active
    bool isLogging;

    /**
     * @struct DeviceInfo
     * @brief Container for resolved device attributes.
     *
     * Combines various resolved attributes for a single USB device interface
     * path so that disconnect events can still display meaningful information
     * even when the device is no longer present in the system.
     */
    struct DeviceInfo {
        /// Human-friendly device name (product/model)
        string friendly;
        /// Categorized type (e.g., "Mass Storage", "Network Adapter")
        string category;
        /// Manufacturer/vendor name (from VID lookup or SetupAPI)
        string manufacturer;
    };

    /// Cache device info by interface path so removals can show resolved names
    unordered_map<string, DeviceInfo> deviceCache; // path -> info

    /// VID -> vendor name lookup table
    unordered_map<unsigned, string> vendorNames;

    /// VID -> PID -> product/model name lookup table
    unordered_map<unsigned, unordered_map<unsigned, string>> productNames;

    /// VID -> PID -> category (type) override lookup table
    unordered_map<unsigned, unordered_map<unsigned, string>> categoryOverrides;

public:
    USBMon();
    ~USBMon();

    /**
     * @brief Starts logging to the specified file, truncating existing content.
     * @param filename Path to the log file.
     * @return true on success, false on failure (and shows a message box).
     */
    bool startLogging(const string &filename);
    /**
     * @brief Stops logging and appends a footer with stop time.
     */
    void stopLogging();
    /**
     * @brief Handles WM_DEVICECHANGE events and writes enriched log lines.
     * @param eventType WM_DEVICECHANGE wParam
     * @param eventData WM_DEVICECHANGE lParam
     */
    void logDeviceChange(WPARAM eventType, LPARAM eventData);

private:
    /**
     * @brief Gets the current system time as a human-readable string.
     * @return Current time string (e.g., "Wed Aug 28 12:34:56 2025")
     * @note Matches ClipMon formatting to enable sharing a helper later.
     */
    string getCurrentTime();
    /**
     * @brief Converts a UTF-16 wide string to UTF-8.
     * @param wstr Input wide string
     * @return Converted UTF-8 encoded string (empty if input empty)
     */
    string wideToUTF8(const wstring &wstr);
    /**
     * @brief Resolves manufacturer, product/friendly name, and category.
     * @param interfacePathW Device interface path (UTF-16) from WM_DEVICECHANGE
     * @return Populated DeviceInfo with any discovered attributes
     * @details Uses VID/PID parsing + lookup tables first, then falls back to
     *          SetupAPI properties and class/GUID heuristics.
     */
    DeviceInfo resolveDeviceInfo(const wstring &interfacePathW);
    /**
     * @brief Initializes lookup tables for known vendors/products/categories.
     * @note Safe to call once in constructor; idempotent within current design.
     */
    void initLookupTables();
    /**
     * @brief Extracts VID and PID hex values from a device interface path.
     * @param path Interface path string (e.g., "\\\\?\\USB#VID_1005&PID_B128#...")
     * @param vid Output vendor ID (hex)
     * @param pid Output product ID (hex)
     * @return true if both VID and PID were successfully parsed
     */
    bool parseVidPid(const string &path, unsigned &vid, unsigned &pid);
};

extern USBMon usbMon;

/**
 * @brief Window procedure for handling device notifications.
 */
LRESULT CALLBACK DeviceProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // USBMON_H