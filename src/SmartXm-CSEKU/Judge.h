#ifndef JUDGE_H
#define JUDGE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<QThread>
#include <QString>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <atomic>
#include<chrono>
#include <QFile>
#include <QTextStream>
#include<cstdio>
#include "verdict.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <thread>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QIODevice>
#include <QProcess>
#include <string>
#include <atomic>
#include <mutex>
#include <QProcess>
#include <QThread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <fcntl.h>
#endif


class Judge
{
public:
    Judge();

private:
#ifdef _WIN32
    QString dirPath = "C:/SmartXm/230201/Editor/";
    QString systemDirPath = "C:/SmartXm/230201/System/";
#else
    QString dirPath = "/SmartXm/230201/Editor/";
    QString systemDirPath = "/SmartXm/230201/System/";
#endif

private:
    std::string currentFile;
    std::string currentProblem;
    std::string pretestCasesPath;



    std::string getFileExtension(const std::string& filename);
    std::string getFileName(const std::string& filepath);
    std::string getDirectoryPath(const std::string& filepath);
    bool checkCompiler(const std::string& ext);
    std::string executeCommand(std::string& command);
    std::string runHiddenCommand(const std::string &cmd);
    double getNormalizeFactor();


    int runWithTimeout(const std::string &exeFile,
                               std::string &inputFile,
                               std::string &outputFile,
                              int timeLimitMS,
                              int memoryLimitKB,
                              long long &usedTimeMS,
                              long long &usedMemoryKB,
                              bool inFlag);
    Verdict isReadyForJudge(int &effectiveTimeLimit,
                                   int &effectiveMemoryLimit,
                            int &effectiveSourceCodeLimit);

    std::vector<Verdict> generateVerdicts(std::string verdict,int n);
    std::string normalize(const std::string &s);


public:
    std::vector<Verdict> runOnTestCases();

    Verdict runOnSingleTestCase();
    void runOnAllStudentsSolution();
    void setCurrentFile(const std::string &cFile);
    void setCurrentProblem(const std::string &cProblem);
    void setPretestCasesPath(const std::string &path);
    void stopJudge(); // 🚀 new function to terminate current process



#ifdef _WIN32
    HANDLE currentProcessHandle = NULL;
    HANDLE currentThreadHandle = NULL;
#else
private:
    long long getProcessMemoryUsageFast(pid_t pid);
    long long getProcessMemoryUsage(pid_t pid);
    long long getChildCpuTimeMS(pid_t pid);
    std::mutex processControlMutex;
    std::atomic<pid_t> currentProcessPid{0};
private:
    bool isProcessRunning(pid_t pid);
    void killProcessSafely(pid_t pid);
#endif

//test case information
private:
    std::string currentTestCaseNo;
    int numberOfTotalTestCase;
    bool stop_flag;
public:
    void setCurrentTestCaseNo(const std::string & testCase);
    void setNumberOfTotalTestCase(int totalTestCase);
//judge information
private:
    bool checkerFlag=0;
    bool inputFlag=1;
    double timeLimit=1000;
    double cppTimeLimit=1;
    double pythonTimeLimit=1;
    double javaTimeLimit=1;
    double memoryLimit=512*1024*1024.0;
    double cppMemoryLimit=1;
    double pythonMemoryLimit=1;
    double javaMemoryLimit=1;
    double sourceCodeLimit=1024*1024;
    double cppSourceCodeLimit=1;
    double pythonSourceCodeLimit=1;
    double javaSourceCodeLimit=1;

public:
    // Setter for judge info
    void setJudgeInfo(const std::vector<double> &judgeInfo);
private:
     std::atomic<bool> stopRequested{false};






};

#endif // JUDGE_H
