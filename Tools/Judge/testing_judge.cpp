#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

// -------------------- Helper Functions --------------------

// Run a shell command and capture output
string runCommand(const string &cmd) {
    char buffer[256];
    string result = "";
    FILE *pipe = _popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
        result += buffer;
    _pclose(pipe);
    return result;
}

// Compile C/C++ code
bool compileCode(const string &file, const string &lang) {
    string cmd;
    if (lang == "c")
        cmd = "gcc " + file + " -o program.exe 2> compile_error.txt";
    else if (lang == "cpp")
        cmd = "g++ " + file + " -o program.exe 2> compile_error.txt";
    else if (lang == "python")
        return true; // no compilation needed

    int res = system(cmd.c_str());
    return (res == 0);
}

// Run program with input/output redirection and timeout
int runWithTimeout(string cmd, string inputFile,
                   string outputFile, int timeLimitMS) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;

    HANDLE hInput = CreateFileA(inputFile.c_str(), GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hOutput = CreateFileA(outputFile.c_str(), GENERIC_WRITE, 0,
                                 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hInput == INVALID_HANDLE_VALUE || hOutput == INVALID_HANDLE_VALUE) {
        cerr << "Failed to open input/output files" << endl;
        return -1;
    }

    SetHandleInformation(hInput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hOutput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    si.hStdInput = hInput;
    si.hStdOutput = hOutput;
    si.hStdError = hOutput; // redirect stderr to output

    ZeroMemory(&pi, sizeof(pi));

    vector<char> cmdBuffer(cmd.begin(), cmd.end());
    cmdBuffer.push_back('\0');

    if (!CreateProcessA(NULL, cmdBuffer.data(), NULL, NULL, TRUE, 0,
                        NULL, NULL, &si, &pi)) {
        cerr << "CreateProcess failed, error: " << GetLastError() << endl;
        CloseHandle(hInput);
        CloseHandle(hOutput);
        return -1;
    }

    DWORD result = WaitForSingleObject(pi.hProcess, timeLimitMS);

    if (result == WAIT_TIMEOUT) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hInput);
        CloseHandle(hOutput);
        return 2; // TLE
    }

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hInput);
    CloseHandle(hOutput);

    if (exitCode == 0) return 0;
    else return 1; // runtime error
}

// Normalize output: remove extra spaces, tabs, blank lines
string normalize(const string &s) {
    stringstream ss(s);
    string line, result = "";
    while (getline(ss, line)) {
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == string::npos) continue;
        size_t end = line.find_last_not_of(" \t\r\n");
        result += line.substr(start, end - start + 1) + "\n";
    }
    return result;
}

// Run custom checker
int runChecker(const string &checkerCmd) {
    int res = system(checkerCmd.c_str());
    if (res == 0) return 0; // AC
    else return 1;          // WA
}

// -------------------- Main Judge --------------------
int main() {
    string lang, file;
    string inputFile = "input.txt", outputFile = "output.txt", expectedFile = "expected.txt";

    cout << "Enter language (c/cpp/python): ";
    cin >> lang;
    cout << "Enter source file: ";
    cin >> file;

    // Step 1: Compile
    if (!compileCode(file, lang)) {
        cout << "Compilation Error:\n";
        cout << runCommand("type compile_error.txt");
        return 0;
    }

    // Step 2: Run
    string cmd;
    if (lang == "python")
        cmd = "python " + file;
    else
        cmd = "program.exe";

    int status = runWithTimeout(cmd, inputFile, outputFile, 2000); // 2 sec limit

    if (status == 2) {
        cout << "Time Limit Exceeded\n";
        return 0;
    } else if (status == 1) {
        cout << "Runtime Error\n";
        return 0;
    }

    // Step 3: Decide how to check output
    bool useChecker = false; // change to true if problem has multiple solutions

    if (useChecker) {
        string checkerCmd = "checker.exe " + expectedFile + " " + outputFile;
        int checkerResult = runChecker(checkerCmd);
        if (checkerResult == 0)
            cout << "Accepted\n";
        else
            cout << "Wrong Answer\n";
    } else {
        // single-solution problem: normalize & compare
        ifstream out(outputFile), exp(expectedFile);
        string s1((istreambuf_iterator<char>(out)), istreambuf_iterator<char>());
        string s2((istreambuf_iterator<char>(exp)), istreambuf_iterator<char>());

        if (normalize(s1) == normalize(s2))
            cout << "Accepted\n";
        else
            cout << "Wrong Answer\n";
    }

    return 0;
}
 