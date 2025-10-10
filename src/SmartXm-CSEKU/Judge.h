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

#include "verdict.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#endif


class Judge
{
public:
    Judge();
private:
#ifdef _WIN32
    QString dirPath = "C:/SmartXM/230201/Editor/";
    QString systemDirPath = "C:/SmartXM/230201/System/";
#else
    QString dirPath = "/SmartXM/230201/Editor/";
    QString systemDirPath = "/SmartXM/230201/System/";
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

    int runWithTimeout(const std::string &exeFile,
                              const std::string &inputFile,
                              const std::string &outputFile,
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
    pid_t currentPid = -1;
#endif

//test case information
private:
    std::string currentTestCaseNo;
    int numberOfTotalTestCase;
public:
    void setCurrentTestCaseNo(const std::string & testCase);
    void setNumberOfTotalTestCase(int totalTestCase);
//judge information
private:
    bool checkerFlag;
    bool inputFlag;
    double timeLimit;
    double cppTimeLimit;
    double pythonTimeLimit;
    double javaTimeLimit;
    double memoryLimit;
    double cppMemoryLimit;
    double pythonMemoryLimit;
    double javaMemoryLimit;
    double sourceCodeLimit;
    double cppSourceCodeLimit;
    double pythonSourceCodeLimit;
    double javaSourceCodeLimit;

public:
    // Setter for judge info
    void setJudgeInfo(const std::vector<double> &judgeInfo);
private:
     std::atomic<bool> stopRequested{false};

};

#endif // JUDGE_H
