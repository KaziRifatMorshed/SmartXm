#ifndef CODERUNNER_H
#define CODERUNNER_H
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define popen _popen
#define pclose _pclose
#else
#include <fcntl.h>
#include <pty.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#endif

class CodeRunner
{
public:
    CodeRunner();


private:
    std::string currentFile;

    std::string executeExeFile(const std::string& exeCommand, int& runtimeError);
    void runCppOrCFile();
    void runPythonFile();

public:
    void setCurrentFile(const std::string& file);
    std::string getFileExtension(const std::string& filename);
    std::string getFileName(const std::string& filepath);
    std::string getDirectoryPath(const std::string& filepath);
    bool checkCompiler(const std::string& ext);
    std::string executeCommand(std::string& command);
    std::string runHiddenCommand(const std::string &cmd);
    void run();
    void stopExecution(); // <-- NEW

private:
#ifdef _WIN32
    HANDLE hProcessHandle = NULL;
    HANDLE hJobHandle = NULL;
#else
    pid_t childPid = -1;
#endif
};


#endif // CODERUNNER_H
