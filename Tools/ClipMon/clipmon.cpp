/**
 * @file clipmon.cpp
 * @brief Implementation of the ClipMon class for monitoring and logging clipboard changes.
 * 
 * This file contains the implementation of a Windows clipboard monitor that tracks
 * clipboard content changes using Windows clipboard format listeners and logs
 * Unicode text content to a file with timestamps. The monitor is designed for
 * exam monitoring and security purposes.
 * 
 * @author SmartXm Development Team
 * @date 2025
 * @version 1.0
 * 
 * @warning This clipboard monitor should only be used for legitimate purposes such as
 * exam monitoring with proper user consent and in accordance with applicable laws.
 */

#include "clipmon.h"

/// @brief Global ClipMon instance used by the clipboard window procedure
ClipMon clipMon;

/**
 * @brief Starts monitoring clipboard changes and logging to the specified file.
 * @param filename Name of the log file to write clipboard content
 * @return true if monitoring started successfully, false otherwise
 * 
 * Opens the specified file in overwrite mode (ios::out | ios::trunc),
 * writes a timestamped "ClipMon Started" header, and sets the internal
 * monitoring flag to true. If the file cannot be opened, an error message
 * is displayed and monitoring remains disabled.
 * 
 * @note The file is opened in truncation mode, so any existing content will be lost.
 * @see stopLogging()
 */
bool ClipMon::startLogging(const string &filename)
{
    clipboardLog.open(filename, ios::out | ios::trunc);

    if (!clipboardLog.is_open()) {
        MessageBoxA(NULL, "Failed to open log file. Error: " + GetLastError(), "ClipMon Failed", MB_ICONERROR | MB_OK);

        return false;
    }

    isLogging = true;

    clipboardLog << "=== ClipMon Started at: " << getCurrentTime() << " ===\n";

    return true;
}

/**
 * @brief Stops clipboard monitoring and closes the log file.
 * 
 * Writes a timestamped "ClipMon Stopped" footer to the log file,
 * closes the file stream, and sets the monitoring flag to false.
 * If monitoring is not currently active, this function has no effect.
 * 
 * @note This function is safe to call even when monitoring is not active.
 * @see startLogging()
 */
void ClipMon::stopLogging()
{
    if (isLogging) {
        clipboardLog << "\n=== ClipMon Stopped at: " << getCurrentTime() << " ===\n";

        clipboardLog.close();

        isLogging = false;
    }
}

/**
 * @brief Gets the current system time as a human-readable string.
 * @return Current time formatted as a string (e.g., "Wed Aug 28 12:34:56 2025")
 * 
 * Uses the standard C time functions to get the current system time
 * and formats it using ctime(). The trailing newline character from
 * ctime() is removed for cleaner logging output.
 * 
 * @note The returned string format is platform-dependent but typically
 *       follows the format "Day Mon DD HH:MM:SS YYYY"
 */
string ClipMon::getCurrentTime()
{
    time_t now = time(0);

    char *timeStr = ctime(&now);

    string currentTime(timeStr);

    currentTime.pop_back();

    return currentTime;
}

/**
 * @brief Converts a wide string (UTF-16) to UTF-8 encoding.
 * @param wstr Wide string to convert (typically from Windows clipboard)
 * @return UTF-8 encoded string suitable for file logging
 * 
 * Uses the Windows WideCharToMultiByte API to convert Unicode clipboard
 * content to UTF-8 encoding for proper storage and display of international
 * characters in log files. This ensures that clipboard content with
 * non-ASCII characters is properly preserved.
 * 
 * @note Returns an empty string if the input wide string is empty.
 * @see WideCharToMultiByte()
 */
string ClipMon::wideToUTF8(const wstring &wstr)
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
 * @brief Logs the current clipboard content to the log file.
 * 
 * Attempts to retrieve Unicode text content from the Windows clipboard
 * and logs it with a timestamp. The function:
 * 1. Opens the clipboard for reading
 * 2. Retrieves CF_UNICODETEXT format data if available
 * 3. Converts Unicode content to UTF-8 for logging
 * 4. Writes timestamped content with separator lines
 * 5. Immediately flushes output to ensure real-time logging
 * 
 * If monitoring is not active, the clipboard cannot be opened, or no
 * Unicode text is available, the function returns without logging.
 * 
 * @note Only CF_UNICODETEXT clipboard format is supported.
 * @note The output is immediately flushed to ensure real-time logging.
 * @warning Displays error message if clipboard access fails.
 * 
 * @see OpenClipboard(), GetClipboardData(), wideToUTF8()
 */
void ClipMon::logClipboardContent()
{
    if (!isLogging) {
        return;
    }

    if (!OpenClipboard(nullptr)) {
        MessageBoxA(NULL, "Clipboard Monitor failed. Error: " + GetLastError(), "ClipMon Failed", MB_ICONERROR | MB_OK);

        return;
    }

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);

    if (hData) {
        wchar_t *pszText = static_cast <wchar_t *> (GlobalLock(hData));

        if (pszText) {
            wstring wText(pszText);
            string utf8 = wideToUTF8(wText);

            clipboardLog << "[ " << getCurrentTime() << " ]\n";
            clipboardLog << utf8 << "\n====================\n\n";

            clipboardLog.flush();
            GlobalUnlock(hData);
        }
    }

    CloseClipboard();
}

/**
 * @brief Window procedure for handling clipboard-related messages.
 * @param hwnd Handle to the window receiving the message
 * @param msg Message identifier (WM_CREATE, WM_CLIPBOARDUPDATE, etc.)
 * @param wParam Additional message information (depends on message type)
 * @param lParam Additional message information (depends on message type)
 * @return LRESULT value indicating message processing result
 * 
 * This function processes Windows messages for clipboard monitoring:
 * - WM_CREATE: Registers the window as a clipboard format listener using
 *              AddClipboardFormatListener() to receive change notifications
 * - WM_CLIPBOARDUPDATE: Called automatically when clipboard content changes,
 *                       triggers logClipboardContent() to record the change
 * - Other messages: Passed to DefWindowProc() for default processing
 * 
 * @note This function must be declared with CALLBACK calling convention
 *       to work properly with the Windows message system.
 * 
 * @see AddClipboardFormatListener(), ClipMon::logClipboardContent(), DefWindowProc()
 */
LRESULT CALLBACK ClipboardProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        AddClipboardFormatListener(hwnd);
        break;
    case WM_CLIPBOARDUPDATE:
        clipMon.logClipboardContent();
        break;
    
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

/**
 * @brief Entry point of the clipboard monitoring application.
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Exit status code (0 for success, 1 for failure)
 * 
 * This function implements the main clipboard monitoring workflow:
 * 1. Saves the current thread ID to a file for external process control
 * 2. Creates and registers a window class for message handling
 * 3. Creates a message-only window for receiving clipboard notifications
 * 4. Determines the log file name from command line or uses default
 * 5. Starts clipboard monitoring with the specified log file
 * 6. Runs a message loop waiting for clipboard changes or WM_STOP
 * 7. Cleans up and stops monitoring on exit
 * 
 * The application creates a hidden message-only window that receives
 * WM_CLIPBOARDUPDATE messages whenever clipboard content changes.
 * 
 * @param argc If argc > 1, argv[1] is used as the log filename
 * @param argv Command line arguments array
 * 
 * @note The thread ID is saved to "ThreadId/clipMonThread.id" to allow
 *       external processes to send termination messages.
 * 
 * @note Default log file is "Logs/clipboard.log" if no argument provided.
 * 
 * @warning The application may require appropriate permissions to monitor
 *          clipboard changes depending on system security settings.
 * 
 * @see CreateWindowEx(), AddClipboardFormatListener(), ClipMon::startLogging()
 */
int main(int argc, char *argv[])
{
    DWORD clipMonThreadId = GetCurrentThreadId();

    ofstream("ThreadId/clipMonThread.id") << clipMonThreadId;

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    const wchar_t CLASS_NAME[] = L"ClipMon";

    WNDCLASS wc = {};

    wc.lpfnWndProc = ClipboardProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Clipboard Monitor",
        0, 0, 0, 0, 0,
        HWND_MESSAGE, nullptr, hInstance, nullptr);

    if (!hwnd) {
        MessageBoxA(NULL, "Clipboard Monitor failed. Error: " + GetLastError(), "ClipMon Failed", MB_ICONERROR | MB_OK);

        return 1;
    }

    string logFileName = (argc > 1) ? argv[1] : "Logs/clipboard.log";

    if (!clipMon.startLogging(logFileName)) {
        MessageBoxA(NULL, "Failed to start logging", "ClipMon Failed", MB_ICONERROR | MB_OK);

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

    clipMon.stopLogging();

    return 0;
}