#include <windows.h>  // Windows API functions, structures, constants
#include <string>     // std::string

using namespace std;  // Use the standard namespace for convenience

// Define a custom Windows message to stop the keylogger
#define WM_STOP (WM_USER + 1)

int main()
{
    // Name of the file where the keylogger will save the logged keys
    string keyloggerFileName = "230201.key";

    // Command to start the keylogger executable and pass the file name as argument
    string startKeyloggerCmd = "keylogger.exe " + keyloggerFileName;

    // Structures used to start a new process
    STARTUPINFOA si;       // Holds info about how to start the process
    PROCESS_INFORMATION pi; // Receives info about the started process

    // Initialize the structures to zero
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);    // Must set the size of STARTUPINFOA
    ZeroMemory(&pi, sizeof(pi));

    // Create the keylogger process
    if (!CreateProcessA(
        NULL,                   // Application name (NULL because it's in command line)
        (LPSTR) startKeyloggerCmd.c_str(), // Command line
        NULL,                   // Process security attributes
        NULL,                   // Thread security attributes
        FALSE,                  // Inherit handles? No
        0,                      // Creation flags
        NULL,                   // Environment block
        NULL,                   // Current directory
        &si,                    // Pointer to STARTUPINFOA
        &pi))                   // Pointer to PROCESS_INFORMATION
    {
        // If process creation fails, show an error message box
        MessageBoxA(
            NULL,
            ("Failed to start keylogger. Error: " + to_string(GetLastError())).c_str(), // Show the error code
            "Keylogger Failed",
            MB_ICONERROR | MB_OK
        );

        return 1; // Exit with error code
    }

    // Wait 10 seconds before attempting to stop the keylogger
    Sleep(10000);

    // Send the custom WM_STOP message to the keylogger thread
    if (!PostThreadMessage(pi.dwThreadId, WM_STOP, 0, 0)) {
        // If sending the message fails, show a warning and forcibly terminate the process
        MessageBoxA(
            NULL,
            ("Failed to send WM_STOP. Error: " + to_string(GetLastError())).c_str(),
            "Keylogger Didn't Close Properly",
            MB_ICONWARNING | MB_OK
        );
        
        TerminateProcess(pi.hProcess, 0); // Forcefully kill the keylogger process
    }

    // Wait for the keylogger process to exit (infinite wait)
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close handles to the process and thread to free resources
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0; // Exit successfully
}
