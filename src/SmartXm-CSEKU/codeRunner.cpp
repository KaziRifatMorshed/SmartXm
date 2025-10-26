#include "codeRunner.h"

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define popen _popen
#define pclose _pclose
#endif
CodeRunner::CodeRunner(){}

std::string CodeRunner::getFileExtension(const std::string &filename)
{
    size_t ind = filename.find_last_of('.');
    if (ind != std::string::npos)
        return filename.substr(ind + 1);
    return "";
}

std::string CodeRunner::getFileName(const std::string &filepath)
{
    size_t lastSlash = filepath.find_last_of("/\\");
    std::string filename = (lastSlash == std::string::npos) ? filepath : filepath.substr(lastSlash + 1);
    size_t lastDot = filename.find_last_of('.');
    return (lastDot == std::string::npos) ? filename : filename.substr(0, lastDot);
}

std::string CodeRunner::getDirectoryPath(const std::string &filepath)
{
    size_t lastSlash = filepath.find_last_of("/\\");
    return (lastSlash == std::string::npos) ? "" : filepath.substr(0, lastSlash + 1);
}

bool CodeRunner::checkCompiler(const std::string &ext)
{
    std::string checkCmd;
    if (ext == "c" || ext == "cpp" || ext == "c++")
    {
#ifdef _WIN32
        checkCmd = "where g++";
#else
        checkCmd = "which g++ >/dev/null 2>&1";
#endif
    }
    else if (ext == "py")
    {
#ifdef _WIN32
        checkCmd = "where python";
#else
        checkCmd = "which python3 >/dev/null 2>&1";
#endif
    }

#ifdef _WIN32
    // --- Silent check without console window ---
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead, hWrite;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
        return false;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;

    std::string wrappedCmd = "cmd.exe /C " + checkCmd + " >nul 2>&1";
    BOOL success = CreateProcessA(
        NULL,
        (LPSTR)wrappedCmd.c_str(),
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi);

    CloseHandle(hWrite);
    if (!success)
    {
        CloseHandle(hRead);
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode = 1;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);

    return (exitCode == 0);
#else
    return system(checkCmd.c_str()) == 0;
#endif
}


#ifdef _WIN32
// Helper: run command silently without showing any console window
std::string CodeRunner::runHiddenCommand(const std::string &cmd)
{
    std::string result;
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead, hWrite;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

           // Create a pipe for capturing output
    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
        return "Error: Pipe creation failed";

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;

    std::string wrappedCmd = "cmd.exe /C " + cmd;

    BOOL success = CreateProcessA(
        NULL,
        (LPSTR)wrappedCmd.c_str(),
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi);

    if (!success)
    {
        CloseHandle(hWrite);
        CloseHandle(hRead);
        return "Error: Failed to execute command.";
    }

    CloseHandle(hWrite);
    char buffer[512];
    DWORD bytesRead;

    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        result += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);

    return result;
}
#endif

std::string CodeRunner::executeCommand(std::string &command)
{
    std::string result;
#ifdef _WIN32
    result = runHiddenCommand(command + " 2>&1");
#else
    command += " 2>&1";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
        return "Error: Failed to execute compile command";
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        result += buffer;
    int exitCode = pclose(pipe);
    if (exitCode != 0)
        result += "\nCommand failed with exit code: " + std::to_string(exitCode);
#endif
    return result;
}


std::string CodeRunner::executeExeFile(const std::string &exeCommand, int &runtimeError)
{
    runtimeError = false;
    std::string result;

#ifdef _WIN32
    const long long MAX_SIZE = 128LL * 1024 * 1024; // 128 MB
    std::string fullCmd = exeCommand + " < input.txt > output.txt 2>>error.txt";

    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

           // Create job
    hJobHandle = CreateJobObject(NULL, NULL);
    if (hJobHandle) {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobLimit = {};
        jobLimit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        SetInformationJobObject(hJobHandle, JobObjectExtendedLimitInformation, &jobLimit, sizeof(jobLimit));
    }

    if (!CreateProcessA(NULL, (LPSTR)("cmd.exe /C " + fullCmd).c_str(),
                        NULL, NULL, FALSE,
                        CREATE_NO_WINDOW | CREATE_BREAKAWAY_FROM_JOB,
                        NULL, NULL, &si, &pi))
    {
        if (hJobHandle) CloseHandle(hJobHandle);
        return "Error: Failed to execute command.";
    }

           // Save handle for external termination
    hProcessHandle = pi.hProcess;

    if (hJobHandle)
        AssignProcessToJobObject(hJobHandle, pi.hProcess);

    bool terminated = false;

    while (WaitForSingleObject(pi.hProcess, 100) == WAIT_TIMEOUT)
    {
        try {
            auto size = std::filesystem::file_size("output.txt");
            if (size > MAX_SIZE) {
                stopExecution(); // <-- USE new method

                terminated = true;
                runtimeError = true;
                WaitForSingleObject(pi.hProcess, 2000);
                std::ofstream errorFile("error.txt", std::ios::app);
                errorFile << "Error: Output exceeded 128 MB. Process terminated.\n";
                errorFile.flush(); // make sure it's written immediately
                errorFile.close();




                break;
            }
        } catch (...) {
            std::cout<<"catch\n";
        }
    }

    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    if (hJobHandle) CloseHandle(hJobHandle);

    hProcessHandle = NULL;
    hJobHandle = NULL;

    if (exitCode != 0 && !terminated) {
        runtimeError = exitCode;

        std::ofstream err("error.txt", std::ios::app);
        if (exitCode == 0xC0000094)
            err << "Error: Division by zero occurred.\n";
        err.close();
    }

#else
    // ---------------- Linux / Unix ----------------
    const long long MAX_SIZE = 128LL * 1024 * 1024;

    int stdoutFd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int stderrFd = open("error.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    int stdinFd = open("input.txt", O_RDONLY);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        setsid(); // create new session
        if (stdoutFd >= 0) dup2(stdoutFd, STDOUT_FILENO);
        if (stderrFd >= 0) dup2(stderrFd, STDERR_FILENO);
        if (stdinFd >= 0) dup2(stdinFd, STDIN_FILENO);

        execl("/bin/sh", "sh", "-c", exeCommand.c_str(), (char*) nullptr);
        _exit(127); // if exec fails
    } else if (pid > 0) {
        // Parent process
        childPid = pid;

        if (stdoutFd >= 0) close(stdoutFd);
        if (stderrFd >= 0) close(stderrFd);
        if (stdinFd >= 0) close(stdinFd);

        bool terminated = false;
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            try {
                auto size = std::filesystem::file_size("output.txt");
                if (size > MAX_SIZE) {
                    stopExecution();
                    std::ofstream fout("error.txt", std::ios::app);
                    fout << "Error: Output exceeded 128 MB. Process terminated.\n";
                    fout.close();
                    runtimeError = true;
                    terminated = true;
                    break;
                }
            } catch (...) {}

                   // Check if process finished
            int status;
            pid_t ret = waitpid(childPid, &status, WNOHANG);
            if (ret > 0) break;
        }



        childPid = -1;
    } else {
        return "Error: fork() failed.";
    }
#endif

    return result;
}

void CodeRunner::stopExecution()
{
#ifdef _WIN32


    if (hJobHandle)
    {
        TerminateJobObject(hJobHandle, 1);
        CloseHandle(hJobHandle);
        hJobHandle = NULL;
    }
    else if (hProcessHandle)
    {
        TerminateProcess(hProcessHandle, 1);
        CloseHandle(hProcessHandle);
        hProcessHandle = NULL;
    }

#else
    std::cout<<"i am here\n";
    if (childPid > 0)
    {
        kill(-childPid, SIGKILL); // kill entire process group
        childPid = -1;
    }

#endif


}



void CodeRunner::setCurrentFile(const std::string &file)
{
    currentFile = file;
}

void CodeRunner::run()
{
    std::ofstream("error.txt", std::ios::trunc).close();
    std::ofstream("output.txt", std::ios::trunc).close();

    if (currentFile.empty())
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout << "Error: No file specified.\n";
        fout.close();
        return;
    }

    if (!std::filesystem::exists(currentFile))
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout<< "Error: File '" << currentFile << "' does not exist.\n";
        fout.close();
        return;
    }

    if (!std::filesystem::exists("input.txt"))
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout<< "Error: File 'input.txt' does not exist.\n";
        fout.close();
        return;
    }

    std::string ext = getFileExtension(currentFile);
    if (ext != "c" && ext != "cpp" && ext != "c++" && ext != "py")
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout<< "Error: Unsupported file type " << ext << ".\n";
        fout.close();
        return;
    }

    if (!checkCompiler(ext))
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout << "Error: Required compiler/interpreter not found for ." << ext << " files\n";
        if (ext == "c" || ext == "cpp" || ext == "c++")
            fout << "Please install GNU g++ compiler\n";
        else if (ext == "py")
            fout << "Please install Python\n";
        fout.close();
        return;
    }

    if (ext == "c" || ext == "cpp" || ext == "c++")
        runCppOrCFile();
    else if (ext == "py")
        runPythonFile();
}

void CodeRunner::runCppOrCFile()
{
    std::string filename = getFileName(currentFile);
    std::string directoryPath = getDirectoryPath(currentFile);
    std::string exeFile, compileCmd;

#ifdef _WIN32
    exeFile = "\"" + directoryPath + filename + ".exe" + "\"";
    compileCmd = "g++ -O2  -march=x86-64 -mtune=generic -pipe -s -static -DONLINE_JUDGE -fno-asm \"" + currentFile + "\" -o " + exeFile + " -Wall";
#else
    exeFile = "\"" + directoryPath + "./" + filename + "\"";
    compileCmd = "g++ -O2 -fsanitize=address -g \"" + currentFile + "\" -o " + exeFile + " -Wall";
#endif

    std::string compileOutput = executeCommand(compileCmd);
    if (!compileOutput.empty())
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout  << compileOutput << std::endl;
        fout.close();
        if (compileOutput.find("error") != std::string::npos)
        {
            exeFile.pop_back();
            exeFile.erase(exeFile.begin());
            std::remove(exeFile.c_str());
            return;
        }
    }

    if (!std::filesystem::exists(exeFile.substr(1, exeFile.size() - 2)))
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout<< "Compilation Failed!\n";
        fout.close();
        exeFile.pop_back();
        exeFile.erase(exeFile.begin());
        std::remove(exeFile.c_str());
        return;
    }

    int exitCode;
    std::string error = executeExeFile(exeFile, exitCode);
    exeFile.pop_back();
    exeFile.erase(exeFile.begin());
    std::remove(exeFile.c_str());

    if (exitCode)
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout << "Runtime error occurred with exit code: " << exitCode << "\n";
        fout.close();
        return;
    }


}

void CodeRunner::runPythonFile()
{
#ifdef _WIN32
    std::string pythonCmd = "python";
#else
    std::string pythonCmd = "python3";
#endif
    std::string runCmd = pythonCmd + " \"" + currentFile + "\"";
    int exitCode;
    std::string error = executeExeFile(runCmd, exitCode);
    std::ofstream fout("error.txt", std::ios::app);
    fout <<error<<std::endl;
    fout.close();
    if (exitCode)
    {
        std::ofstream fout("error.txt", std::ios::app);
        fout<<"Runtime error occurred with exit code: "<<exitCode<<std::endl;
        fout.close();
    }
}
