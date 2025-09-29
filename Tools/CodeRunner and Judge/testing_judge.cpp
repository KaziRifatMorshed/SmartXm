#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <iterator>



#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <fcntl.h>
#endif

using namespace std;
// Run a shell command and capture output
string runCommand(const string &cmd) {
    char buffer[256]; string result = "";
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    while (fgets(buffer, sizeof(buffer), pipe)) result += buffer;
    pclose(pipe);
    return result;
}

// Compile C/C++ code
bool compileCode(const string &file, const string &lang) {
    string cmd;
    if (lang == "c")
        cmd = "gcc " + file + " -o program" + string(
#ifdef _WIN32
        ".exe"
#endif
        ) + " 2> compile_error.txt";
    else if (lang == "cpp")
        cmd = "g++ " + file + " -o program" + string(
#ifdef _WIN32
        ".exe"
#endif
        ) + " 2> compile_error.txt";
    else if (lang == "python")
        return true;

    return (system(cmd.c_str()) == 0);
}

// Run with timeout
int runWithTimeout(const string &cmd, const string &inputFile,
                   const string &outputFile, int timeLimitMS) {
#ifdef _WIN32
    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESTDHANDLES;

    HANDLE hInput = CreateFileA(inputFile.c_str(), GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hOutput = CreateFileA(outputFile.c_str(), GENERIC_WRITE, 0,
                                 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hInput == INVALID_HANDLE_VALUE || hOutput == INVALID_HANDLE_VALUE)
        return -1;

    SetHandleInformation(hInput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hOutput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    si.hStdInput = hInput; si.hStdOutput = si.hStdError = hOutput;

    PROCESS_INFORMATION pi{};
    vector<char> cmdBuf(cmd.begin(), cmd.end()); cmdBuf.push_back('\0');
    if (!CreateProcessA(NULL, cmdBuf.data(), NULL, NULL, TRUE, 0,
                        NULL, NULL, &si, &pi)) {
        CloseHandle(hInput); CloseHandle(hOutput); return -1;
    }

    DWORD res = WaitForSingleObject(pi.hProcess, timeLimitMS);
    DWORD exitCode = 1;
    if (res == WAIT_TIMEOUT) { TerminateProcess(pi.hProcess, 1); exitCode = 2; }
    else { GetExitCodeProcess(pi.hProcess, &exitCode); exitCode = (exitCode == 0) ? 0 : 1; }

    CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    CloseHandle(hInput); CloseHandle(hOutput);
    return exitCode;

#else
    pid_t pid = fork();
    if (pid == 0) {
        int in = open(inputFile.c_str(), O_RDONLY);
        int out = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (in < 0 || out < 0) _exit(1);
        dup2(in, STDIN_FILENO); dup2(out, STDOUT_FILENO); dup2(out, STDERR_FILENO);
        close(in); close(out);
        execl("/bin/sh", "sh", "-c", cmd.c_str(), (char*)NULL);
        _exit(1);
    } else {
        int status = 0, elapsed = 0;
        while (elapsed < timeLimitMS) {
            pid_t r = waitpid(pid, &status, WNOHANG);
            if (r == pid) {
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) return 0;
                return 1;
            }
            usleep(1000 * 10); elapsed += 10;
        }
        kill(pid, SIGKILL); return 2;
    }
#endif
}

// Normalize output
string normalize(const string &s) {
    stringstream ss(s); string line, res="";
    while (getline(ss, line)) {
        size_t st = line.find_first_not_of(" \t\r\n");
        if (st == string::npos) continue;
        size_t en = line.find_last_not_of(" \t\r\n");
        res += line.substr(st, en-st+1) + "\n";
    }
    return res;
}

// ---------------- Main ----------------
int main() {
    string lang, file;
    string inputFile="input.txt", outputFile="output.txt", expectedFile="expected.txt";

    cout << "Enter language (c/cpp/python): "; cin >> lang;
    cout << "Enter source file: "; cin >> file;

    if (!compileCode(file, lang)) {
        cout << "Compilation Error:\n";
#ifdef _WIN32
        cout << runCommand("type compile_error.txt");
#else
        cout << runCommand("cat compile_error.txt");
#endif
        return 0;
    }

    string cmd;
    if (lang == "python") {
#ifdef _WIN32
        cmd = "python " + file;
#else
        cmd = "python3 " + file;
#endif
    } else {
#ifdef _WIN32
        cmd = "program.exe";
#else
        cmd = "./program";
#endif
    }

    int status = runWithTimeout(cmd, inputFile, outputFile, 2000);
    if (status == 2) { cout << "Time Limit Exceeded\n"; return 0; }
    if (status == 1) { cout << "Runtime Error\n"; return 0; }

    ifstream out(outputFile), exp(expectedFile);
    string s1((istreambuf_iterator<char>(out)), {}), s2((istreambuf_iterator<char>(exp)), {});
    cout << ((normalize(s1) == normalize(s2)) ? "Accepted\n" : "Wrong Answer\n");
    return 0;
}
