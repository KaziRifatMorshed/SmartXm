/**
 * @file clipmon.h
 * @brief Header file for ClipMon - Clipboard monitoring and logging utility.
 * 
 * This header defines the ClipMon class that monitors Windows clipboard changes
 * and logs clipboard content to a file. It uses Windows clipboard format listeners
 * to detect changes and supports Unicode text content.
 * 
 * @author SmartXm Development Team
 * @date 2025
 * @version 1.0
 * 
 * @warning This clipboard monitor should only be used for legitimate purposes such as
 * exam monitoring with proper user consent and in accordance with applicable laws.
 */

#ifndef CLIPMON_H
#define CLIPMON_H
#define UNICODE
#define _UNICODE

#include <fstream>      // For file operations (ofstream)
#include <windows.h>    // For Windows API (clipboard, window procedures)
#include <ctime>        // For timestamps

using namespace std;

/// @brief Custom message to stop the clipboard monitor
#define WM_STOP (WM_USER + 1)

/**
 * @class ClipMon
 * @brief Monitors Windows clipboard changes and logs content to a file.
 *
 * The ClipMon class provides functionality to start and stop clipboard monitoring,
 * automatically detecting clipboard content changes and logging Unicode text content
 * with timestamps. It uses Windows clipboard format listeners for real-time monitoring.
 */
class ClipMon {
private:
    /// Output file stream to store clipboard content
    ofstream clipboardLog;

    /// Flag indicating whether clipboard monitoring is active
    bool isLogging;

public:
    /**
     * @brief Constructs a ClipMon instance with monitoring disabled.
     * 
     * Initializes the clipboard monitor with logging set to false.
     * Call startLogging() to begin monitoring clipboard changes.
     */
    ClipMon() : isLogging(false) {}

    /**
     * @brief Starts monitoring clipboard changes and logging to the specified file.
     * @param filename Name of the log file to write clipboard content
     * @return true if monitoring started successfully, false otherwise
     * 
     * Opens the log file in truncation mode and begins monitoring clipboard changes.
     * If the file cannot be opened, monitoring remains disabled.
     */
    bool startLogging(const string &filename);

    /**
     * @brief Stops clipboard monitoring and closes the log file.
     * 
     * Writes a timestamped footer to the log file, closes the file stream,
     * and disables monitoring. Safe to call even when monitoring is not active.
     */
    void stopLogging();

    /**
     * @brief Logs the current clipboard content to the log file.
     * 
     * Retrieves Unicode text content from the Windows clipboard and logs it
     * with a timestamp. If the clipboard is empty or contains non-text data,
     * nothing is logged. This function is typically called automatically
     * when clipboard changes are detected.
     * 
     * @note Only CF_UNICODETEXT format is supported for logging.
     */
    void logClipboardContent();

private:
    /**
     * @brief Gets the current system time as a human-readable string.
     * @return Current time formatted as a string (e.g., "Wed Aug 28 12:34:56 2025")
     * 
     * Uses standard C time functions to format the current system time.
     * The trailing newline from ctime() is removed for cleaner output.
     */
    string getCurrentTime();

    /**
     * @brief Converts a wide string (UTF-16) to UTF-8 encoding.
     * @param wstr Wide string to convert
     * @return UTF-8 encoded string
     * 
     * Uses Windows WideCharToMultiByte API to convert Unicode clipboard
     * content to UTF-8 for proper logging of international characters.
     * Returns empty string if input is empty.
     */
    string wideToUTF8(const wstring &wstr);
};

/// @brief Global ClipMon instance accessible from clipboard window procedure
extern ClipMon clipMon;

/**
 * @brief Window procedure for handling clipboard-related messages.
 * @param hwnd Handle to the window receiving the message
 * @param msg Message identifier (WM_CREATE, WM_CLIPBOARDUPDATE, etc.)
 * @param wParam Additional message information (depends on message type)
 * @param lParam Additional message information (depends on message type)
 * @return LRESULT value indicating message processing result
 * 
 * This function handles Windows messages for clipboard monitoring:
 * - WM_CREATE: Registers the window as a clipboard format listener
 * - WM_CLIPBOARDUPDATE: Called when clipboard content changes, triggers logging
 * - Other messages: Passed to default window procedure
 * 
 * @note This function must be declared with CALLBACK calling convention
 *       to work properly with the Windows message system.
 * 
 * @see AddClipboardFormatListener(), ClipMon::logClipboardContent()
 */
LRESULT CALLBACK ClipboardProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // CLIPMON_H