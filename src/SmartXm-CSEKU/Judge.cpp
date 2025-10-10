#include "Judge.h"

Judge::Judge() {}

std::string Judge::getFileExtension(const std::string &filename)
{
    size_t ind = filename.find_last_of('.');
    if (ind != std::string::npos)
        return filename.substr(ind + 1);
    return "";
}

std::string Judge::getFileName(const std::string &filepath)
{
    size_t lastSlash = filepath.find_last_of("/\\");
    std::string filename = (lastSlash == std::string::npos) ? filepath : filepath.substr(lastSlash + 1);
    size_t lastDot = filename.find_last_of('.');
    return (lastDot == std::string::npos) ? filename : filename.substr(0, lastDot);
}

std::string Judge::getDirectoryPath(const std::string &filepath)
{
    size_t lastSlash = filepath.find_last_of("/\\");
    return (lastSlash == std::string::npos) ? "" : filepath.substr(0, lastSlash + 1);
}

bool Judge::checkCompiler(const std::string &ext)
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
std::string Judge::runHiddenCommand(const std::string &cmd)
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

std::string Judge::executeCommand(std::string &command)
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

int Judge::runWithTimeout(const std::string &cmd, const std::string &inputFile,
                          const std::string &outputFile, int timeLimitMS,
                          int memoryLimitKB, long long &usedTimeMS, long long &usedMemoryKB)
{
#ifdef _WIN32
    STARTUPINFOA si = {sizeof(si)};
    si.dwFlags = STARTF_USESTDHANDLES;

    HANDLE hInput = CreateFileA(inputFile.c_str(), GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hOutput = CreateFileA(outputFile.c_str(), GENERIC_WRITE, 0,
                                 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hInput == INVALID_HANDLE_VALUE || hOutput == INVALID_HANDLE_VALUE)
        return -1;

    SetHandleInformation(hInput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hOutput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    si.hStdInput = hInput;
    si.hStdOutput = si.hStdError = hOutput;

    PROCESS_INFORMATION pi{};
    std::vector<char> cmdBuf(cmd.begin(), cmd.end());
    cmdBuf.push_back('\0');

    if (!CreateProcessA(NULL, cmdBuf.data(), NULL, NULL, TRUE,
                        CREATE_SUSPENDED, NULL, NULL, &si, &pi))
    {
        CloseHandle(hInput);
        CloseHandle(hOutput);
        return -1;
    }
    currentProcessHandle = pi.hProcess;
    currentThreadHandle = pi.hThread;

           // Create Job Object to set memory limit
    HANDLE hJob = CreateJobObject(NULL, NULL);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobInfo{};
    jobInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_PROCESS_MEMORY;
    jobInfo.ProcessMemoryLimit = (SIZE_T)memoryLimitKB * 1024;
    SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jobInfo, sizeof(jobInfo));
    AssignProcessToJobObject(hJob, pi.hProcess);

           // Start timing
    auto start = std::chrono::steady_clock::now();
    ResumeThread(pi.hThread);

    DWORD res = WaitForSingleObject(pi.hProcess, timeLimitMS);
    auto end = std::chrono::steady_clock::now();
    usedTimeMS = std::chrono::duration<double, std::milli>(end - start).count();

    DWORD exitCode = 1;
    PROCESS_MEMORY_COUNTERS pmc{};
    GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof(pmc));
    usedMemoryKB = pmc.PeakWorkingSetSize / 1024;

    if (res == WAIT_TIMEOUT)
    {
        TerminateProcess(pi.hProcess, 1);
        exitCode = 2;
    }
    else
    {
        GetExitCodeProcess(pi.hProcess, &exitCode);
        exitCode = (exitCode == 0) ? 0 : 1;
    }

    CloseHandle(hJob);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hInput);
    CloseHandle(hOutput);

    currentProcessHandle = NULL;
    currentThreadHandle = NULL;
    return exitCode;

#else
    pid_t pid = fork();
    if (pid == 0)
    {
        // Apply memory limit
        struct rlimit memLimit;
        memLimit.rlim_cur = memLimit.rlim_max = (rlim_t)memoryLimitKB * 1024;
        setrlimit(RLIMIT_AS, &memLimit);

        int in = open(inputFile.c_str(), O_RDONLY);
        int out = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (in < 0 || out < 0)
            _exit(1);
        dup2(in, STDIN_FILENO);
        dup2(out, STDOUT_FILENO);
        dup2(out, STDERR_FILENO);
        close(in);
        close(out);
        execl("/bin/sh", "sh", "-c", cmd.c_str(), (char *)NULL);
        _exit(1);
    }
    else
    {
        currentPid = pid;
        auto start = std::chrono::steady_clock::now();
        int status = 0, elapsed = 0;
        usedMemoryKB = 0;

        while (elapsed < timeLimitMS)
        {
            pid_t r = waitpid(pid, &status, WNOHANG);
            if (r == pid)
            {
                struct rusage usage;
                getrusage(RUSAGE_CHILDREN, &usage);
                usedTimeMS = std::chrono::duration<double, std::milli>(
                                 std::chrono::steady_clock::now() - start)
                                 .count();
                usedMemoryKB = usage.ru_maxrss; // KB on Linux

                if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                    return 0;
                return 1;
            }

                   // Check memory usage periodically
            std::string statmPath = "/proc/" + std::to_string(pid) + "/status";
            std::ifstream statusFile(statmPath);
            std::string line;
            while (std::getline(statusFile, line))
            {
                if (line.rfind("VmPeak:", 0) == 0)
                {
                    long memKB = 0;
                    sscanf(line.c_str(), "VmPeak: %ld kB", &memKB);
                    usedMemoryKB = std::max(usedMemoryKB, (long long)memKB);
                    if (memKB > memoryLimitKB)
                    {
                        kill(pid, SIGKILL);
                        return 3; // Memory limit exceeded
                    }
                    break;
                }
            }

            usleep(1000 * 10);
            elapsed += 10;
        }

        kill(pid, SIGKILL);
        currentPid = -1;
        usedTimeMS = timeLimitMS;
        return 2; // Timeout
    }
#endif
}


void Judge::setCurrentFile(const std::string &cFile)
{
    currentFile = cFile;
}

void Judge::setCurrentProblem(const std::string &cProblem)
{
    currentProblem = cProblem;
}

void Judge::setPretestCasesPath(const std::string &path)
{
    pretestCasesPath = path;
}



Verdict Judge::runOnSingleTestCase(std::vector <std::string> &testcaseData,std::vector<double>&judgeInfo)
{
    setCurrentFile(dirPath.toStdString()+testcaseData[2]);
    setCurrentProblem(std::to_string(testcaseData[0][0]));
    setPretestCasesPath(systemDirPath.toStdString()+"Pretest/"+currentProblem+"/");
    std::string testCaseNo=testcaseData[1];

    std::string filename = getFileName(currentFile);
    std::string directoryPath = getDirectoryPath(currentFile);

    while(testCaseNo.size()>0)
    {
        if(*testCaseNo.begin()=='#')
        {
            testCaseNo.erase(testCaseNo.begin());
            break;
        }
        else
        {
            testCaseNo.erase(testCaseNo.begin());
        }
    }
    std::string inputFile = pretestCasesPath + testCaseNo + ".in";
    std::string outputFile = pretestCasesPath + testCaseNo + ".output";
    std::string expectedFile = pretestCasesPath + testCaseNo + ".out";
    std::ofstream output(outputFile);
    output.close();
    std::string verdict;
    long long cpuTime;
    long long memorySize;
    bool checkerFlag=judgeInfo[0];
    bool intputFlag=judgeInfo[1];
    double timeLimit=judgeInfo[2];
    double cppTimeLimit=judgeInfo[3];
    double pythonTimeLimit=judgeInfo[4];
    double javaTimeLimit=judgeInfo[5];
    double memoryLimit=judgeInfo[6];
    double cppMemoryLimit=judgeInfo[7];
    double pythonMemoryLimit=judgeInfo[8];
    double javaMemoryLimit=judgeInfo[9];
    double sourceCodeLimit=judgeInfo[10];
    double cppSourceCodeLimit=judgeInfo[11];
    double pythonSourceCodeLimit=judgeInfo[12];
    double javaSourceCodeLimit=judgeInfo[13];


        if (currentFile.empty())
        {

            return Verdict("Solution File Not Found",0,0);
        }

        if (!std::filesystem::exists(currentFile))
        {
            return Verdict("Solution File Not Exist",0,0);
        }

        if (currentProblem.empty())
        {
            return Verdict("Problem Not Specified",0,0);
        }
        std::string ext = getFileExtension(currentFile);

        int effectiveTimeLimit;
        int effectiveMemoryLimit;
        int effectiveSourceCodeLimit;

        if (ext == "c")
        {
            effectiveTimeLimit       = timeLimit * cppTimeLimit;
            effectiveMemoryLimit     = memoryLimit * cppMemoryLimit;
            effectiveSourceCodeLimit = sourceCodeLimit * cppSourceCodeLimit;
        }
        else if (ext == "py")
        {
            effectiveTimeLimit       = timeLimit * pythonTimeLimit;
            effectiveMemoryLimit     = memoryLimit * pythonMemoryLimit;
            effectiveSourceCodeLimit = sourceCodeLimit * pythonSourceCodeLimit;
        }
        else // assume java
        {
            effectiveTimeLimit       = timeLimit * javaTimeLimit;
            effectiveMemoryLimit     = memoryLimit * javaMemoryLimit;
            effectiveSourceCodeLimit = sourceCodeLimit * javaSourceCodeLimit;
        }

        try {
            auto size = std::filesystem::file_size(currentFile);
            if(size>effectiveSourceCodeLimit)
            {
                return Verdict("Source Code Limit Exceeded",0,0);
            }
        } catch (std::filesystem::filesystem_error &e) {
            return Verdict("Source Code Error",0,0);
        }

        if(!checkCompiler(ext))
        {
            return Verdict("Compilation Failed",0,0);
        }

        if(ext=="c"||ext=="cpp"||ext=="c++")
        {
            std::string exeFile, compileCmd;
#ifdef _WIN32
            exeFile = "\"" + directoryPath + filename + "-judge.exe" + "\"";
            compileCmd = "g++ \"" + currentFile + "\" -o " + exeFile + " -Wall";
#else
            exeFile = "\"" + directoryPath + "./" + filename +"-judge"+"\"";
            compileCmd = "g++ -O2 -fsanitize=address -g \"" + currentFile + "\" -o " + exeFile + " -Wall ";
#endif

            std::string compileOutput = executeCommand(compileCmd);

            if (!compileOutput.empty())
            {
                if (compileOutput.find("error") != std::string::npos)
                {

                    return Verdict("Compilation Error",0,0);
                }
            }
            if (!std::filesystem::exists(exeFile.substr(1, exeFile.size() - 2)))
            {
                return Verdict("Compilation Failed",0,0);
            }

            int status=runWithTimeout(exeFile,inputFile,outputFile,effectiveTimeLimit,effectiveMemoryLimit,
                                        cpuTime,memorySize);
            if(status==0)
            {

            }
            else if(status==1)
            {
                verdict="Runtime Error";
                return Verdict(verdict,cpuTime,memorySize);
            }
            else if(status==2)
            {
                verdict="Time Limit Exceeded";
                return Verdict(verdict,cpuTime,memorySize);
            }
            else if(status==3)
            {
                verdict="Memory Limit Exceeded";
                return Verdict(verdict,cpuTime,memorySize);
            }
            else
            {
                verdict="I/O Error";
                return Verdict(verdict,cpuTime,memorySize);
            }
            if(!checkerFlag)
            {
                std::ifstream out(outputFile), exp(expectedFile);
                if (!out || !exp)
                {

                    return Verdict("Error",cpuTime,memorySize);
                }
                std::string s1((std::istreambuf_iterator<char>(out)), {});
                std::string s2((std::istreambuf_iterator<char>(exp)), {});
                if (normalize(s1) == normalize(s2))
                {
                    return Verdict("Accepted",cpuTime,memorySize);
                }
                else
                {
                    return Verdict("Wrong Answer",cpuTime,memorySize);
                }

            }

        }

}
void Judge::stopJudge()
{
#ifdef _WIN32
    if (currentProcessHandle)
    {
        TerminateProcess(currentProcessHandle, 1);
        CloseHandle(currentProcessHandle);
        if (currentThreadHandle)
            CloseHandle(currentThreadHandle);
        currentProcessHandle = NULL;
        currentThreadHandle = NULL;
    }
#else
    if (currentPid > 0)
    {
        kill(currentPid, SIGKILL);
        waitpid(currentPid, nullptr, 0);
        currentPid = -1;
    }
#endif
}


// void Judge::runOnTestCases()
// {
//     std::ofstream fout("verdict.txt", std::ios::trunc);
//     fout.close();

//     if (currentFile.empty())
//     {
//         std::ofstream fout("verdict.txt", std::ios::app);
//         fout << "Error: No file specified.\n" << std::endl;
//         fout.close();
//         return;
//     }

//     if (!std::filesystem::exists(currentFile))
//     {
//         std::ofstream fout("verdict.txt", std::ios::app);
//         fout << "Error : File '" << currentFile << "' does not exist.\n";
//         fout.close();
//         return;
//     }

//     if (currentProblem.empty())
//     {
//         std::ofstream fout("verdict.txt", std::ios::app);
//         fout << "Error: No problem specified.\n" << std::endl;
//         fout.close();
//         return;
//     }

//     std::ofstream foutt("verdict.txt", std::ios::app);
//     foutt << "Problem: " << currentProblem << std::endl;
//     foutt.close();

//     double timeLimit, cTimeLimit, cppTimeLimit, pyTimeLimit, javaTimeLimit;
//     int checker;
//     int numberOfTestCases;

// #ifdef _WIN32
//     judgeInfoPath += "\\";
//     pretestCasesPath += "\\" + currentProblem + "\\";
// #else
//     judgeInfoPath += "/";
//     pretestCasesPath += "/" + currentProblem + "/";
// #endif

//     std::string judgeInfoFile = judgeInfoPath + currentProblem + ".txt";
//     std::cout << judgeInfoFile << std::endl;

//     std::ifstream fin(judgeInfoFile);
//     if (!fin)
//     {
//         std::ofstream fout("verdict.txt", std::ios::app);
//         fout << "Judge Information not found.\n";
//         fout.close();
//         return;
//     }

//     fin >> checker >> timeLimit >> cTimeLimit >> cppTimeLimit >> pyTimeLimit >> javaTimeLimit >> numberOfTestCases;
//     fin.close();

//     std::string ext = getFileExtension(currentFile);
//     int effectiveTimeLimit;
//     if (ext == "c")
//         effectiveTimeLimit = timeLimit * cTimeLimit * 1000;
//     else if (ext == "cpp")
//         effectiveTimeLimit = timeLimit * cppTimeLimit * 1000;
//     else if (ext == "py")
//         effectiveTimeLimit = timeLimit * pyTimeLimit * 1000;
//     else
//         effectiveTimeLimit = timeLimit * javaTimeLimit * 1000;

//     if (!checkCompiler(ext))
//     {
//         std::ofstream fout("verdict.txt", std::ios::app);
//         fout << "Error: Required compiler/interpreter not found for ." << ext << " files\n";

//         if (ext == "c" || ext == "cpp" || ext == "c++")
//             fout << "Please install GNU g++ compiler\n";
//         else if (ext == "py")
//             fout << "Please install Python\n";

//         fout.close();
//         return;
//     }

//     if (ext == "c" || ext == "cpp" || ext == "c++")
//         runCppOrCFile(checker, effectiveTimeLimit, numberOfTestCases, pretestCasesPath);
//     else if (ext == "py")
//         runPythonFile();
// }

// void Judge::runCppOrCFile(int checker, int timeLimit, int numOfTestCases, std::string testCasesPath)
// {
//     std::string filename = getFileName(currentFile);
//     std::string exeFile, compileCmd;
//     std::string directoryPath = getDirectoryPath(currentFile);

// #ifdef _WIN32
//     exeFile = "\"" + directoryPath + filename + "-judge.exe" + "\"";
//     compileCmd = "g++ \"" + currentFile + "\" -o " + exeFile + " -Wall";
// #else
//     exeFile = "\"" + directoryPath + "./" + filename + "\"";
//     compileCmd = "g++ -O2 -fsanitize=address -g \"" + currentFile + "\" -o " + exeFile + " -Wall ";
// #endif

//     std::string compileOutput = executeCommand(compileCmd);

//     if (!compileOutput.empty())
//     {
//         if (compileOutput.find("error") != std::string::npos)
//         {
//             std::ofstream fout("verdict.txt", std::ios::app);
//             fout << "Compilation error." << std::endl;
//             fout.close();
//             return;
//         }
//     }

//     if (!std::filesystem::exists(exeFile.substr(1, exeFile.size() - 2)))
//     {
//         std::ofstream fout("verdict.txt", std::ios::app);
//         fout << "Compilation Failed!" << std::endl;
//         fout.close();
//         return;
//     }

//     for (int i = 1; i <= numOfTestCases; i++)
//     {
//         int caseNo = i;
//         std::string inputFile = testCasesPath + std::to_string(caseNo) + ".in";
//         std::string outputFile = testCasesPath + std::to_string(caseNo) + ".output";
//         std::string expectedFile = testCasesPath + std::to_string(caseNo) + ".out";

//         int status = runWithTimeout(exeFile, inputFile, outputFile, timeLimit);
//         if (status == 2)
//         {
//             std::ofstream fout("verdict.txt", std::ios::app);
//             fout << "Pretest Case " << caseNo << ": " << "Time Limit Exceeded\t";
//             fout.close();
//             continue;
//         }
//         else if (status == 1)
//         {
//             std::ofstream fout("verdict.txt", std::ios::app);
//             fout << "Pretest Case " << caseNo << ": " << "Runtime Error\t";
//             fout.close();
//             continue;
//         }

//         if (!checker)
//         {
//             std::ifstream out(outputFile), exp(expectedFile);
//             if (!out || !exp)
//             {
//                 std::ofstream fout("verdict.txt", std::ios::app);
//                 fout << "Couldn't open output file.\n" << std::endl;
//                 fout.close();
//                 return;
//             }

//             std::string s1((std::istreambuf_iterator<char>(out)), {});
//             std::string s2((std::istreambuf_iterator<char>(exp)), {});

//             if (normalize(s1) == normalize(s2))
//             {
//                 std::ofstream fout("verdict.txt", std::ios::app);
//                 fout << "Pretest Case " << caseNo << ": " << "Accepted\t";
//                 fout.close();
//             }
//             else
//             {
//                 std::ofstream fout("verdict.txt", std::ios::app);
//                 fout << "Pretest Case " << caseNo << ": " << "Wrong Answer\t";
//                 fout.close();
//             }
//         }
//     }
// }

void Judge::runPythonFile()
{
  // Implementation left empty
}

std::string Judge::normalize(const std::string &s)
{
    std::stringstream ss(s);
    std::string line, result = "";
    while (std::getline(ss, line))
    {
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
            continue;
        size_t end = line.find_last_not_of(" \t\r\n");
        result += line.substr(start, end - start + 1) + "\n";
    }
    return result;
}
