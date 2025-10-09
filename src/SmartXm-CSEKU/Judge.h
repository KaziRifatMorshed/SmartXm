#ifndef JUDGE_H
#define JUDGE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <filesystem>

#include <QObject>
#include <QWidget>

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

class Judge
{
public:
    Judge();

private:
    string currentFile;
    string currentProblem;
    string pretestCasesPath;
    string judgeInfoPath;

    std::string getFileExtension(const std::string& filename);
    std::string getFileName(const std::string& filepath);
    std::string getDirectoryPath(const std::string& filepath);
    bool checkCompiler(const std::string& ext);
    std::string executeCommand(std::string& command);
    std::string runHiddenCommand(const std::string &cmd);

    int runWithTimeout(const string &cmd, const string &inputFile,
                       const string &outputFile, int timeLimitMS);
    void runCppOrCFile(int checker,int timeLimit,int numOfTestCases,string testCasesPath);
    void runPythonFile();
    string normalize(const string &s);


public:
    void runOnTestCases();
    void runOnTestCase();
    void runOnAllStudentsSolution();
    void setCurrentFile(const string &cFile);
    void setCurrentProblem(const string &cProblem);
    void setPretestCasesPath(const string &path);
    void setJudgeInfoPath(const string &path);
};

#endif // JUDGE_H
