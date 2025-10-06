#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "codeRunner.h"





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

private:
     string currentFile;
     string currentProblem;
     string pretestCasesPath;
     string judgeInfoPath;

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