#include "Judge.h"

Judge::Judge(){};
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

void Judge::setJudgeInfo(const std::vector<double> &judgeInfo)
{
    if (judgeInfo.size() < 14)
        return; // Not enough data; you can also throw an error or handle safely

    checkerFlag = static_cast<bool>(judgeInfo[0]);
    inputFlag = static_cast<bool>(judgeInfo[1]);
    timeLimit = judgeInfo[2];
    cppTimeLimit = judgeInfo[3];
    pythonTimeLimit = judgeInfo[4];
    javaTimeLimit = judgeInfo[5];
    memoryLimit = judgeInfo[6];
    cppMemoryLimit = judgeInfo[7];
    pythonMemoryLimit = judgeInfo[8];
    javaMemoryLimit = judgeInfo[9];
    sourceCodeLimit = judgeInfo[10];
    cppSourceCodeLimit = judgeInfo[11];
    pythonSourceCodeLimit = judgeInfo[12];
    javaSourceCodeLimit = judgeInfo[13];
}
void Judge::setCurrentTestCaseNo(const std::string & testCase)
{
    currentTestCaseNo=testCase;
}
void Judge::setNumberOfTotalTestCase(int totalTestCase)
{
    numberOfTotalTestCase=totalTestCase;
}
Verdict Judge::isReadyForJudge(int &effectiveTimeLimit,
                               int &effectiveMemoryLimit,
                               int &effectiveSourceCodeLimit)
{

    std::string filename = getFileName(currentFile);
    std::string directoryPath = getDirectoryPath(currentFile);

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
    if(ext=="c"||ext=="cpp"||ext=="c++"||ext=="py")
    {
        if(!checkCompiler(ext))
        {
            return Verdict("Compilation Failed",0,0);
        }
    }
    else
    {
        return Verdict("Unsupported File Type",0,0);
    }

    sourceCodeLimit*=1024;

    if (ext == "c"||ext=="cpp"||ext=="c++")
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
        if((int)(size)>effectiveSourceCodeLimit)
        {
            return Verdict("Source Code Limit Exceeded",0,0);
        }
    } catch (std::filesystem::filesystem_error &e) {
        return Verdict("Source Code Error",0,0);
    }
    return Verdict("Ready",0,0);
}

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

#ifdef _WIN32

DWORD WINAPI ThreadFunction(LPVOID lpParam)
{
    int n = 100000;
    const char* filename = "normalizeFactorCal.txt";
    std::ofstream outFile(filename);
    if (!outFile) {

        return 0;
    }
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum = (sum + i) % 10000007;
        outFile << sum << std::endl;
    }


    outFile.close();


    if (std::remove(filename) == 0) {
        return 0;
    } else {
        return 0;
    }
    return 0;
}
#endif

double Judge::getNormalizeFactor()
{
    double factor=1;
#ifdef _WIN32
    HANDLE hThread;
    DWORD threadId;

           // Create the thread
    hThread = CreateThread(
        NULL,              // default security
        0,                 // default stack size
        ThreadFunction,    // thread function
        NULL,              // parameter
        0,                 // default creation flags
        &threadId          // thread ID
        );

    if (hThread == NULL) {

        return 1;
    }

           // Wait for the thread to finish
    WaitForSingleObject(hThread, INFINITE);

           // Get thread CPU times
    FILETIME createTime, exitTime, kernelTime, userTime;
    if (!GetThreadTimes(hThread, &createTime, &exitTime, &kernelTime, &userTime)) {

        return 1;
    }

           // Convert FILETIME to milliseconds
    ULARGE_INTEGER k, u;
    k.LowPart = kernelTime.dwLowDateTime;
    k.HighPart = kernelTime.dwHighDateTime;
    u.LowPart = userTime.dwLowDateTime;
    u.HighPart = userTime.dwHighDateTime;

    long long cpuTimeMS = (k.QuadPart + u.QuadPart) / 10000;
     factor =cpuTimeMS/110.0;

    CloseHandle(hThread);

#else
    const int n = 200000;
    const char* filename = "normalizeFactorCal.txt";

    long long cpuTimeMS = 0;

           // Create and run the thread using a lambda
    QThread* thread = QThread::create([n, filename, &cpuTimeMS,&factor]() {
        // Measure CPU time at thread start
        struct timespec startTime, endTime;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startTime);

        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            int sum = 0;
            for (int i = 0; i < n; ++i) {
                sum = (sum + i) % 10000007;
                out << sum << Qt::endl;
            }
            file.close();
        }

        std::remove(filename);  // delete file

               // Measure CPU time at thread end
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &endTime);

        cpuTimeMS = (endTime.tv_sec - startTime.tv_sec) * 1000 +
            (endTime.tv_nsec - startTime.tv_nsec) / 1000000;

         factor = cpuTimeMS / 170.0;
    });

    thread->start();
    thread->wait();  // wait for thread to finish
    delete thread;



#endif
      return factor;
}


int Judge::runWithTimeout(const std::string &runCommand,
                          std::string &inputFile,
                          std::string &outputFile,
                          int timeLimitMS,
                          int memoryLimitKB,
                          long long &usedTimeMS,
                          long long &usedMemoryKB,
                          bool inFlag)
{
    usedTimeMS=30;

    double factor=getNormalizeFactor();
    timeLimitMS=round(timeLimitMS*factor);

#ifdef _WIN32



    STARTUPINFOA si = {sizeof(si)};
    si.dwFlags = STARTF_USESTDHANDLES;
    HANDLE hInput = INVALID_HANDLE_VALUE;
    HANDLE hOutput = INVALID_HANDLE_VALUE;
    int verdict = -1;
    DWORD waitResult = WAIT_TIMEOUT;
    bool processExited = false;
    bool jobKilledProcess = false;

           // Output redirection
    hOutput = CreateFileA(outputFile.c_str(), GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hOutput == INVALID_HANDLE_VALUE) return -1;

           // Input or NUL
    if (inFlag) {
        hInput = CreateFileA(inputFile.c_str(), GENERIC_READ, FILE_SHARE_READ,
                             NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    } else {
        hInput = CreateFileA("NUL", GENERIC_READ, FILE_SHARE_READ,
                             NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    if (hInput == INVALID_HANDLE_VALUE) { CloseHandle(hOutput); return -1; }

    SetHandleInformation(hInput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hOutput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    si.hStdInput = hInput;
    si.hStdOutput = si.hStdError = hOutput;

    PROCESS_INFORMATION pi{};
    std::vector<char> cmdBuf(runCommand.begin(), runCommand.end());
    cmdBuf.push_back('\0');

    if (!CreateProcessA(NULL, cmdBuf.data(), NULL, NULL, TRUE,
                        CREATE_NO_WINDOW | CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        CloseHandle(hInput); CloseHandle(hOutput);
        return -1;
    }

    currentProcessHandle = pi.hProcess;
    currentThreadHandle  = pi.hThread;

           // Create Job Object with I/O Completion Port for notifications
    HANDLE hJob = CreateJobObject(NULL, NULL);
    HANDLE hIOCP = NULL;

    if (hJob) {
        // Create I/O Completion Port for job notifications
        hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
        if (hIOCP) {
            JOBOBJECT_ASSOCIATE_COMPLETION_PORT port;
            port.CompletionKey = hJob;
            port.CompletionPort = hIOCP;
            SetInformationJobObject(hJob, JobObjectAssociateCompletionPortInformation,
                                    &port, sizeof(port));
        }

               // Set memory limit
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobInfo{};
        jobInfo.BasicLimitInformation.LimitFlags =
            JOB_OBJECT_LIMIT_PROCESS_MEMORY | JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        jobInfo.ProcessMemoryLimit = (SIZE_T)memoryLimitKB * 1024;
        SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jobInfo, sizeof(jobInfo));

        AssignProcessToJobObject(hJob, pi.hProcess);
    }

           // Resume the process
    ResumeThread(pi.hThread);



           // Track peak memory usage during execution
    SIZE_T peakMemoryBytes = 0;

           // Poll loop with precise timing
    while (true) {
        // Check for job notifications (memory limit exceeded)
        if (hIOCP) {
            DWORD numberOfBytes;
            ULONG_PTR completionKey;
            LPOVERLAPPED overlapped;

            while (GetQueuedCompletionStatus(hIOCP, &numberOfBytes, &completionKey,
                                             &overlapped, 0)) {
                if (numberOfBytes == JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT) {
                    jobKilledProcess = true;
                    verdict = 3; // MLE
                    break;
                }
            }
        }

               // Monitor memory usage WHILE process is running
        PROCESS_MEMORY_COUNTERS_EX pmcEx{};
        pmcEx.cb = sizeof(pmcEx);
        if (GetProcessMemoryInfo(pi.hProcess, (PROCESS_MEMORY_COUNTERS*)&pmcEx, sizeof(pmcEx))) {
            // Track peak memory (PrivateUsage = commit charge)
            if (pmcEx.PrivateUsage > peakMemoryBytes) {
                peakMemoryBytes = pmcEx.PrivateUsage;
            }

                   // Check if memory limit exceeded (in case job object didn't catch it)
            SIZE_T currentMemoryKB = pmcEx.PrivateUsage / 1024;
            if (currentMemoryKB > memoryLimitKB) {
                TerminateProcess(pi.hProcess, 1);
                WaitForSingleObject(pi.hProcess, 500);
                verdict = 3; // MLE
                processExited = true;
                break;
            }
        }

               // Check stop request first - highest priority
        if (stopRequested.load()) {
            TerminateProcess(pi.hProcess, 1);
            WaitForSingleObject(pi.hProcess, 500);
            verdict = 5;
            processExited = true;
            break;
        }

               // If job killed the process for memory, break
        if (jobKilledProcess) {
            WaitForSingleObject(pi.hProcess, 500);
            processExited = true;
            break;
        }

               // Calculate remaining time

        FILETIME createTime, exitTime, kernelTime, userTime;
        GetProcessTimes(currentProcessHandle, &createTime, &exitTime, &kernelTime, &userTime);

        ULARGE_INTEGER k, u;
        k.LowPart = kernelTime.dwLowDateTime;
        k.HighPart = kernelTime.dwHighDateTime;
        u.LowPart = userTime.dwLowDateTime;
        u.HighPart = userTime.dwHighDateTime;

               // Convert from 100-nanosecond units to milliseconds
        long long elapsed = (k.QuadPart + u.QuadPart) / 10000;
        usedTimeMS=fmax(elapsed/(1.0*factor),usedTimeMS);
        if (elapsed >= timeLimitMS) {
            // Time limit exceeded

            TerminateProcess(pi.hProcess, 1);
            WaitForSingleObject(pi.hProcess, 500);
            verdict = 2; // TLE
            processExited = true;
            break;
        }

               // Wait for remaining time or 10ms, whichever is smaller
        DWORD waitTime = (DWORD)std::min((long long)10, timeLimitMS - elapsed);
        waitResult = WaitForSingleObject(pi.hProcess, waitTime);

        if (waitResult != WAIT_TIMEOUT) {
            // Process finished naturally
            processExited = true;
            break;
        }
    }


           // Use the peak memory we tracked during execution
    usedMemoryKB = peakMemoryBytes / 1024;

           // Check stop request one more time before determining verdict
    if (stopRequested.load()) {
        verdict = 5;
    }
    // Determine verdict if not already set
    else if (verdict == -1) {
        // Check memory limit
        if (usedMemoryKB > memoryLimitKB) {
            verdict = 3; // MLE
        }
        else {
            DWORD exitCode = STILL_ACTIVE;
            if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
                // Check for memory-related error codes
                if (exitCode == STATUS_NO_MEMORY ||
                    exitCode == 0xC0000017 || // STATUS_NO_MEMORY
                    exitCode == 0xC000012D || // STATUS_COMMITMENT_LIMIT
                    exitCode == 0xC00000FD) { // STATUS_STACK_OVERFLOW
                    verdict = 3; // MLE
                } else {
                    verdict = (exitCode == 0) ? 0 : 1;
                }
            } else {
                verdict = 1;
            }
        }
    }

           // Cleanup
    if (hIOCP) CloseHandle(hIOCP);
    if (hJob) CloseHandle(hJob);
    if (pi.hProcess) CloseHandle(pi.hProcess);
    if (pi.hThread)  CloseHandle(pi.hThread);
    if (hInput != INVALID_HANDLE_VALUE)  CloseHandle(hInput);
    if (hOutput != INVALID_HANDLE_VALUE) CloseHandle(hOutput);

    currentProcessHandle = NULL;
    currentThreadHandle  = NULL;
    stopRequested.store(false);

           // Ensure file flushed/unlocked by process
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return verdict;
#else


    QProcess process;
    int verdict = -1;
    bool processExited = false;
    bool memoryLimitExceeded = false;

           // Set up input redirection
    if (inFlag) {
        process.setStandardInputFile(QString::fromStdString(inputFile));
    } else {
        process.setStandardInputFile("/dev/null");
    }

           // Set up output redirection
    process.setStandardOutputFile(QString::fromStdString(outputFile));
    process.setStandardErrorFile(QString::fromStdString(outputFile), QIODevice::Append);

           // Parse command for QProcess
    QStringList args;
    QString program;

           // Better parsing that handles quoted strings
    QString cmdStr = QString::fromStdString(runCommand);
    QStringList parts;

    QString current;
    bool inQuotes = false;
    QChar quoteChar;

    for (int i = 0; i < cmdStr.length(); ++i) {
        QChar c = cmdStr[i];

        if (!inQuotes && (c == '"' || c == '\'')) {
            inQuotes = true;
            quoteChar = c;
        } else if (inQuotes && c == quoteChar) {
            inQuotes = false;
        } else if (!inQuotes && c == ' ') {
            if (!current.isEmpty()) {
                parts << current;
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.isEmpty()) {
        parts << current;
    }

    if (parts.isEmpty()) {
        return -1;
    }

    program = parts[0];
    for (int i = 1; i < parts.size(); ++i) {
        args << parts[i];
    }

           // Start the process
    process.start(program, args);

    if (!process.waitForStarted(1000)) {
        return -1; // Failed to start
    }

    pid_t pid = process.processId();

           // Store only the PID atomically - no pointers
    currentProcessPid.store(pid);



           // Monitoring loop with precise timing
    while (true) {
        // Check stop request first - highest priority
        if (stopRequested.load()) {
            // Kill only the child process, not our application
            killProcessSafely(pid);
            process.waitForFinished(500);
            verdict = 5;
            processExited = true;
            break;
        }

               // Check if process is still running
        if (process.state() == QProcess::NotRunning) {
            processExited = true;
            break;
        }

               // Calculate elapsed time

        long long elapsed =getChildCpuTimeMS(pid);
        usedTimeMS=fmax(usedTimeMS,elapsed/(1.0*factor));

               // Check memory usage
        long long currentMemoryKB = getProcessMemoryUsage(pid);
        usedMemoryKB=fmax(usedMemoryKB,currentMemoryKB);
        if (currentMemoryKB > memoryLimitKB) {
            killProcessSafely(pid);
            process.waitForFinished(500);
            verdict = 3; // MLE
            memoryLimitExceeded = true;
            processExited = true;
            break;
        }

               // Check time limit
        if (elapsed >= timeLimitMS) {
            killProcessSafely(pid);
            process.waitForFinished(500);
            verdict = 2; // TLE
            processExited = true;
            break;
        }

               // Wait for a short period or remaining time
        int waitTime = std::min(10LL, timeLimitMS - elapsed);
        process.waitForFinished(waitTime);
    }


    long long elapsed =getChildCpuTimeMS(pid);
    usedTimeMS=fmax(usedTimeMS,elapsed/(1.0*factor));

           // Get final memory usage
    //usedMemoryKB = getProcessMemoryUsage(pid);

           // Check stop request one more time before determining verdict
    if (stopRequested.load()) {
        verdict = 5;
    }
    // Determine verdict if not already set
    else if (verdict == -1) {
        // Check memory limit first
        if (usedMemoryKB > memoryLimitKB) {
            verdict = 3; // MLE
        } else {
            int exitCode = process.exitCode();
            QProcess::ExitStatus exitStatus = process.exitStatus();

                   // Check for abnormal termination
            if (exitStatus == QProcess::CrashExit) {
                // Check if it was a memory-related crash
                if (exitCode == 9 || exitCode == 11) { // SIGKILL or SIGSEGV
                    // Could be memory related, check memory usage
                    if (usedMemoryKB > memoryLimitKB * 0.95) { // 95% threshold
                        verdict = 3; // MLE
                    } else {
                        verdict = 1; // Runtime Error
                    }
                } else {
                    verdict = 1; // Runtime Error
                }
            } else {
                // Normal exit - check exit code
                verdict = (exitCode == 0) ? 0 : 1;
            }
        }
    }

           // Cleanup - Reset PID
    currentProcessPid.store(0);
    stopRequested.store(false);

           // Ensure file flushed
    QThread::msleep(100);

    return verdict;

#endif
}
void Judge::stopJudge() {
    stopRequested.store(true);
#ifdef _WIN32
    if (currentProcessHandle) {
        TerminateProcess(currentProcessHandle, 1);
        WaitForSingleObject(currentProcessHandle, 500);
        if (currentThreadHandle) {
            CloseHandle(currentThreadHandle);
            currentThreadHandle = NULL;
        }
        CloseHandle(currentProcessHandle);
        currentProcessHandle = NULL;
    }
#else
    pid_t pid = currentProcessPid.load();

           // Only kill if we have a valid PID
    if (pid > 0) {
        killProcessSafely(pid);
    }
#endif
}

#ifndef _WIN32

long long Judge::getChildCpuTimeMS(pid_t pid)
{
    std::ifstream statFile("/proc/" + std::to_string(pid) + "/stat");
    if (!statFile.is_open())
        return 0;

    std::string content;
    std::getline(statFile, content);
    statFile.close();

    std::istringstream iss(content);
    std::string token;
    int i = 0;
    long utime = 0, stime = 0;

    while (iss >> token) {
        ++i;
        if (i == 14) utime = std::stol(token);   // user CPU time
        if (i == 15) { stime = std::stol(token); break; } // kernel CPU time
    }

    long ticksPerSec = sysconf(_SC_CLK_TCK); // usually 100
    long long cpuMS = (utime + stime) * 1000 / ticksPerSec;
    return cpuMS;
}


bool Judge::isProcessRunning(pid_t pid)
{
    if (pid <= 0) return false;

           // Use kill with signal 0 to check if process exists
           // Returns 0 if process exists, -1 if not
    return (kill(pid, 0) == 0);
}

void Judge::killProcessSafely(pid_t pid)
{
    if (pid <= 0) {
        return; // Invalid PID, do nothing
    }

           // Double-check this is the exact PID we stored
    if (pid != currentProcessPid.load()) {
        return; // Safety check - don't kill if PID doesn't match
    }

           // Verify process exists before killing
    if (!isProcessRunning(pid)) {
        return; // Process already dead
    }

           // Send SIGTERM for graceful shutdown
    kill(pid, SIGTERM);

           // If still running, send SIGKILL
    if (isProcessRunning(pid)) {

        kill(pid, SIGKILL);
    }
}

long long Judge::getProcessMemoryUsage(pid_t pid)
{
    if (pid <= 0) return 0;

           // Read from /proc/[pid]/status for VmRSS (Resident Set Size)
    std::string statusFile = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream file(statusFile);

    if (!file.is_open()) {
        return 0;
    }

    std::string line;
    long long memoryKB = 0;

    while (std::getline(file, line)) {
        // Trim leading spaces
        size_t first = line.find_first_not_of(" \t");
        if (first == std::string::npos) continue;

        if (line.substr(first, 6) == "VmRSS:") {
            std::istringstream iss(line);
            std::string label;
            long long value;
            std::string unit;

            if (iss >> label >> value >> unit) {
                memoryKB = value; // Already in kB
            }
            break;
        }
    }

    file.close();
    return memoryKB;
}

// Alternative helper function using /proc/[pid]/statm (faster but less detailed)
long long Judge::getProcessMemoryUsageFast(pid_t pid)
{
    if (pid <= 0) return 0;

    std::string statmFile = "/proc/" + std::to_string(pid) + "/statm";
    std::ifstream file(statmFile);

    if (!file.is_open()) {
        return 0;
    }

    long long vmSize = 0, rss = 0;
    file >> vmSize >> rss;
    file.close();

    if (rss <= 0) return 0;

           // rss is in pages, convert to KB (assuming system page size)
    long long pageSizeKB = sysconf(_SC_PAGESIZE) / 1024;
    return rss * pageSizeKB;
}

#endif

Verdict Judge::runOnSingleTestCase()
{

    std::string outputFile = pretestCasesPath + currentTestCaseNo + ".output";

    std::ofstream output(outputFile);
    output.close();
    int effectiveTimeLimit;
    int effectiveMemoryLimit;
    int effectiveSourceCodeLimit;
    Verdict _verdict =isReadyForJudge(effectiveTimeLimit,effectiveMemoryLimit,effectiveSourceCodeLimit);
    if(_verdict.verdict!="Ready")return _verdict;

    std::string ext=getFileExtension(currentFile);
    std::string directoryPath=getDirectoryPath(currentFile);
    std::string filename=getFileName(currentFile);

    std::string exeFile, compileCmd;
    std::string verdict;
    long long cpuTime=0;
    long long memorySize=0;
    std::string inputFile = pretestCasesPath + currentTestCaseNo + ".in";
    std::string expectedFile = pretestCasesPath + currentTestCaseNo + ".out";


    if(ext=="c"||ext=="cpp"||ext=="c++")
    {


#ifdef _WIN32
        exeFile = "\"" + directoryPath + filename + "-judge.exe" + "\"";
        compileCmd = "g++ -O2  -march=x86-64 -mtune=generic -pipe -s -static -DONLINE_JUDGE -fno-asm \"" + currentFile + "\" -o " + exeFile + " -Wall";
#else

        exeFile = "\"" + directoryPath + "./" + filename +"-judge.out"+"\"";
        compileCmd = "g++   -O2 -std=c++17 -march=x86-64 -mtune=generic -pipe -s -static \"" + currentFile + "\" -o " + exeFile + " -Wall ";
#endif

        std::string compileOutput = executeCommand(compileCmd);

        if (!compileOutput.empty())
        {
            if (compileOutput.find("error") != std::string::npos)
            {
                exeFile.pop_back();
                exeFile.erase(exeFile.begin());
                std::remove(exeFile.c_str());
                return Verdict("Compilation Error",0,0);
            }
        }
        if (!std::filesystem::exists(exeFile.substr(1, exeFile.size() - 2)))
        {
            exeFile.pop_back();
            exeFile.erase(exeFile.begin());
            std::remove(exeFile.c_str());
            return Verdict("Compilation Failed",0,0);
        }
    }
    else if(ext=="py")
    {
#ifdef _WIN32
        std::string pythonCmd = "python";
#else
        std::string pythonCmd = "python3";
#endif
        exeFile = pythonCmd + " \"" + currentFile + "\"";

    }

    int status=runWithTimeout(exeFile,inputFile,outputFile,effectiveTimeLimit,effectiveMemoryLimit,
                                cpuTime,memorySize,inputFlag);

    if(status==0)
    {
      //Succeess Execution
    }
    else if(status==1)
    {
        exeFile.pop_back();
        exeFile.erase(exeFile.begin());
        std::remove(exeFile.c_str());
        verdict="Runtime Error";
        return Verdict(verdict,cpuTime,memorySize);
    }
    else if(status==2)
    {
        exeFile.pop_back();
        exeFile.erase(exeFile.begin());
        std::remove(exeFile.c_str());
        verdict="Time Limit Exceeded";
        return Verdict(verdict,cpuTime,memorySize);
    }
    else if(status==3)
    {
        exeFile.pop_back();
        exeFile.erase(exeFile.begin());
        std::remove(exeFile.c_str());
        verdict="Memory Limit Exceeded";
        return Verdict(verdict,cpuTime,memorySize+effectiveMemoryLimit);
    }
    else if(status==5)
    {
        exeFile.pop_back();
        exeFile.erase(exeFile.begin());
        std::remove(exeFile.c_str());
        verdict="Judge Terminated";
        return Verdict(verdict,cpuTime,memorySize);
    }
    else
    {
        exeFile.pop_back();
        exeFile.erase(exeFile.begin());
        std::remove(exeFile.c_str());
        verdict="I/O Error";
        return Verdict(verdict,cpuTime,memorySize);
    }
    if(!checkerFlag)
    {

        std::ifstream out(outputFile), exp(expectedFile);
        if (!out || !exp)
        {
            exeFile.pop_back();
            exeFile.erase(exeFile.begin());
            std::remove(exeFile.c_str());

            return Verdict("Error",cpuTime,memorySize);
        }
        std::string s1((std::istreambuf_iterator<char>(out)), {});
        std::string s2((std::istreambuf_iterator<char>(exp)), {});


        if (normalize(s1) == normalize(s2))
        {
            exeFile.pop_back();
            exeFile.erase(exeFile.begin());
            std::remove(exeFile.c_str());
            return Verdict("Accepted",cpuTime,memorySize);
        }
        else
        {
            exeFile.pop_back();
            exeFile.erase(exeFile.begin());
            std::remove(exeFile.c_str());
            return Verdict("Wrong Answer",cpuTime,memorySize);
        }


    }
    exeFile.pop_back();
    exeFile.erase(exeFile.begin());
    std::remove(exeFile.c_str());
    return Verdict("",0,0);

}
std::vector<Verdict> Judge::generateVerdicts(std::string verdict,int n)
{
    std::vector<Verdict> verdicts;
    int i;
    for(i=1;i<=n;i++)
    {
        verdicts.push_back(Verdict(verdict,0,0));
    }
    return verdicts;
}



std::vector<Verdict> Judge::runOnTestCases()
{
    std::vector<Verdict> verdicts;

    int effectiveTimeLimit;
    int effectiveMemoryLimit;
    int effectiveSourceCodeLimit;
    Verdict verdict =isReadyForJudge(effectiveTimeLimit,effectiveMemoryLimit,effectiveSourceCodeLimit);
    if(verdict.verdict!="Ready")return generateVerdicts(verdict.verdict,numberOfTotalTestCase);

    std::string ext=getFileExtension(currentFile);
    std::string directoryPath=getDirectoryPath(currentFile);
    std::string filename=getFileName(currentFile);
    std::string exeFile, compileCmd;
    if(ext=="c"||ext=="cpp"||ext=="c++")
    {



#ifdef _WIN32
        exeFile = "\"" + directoryPath + filename + "-judge.exe" + "\"";
        compileCmd = "g++ -O2  -march=x86-64 -mtune=generic -pipe -s -static -DONLINE_JUDGE -fno-asm \"" + currentFile + "\" -o " + exeFile + " -Wall";

#else
        exeFile = "\"" + directoryPath + "./" + filename +"-judge"+"\"";
        compileCmd = "g++   -O2 -std=c++17 -march=x86-64 -mtune=generic -pipe -s -static \"" + currentFile + "\" -o " + exeFile + " -Wall ";
#endif

        std::string compileOutput = executeCommand(compileCmd);

        if (!compileOutput.empty())
        {
            if (compileOutput.find("error") != std::string::npos)
            {
                exeFile.pop_back();
                exeFile.erase(exeFile.begin());
                std::remove(exeFile.c_str());

                return generateVerdicts("Compilation Error",numberOfTotalTestCase);
            }
        }
        if (!std::filesystem::exists(exeFile.substr(1, exeFile.size() - 2)))
        {
            exeFile.pop_back();
            exeFile.erase(exeFile.begin());
            std::remove(exeFile.c_str());
            return generateVerdicts("Compilation Failed",0);
        }
    }
    else if(ext=="py")
    {
#ifdef _WIN32
        std::string pythonCmd = "python";
#else
        std::string pythonCmd = "python3";
#endif
        exeFile = pythonCmd + " \"" + currentFile + "\"";

    }
    int terminated=0;

    for(int i=1;i<=numberOfTotalTestCase;i++)
    {
        if(terminated)
        {
            std::string verdict="Judge Terminated";
            verdicts.push_back(Verdict(verdict,0,0));
            continue;
        }
        std::string testCaseNo=std::to_string(i);
        std::string verdict="";
        long long cpuTime=0;
        long long memorySize=0;
        std::string inputFile = pretestCasesPath + testCaseNo + ".in";
        std::string expectedFile = pretestCasesPath + testCaseNo + ".out";
        std::string outputFile = pretestCasesPath + testCaseNo + ".output";


        int status=runWithTimeout(exeFile,inputFile,outputFile,effectiveTimeLimit,effectiveMemoryLimit,
                                    cpuTime,memorySize,inputFlag);
        if(status==0)
        {
          //Succeess Execution
        }
        else if(status==1)
        {
            verdict="Runtime Error";
            verdicts.push_back(Verdict(verdict,cpuTime,memorySize));
            continue;
        }
        else if(status==2)
        {
            verdict="Time Limit Exceeded";
            verdicts.push_back(Verdict(verdict,cpuTime,memorySize));
            continue;
        }
        else if(status==3)
        {
            verdict="Memory Limit Exceeded";
            verdicts.push_back(Verdict(verdict,cpuTime,memorySize+effectiveMemoryLimit));
            continue;
        }else if(status==5)
        {
            verdict="Judge Terminated";
            verdicts.push_back(Verdict(verdict,cpuTime,memorySize));
            terminated=1;
            continue;

        }
        else
        {
            verdict="I/O Error";
            verdicts.push_back(Verdict(verdict,cpuTime,memorySize));
            continue;
        }
        if(!checkerFlag)
        {

            std::ifstream out(outputFile), exp(expectedFile);
            if (!out || !exp)
            {

                verdict="I/O Error";
                verdicts.push_back(Verdict(verdict,cpuTime,memorySize));
                continue;
            }
            std::string s1((std::istreambuf_iterator<char>(out)), {});
            std::string s2((std::istreambuf_iterator<char>(exp)), {});
            if (normalize(s1) == normalize(s2))
            {
                verdict="Accepted";
                verdicts.push_back(Verdict(verdict,cpuTime,memorySize));
                continue;
            }
            else
            {
                verdict="Wrong Answer";
                verdicts.push_back(Verdict(verdict,cpuTime,memorySize));
                continue;
            }


        }


    }
    for(int i=1;i<=numberOfTotalTestCase;i++)
    {
        std::string testCaseNo=std::to_string(i);
        std::string outputFile = pretestCasesPath + testCaseNo + ".output";
        std::remove(outputFile.c_str());
    }
    exeFile.pop_back();
    exeFile.erase(exeFile.begin());
    std::remove(exeFile.c_str());

    return verdicts;
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
//     compileCmd = "g++   -O2 -std=c++17 -march=x86-64 -mtune=generic -pipe -s -static \"" + currentFile + "\" -o " + exeFile + " -Wall ";
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
