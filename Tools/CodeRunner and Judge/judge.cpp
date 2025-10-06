#include "judge.h"

int Judge::runWithTimeout(const string &cmd, const string &inputFile,
                          const string &outputFile, int timeLimitMS)
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
     vector<char> cmdBuf(cmd.begin(), cmd.end());
     cmdBuf.push_back('\0');
     if (!CreateProcessA(NULL, cmdBuf.data(), NULL, NULL, TRUE, 0,
                         NULL, NULL, &si, &pi))
     {
          CloseHandle(hInput);
          CloseHandle(hOutput);
          return -1;
     }

     DWORD res = WaitForSingleObject(pi.hProcess, timeLimitMS);
     DWORD exitCode = 1;
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

     CloseHandle(pi.hProcess);
     CloseHandle(pi.hThread);
     CloseHandle(hInput);
     CloseHandle(hOutput);
     return exitCode;

#else
     pid_t pid = fork();
     if (pid == 0)
     {
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
          int status = 0, elapsed = 0;
          while (elapsed < timeLimitMS)
          {
               pid_t r = waitpid(pid, &status, WNOHANG);
               if (r == pid)
               {
                    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                         return 0;
                    return 1;
               }
               usleep(1000 * 10);
               elapsed += 10;
          }
          kill(pid, SIGKILL);
          return 2;
     }
#endif
}

void Judge::setCurrentFile(const string &cFile)
{
     currentFile = cFile;
}
void Judge::setCurrentProblem(const string &cProblem)
{
     currentProblem = cProblem;
}
void Judge::setPretestCasesPath(const string &path)
{
     pretestCasesPath = path;
}
void Judge::setJudgeInfoPath(const string &path)
{
     judgeInfoPath = path;
}
void Judge::runOnTestCases()
{

     std::ofstream fout("verdict.txt", std::ios::trunc);

     fout.close();
     if (currentFile.empty())
     {

          std::ofstream fout("verdict.txt", std::ios::app);
          fout << "Error: No file specified.\n"
               << std::endl;

          fout.close();
          return;
     }

     if (!std::filesystem::exists(currentFile))
     {
          std::ofstream fout("verdict.txt", std::ios::app);
          fout << "Error : File '" << currentFile << "' does not exist.\n";
          fout.close();

          return;
     }
     if (currentProblem.empty())
     {

          std::ofstream fout("verdict.txt", std::ios::app);
          fout << "Error: No problem specified.\n"
               << std::endl;

          fout.close();
          return;
     }
     std::ofstream foutt("verdict.txt", std::ios::app);
     foutt << "Problem: " << currentProblem << endl;
     foutt.close();
    
     double timeLimit, cTimeLimit, cppTimeLimit, pyTimeLimit, javaTimeLimit;
     int checker;
     int numberOfTestCases;
     judgeInfoPath+="\\";
     pretestCasesPath+="\\"+currentProblem+"\\";
     string judgeInfoFile = judgeInfoPath + currentProblem + ".txt";
     cout<<judgeInfoFile<<endl;

     ifstream fin(judgeInfoFile);
     if (!fin)
     {
          std::ofstream fout("verdict.txt", std::ios::app);
          fout << "Judge Information not found.\n";
          fout.close();
          return;
     }
     fin >> checker >> timeLimit >> cTimeLimit >> cppTimeLimit >> pyTimeLimit >> javaTimeLimit >> numberOfTestCases;

     fin.close();

     string ext = CodeRunner::getFileExtension(currentFile);
     int effectiveTimeLimit;
     if (ext == "c")
     {
          effectiveTimeLimit = timeLimit * cTimeLimit * 1000;
     }
     else if (ext == "cpp")
     {
          effectiveTimeLimit = timeLimit * cppTimeLimit * 1000;
     }
     else if (ext == "py")
     {
          effectiveTimeLimit = timeLimit * pyTimeLimit * 1000;
     }
     else
     {
          effectiveTimeLimit = timeLimit * javaTimeLimit * 1000;
     }

     if (!CodeRunner::checkCompiler(ext))
     {
          std::ofstream fout("verdict.txt", std::ios::app);
          fout << "Error: Required compiler/interpreter not found for ." << ext << " files\n";

          if (ext == "c" || ext == "cpp" || ext == "c++")
          {
               fout << "Please install GNU g++ compiler\n";
          }
          else if (ext == "py")
          {
               fout << "Please install Python\n";
          }
          fout.close();
          return;
     }

     if (ext == "c" || ext == "cpp" || ext == "c++")
     {
          runCppOrCFile(checker, effectiveTimeLimit, numberOfTestCases, pretestCasesPath);
     }
     else if (ext == "py")
     {
          runPythonFile();
     }
}

void Judge::runCppOrCFile(int checker, int timeLimit, int numOfTestCases, string testCasesPath)
{
     std::string filename = CodeRunner::getFileName(currentFile);

     std::string exeFile, compileCmd;

     std::string directoryPath = CodeRunner::getDirectoryPath(currentFile);

#ifdef _WIN32
     // exeFile = "\"" + directoryPath + filename + "-pretest.exe" + "\"";
     // compileCmd = "g++ \"" + currentFile + "\" -o " + exeFile + " -Wall";
      int i;
     for (i = 1; i <= 100; i++)
     {
          int temp=i;
          std::string s=std::to_string(temp);
          exeFile = "\"" + directoryPath + filename +"-"+s +"-pretest.exe" + "\"";
          compileCmd = "g++ \"" + currentFile + "\" -o " + exeFile + " -Wall";
          std::string compileOutput = CodeRunner::executeCommand(compileCmd);
     }
#else
     exeFile = "\"" + directoryPath + "./" + filename + "\"";
     compileCmd = "g++ -O2 -fsanitize=address -g \"" + currentFile + "\" -o " + exeFile + " -Wall ";

#endif

     std::string compileOutput = CodeRunner::executeCommand(compileCmd);

     if (!compileOutput.empty())
     {

          if (compileOutput.find("error") != std::string::npos)
          {
               std::ofstream fout("verdict.txt", std::ios::app);
               fout << "Compilation error." << std::endl;
               fout.close();

               return;
          }
     }

     if (!std::filesystem::exists(exeFile.substr(1, exeFile.size() - 2)))
     {
          std::ofstream fout("verdict.txt", std::ios::app);
          fout << "Compilation Failed!" << std::endl;
          fout.close();
          return;
     }

     for (int i = 1; i <= numOfTestCases; i++)
     {
          int caseNo = i;
          string inputFile = testCasesPath + to_string(caseNo) + ".in";
          string outputFile = testCasesPath + to_string(caseNo) + ".output";
          string output_file = "\"" + testCasesPath + to_string(caseNo) + ".output\"";
          string expectedFile = testCasesPath + to_string(caseNo) + ".out";

          int status = runWithTimeout(exeFile, inputFile, outputFile, timeLimit);
          if (status == 2)
          {
               std::ofstream fout("verdict.txt", std::ios::app);
               fout << "Pretest Case " << caseNo << ": " << "Time Limit Exceeded\t";
               fout.close();
               continue;
          }
          else if (status == 1)
          {
               std::ofstream fout("verdict.txt", std::ios::app);
               fout << "Pretest Case " << caseNo << ": " << "Runtime Error\t";
               fout.close();
               continue;
          }
          if (!checker)
          {
               ifstream out(outputFile), exp(expectedFile);
               if (!out || !exp)
               {
                    std::ofstream fout("verdict.txt", std::ios::app);
                    fout << "Couldn't open output file.\n"
                         << std::endl;
                    fout.close();
                    return;
               }
               string s1((istreambuf_iterator<char>(out)), {});
               string s2((istreambuf_iterator<char>(exp)), {});

               if (normalize(s1) == normalize(s2))
               {
                    std::ofstream fout("verdict.txt", std::ios::app);
                    fout << "Pretest Case " << caseNo << ": " << "Accepted\t";
                    fout.close();
               }
               else
               {
                    std::ofstream fout("verdict.txt", std::ios::app);
                    fout << "Pretest Case " << caseNo << ": " << "Wrong Answer\t";
                    fout.close();
               }
          }
     }
}

void Judge::runPythonFile()
{
}

string Judge::normalize(const string &s)
{
     stringstream ss(s);
     string line, result = "";
     while (getline(ss, line))
     {
          size_t start = line.find_first_not_of(" \t\r\n");
          if (start == string::npos)
               continue;
          size_t end = line.find_last_not_of(" \t\r\n");
          result += line.substr(start, end - start + 1) + "\n";
     }
     return result;
}