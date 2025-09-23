#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <filesystem>

#ifdef _WIN32
     #include <windows.h>
     #include <io.h>
     #define popen _popen
     #define pclose _pclose
#else
     #include <unistd.h>
     #include <sys/wait.h>
     #include <signal.h>
     #include <pty.h>
     #include <fcntl.h>

#endif

class CodeRunner
{

private:
     std::string currentFile;

     std::string getFileExtension(const std::string &filename)
     {
          size_t ind = filename.find_last_of('.');
          if (ind != std::string::npos)
          {
               return filename.substr(ind + 1);
          }
          return "";
     }

     std::string getFileName(const std::string &filepath)
     {
          size_t lastSlash = filepath.find_last_of("/\\");

          std::string filename = (lastSlash == std::string::npos) ? filepath : filepath.substr(lastSlash + 1);

          size_t lastDot = filename.find_last_of('.');

          return (lastDot == std::string::npos) ? filename : filename.substr(0, lastDot);
     }

     std::string getDirectoryPath(const std::string &filepath)
     {
          size_t lastSlash = filepath.find_last_of("/\\");

          return (lastSlash == std::string::npos) ? "" : filepath.substr(0, lastSlash + 1);
     }

     bool checkCompiler(const std::string &ext)
     {
          std::string checkCmd;
          if (ext == "c" || ext == "cpp" || ext == "c++")
          {
#ifdef _WIN32
               checkCmd = "where g++ >nul 2>&1";
#else
               checkCmd = "which g++ >/dev/null 2>&1";
#endif
          }
          else if (ext == "py")
          {
#ifdef _WIN32
               checkCmd = "where python>nul 2>&1";
#else
               checkCmd = "which python3 >/dev/null 2>&1";
#endif
          }
          return system(checkCmd.c_str()) == 0;
     }

     std::string executeCommand(std::string &command)
     {
          std::string result;
          command += std::string(" 2>&1");
          FILE *pipe = popen(command.c_str(), "r");

          if (!pipe)
          {
               return "Error: Failed to execute compile command";
          }

          char buffer[512];
          while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
          {
               result += buffer;
          }

          int exitCode = pclose(pipe);

          if (exitCode != 0 && result.find("Command failed") == std::string::npos)
          {
               result += "\nCommand failed with exit code: " + std::to_string(exitCode);
          }
          return result;
     }

     std::string executeExeFile(const std::string &exeCommand, int &runtimeError)
     {
          std::string result;
          runtimeError = false;

#ifdef _WIN32
          std::string fullCmd = exeCommand + " < input.txt > output.txt 2>>error.txt";

          FILE *pipe = _popen(fullCmd.c_str(), "r");
          if (!pipe)
               return "Error: Failed to execute run command";

          setvbuf(pipe, NULL, _IONBF, 0);

          char buffer[128];
          while (fread(buffer, 1, 1, pipe) == 1)
          {
               result += buffer;
          }

          int exitCode = _pclose(pipe);
          if (exitCode != 0)
               runtimeError = exitCode;

#else
          std::string fullCmd = exeCommand + " < input.txt >output.txt 2>>error.txt";

          FILE *pipe = popen(fullCmd.c_str(), "r");
          if (!pipe)
               return "Error: Failed to execute command";

          char buffer[128];
          while (fread(buffer, 1, 1, pipe) == 1)
          {
               result += buffer;
          }

          int status = pclose(pipe);
          if (status == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0)
               runtimeError = true;
#endif

          return result;
     }

public:
     void setCurrentFile(const std::string &file)
     {
          currentFile = file;
     }

     void run()
     {
          std::ofstream fout("error.txt", std::ios::trunc);
          fout.close();
        
          std::ofstream("output.txt", std::ios::trunc);
          fout.close();
          if (currentFile.empty())
          {

               std::ofstream fout("error.txt", std::ios::app);
               fout << "Error: No file specified.\n"
                    << std::endl;

               fout.close();
               return;
          }

          if (!std::filesystem::exists(currentFile))
          {
               std::ofstream fout("error.txt", std::ios::app);
               fout << "Error : File '" << currentFile << "' does not exist.\n";
               fout.close();

               return;
          }
          if (!std::filesystem::exists("input.txt"))
          {
               std::ofstream fout("error.txt", std::ios::app);
               fout << "Error : File '" << "input.txt" << "' does not exist.\n";
               fout.close();
               return;
          }

          std::string ext = getFileExtension(currentFile);

          if (ext != "c" && ext != "cpp" && ext != "c++" && ext != "py")
          {
               std::ofstream fout("error.txt", std::ios::app);
               fout << "Error: Unsupported file type " << ext << ".\n";
               fout.close();
               return;
          }

          if (!checkCompiler(ext))
          {
               std::ofstream fout("error.txt", std::ios::app);
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
               runCppOrCFile();
          }
          else if (ext == "py")
          {
               runPythonFile();
          }
     }

private:
     void runCppOrCFile()
     {
          std::string filename = getFileName(currentFile);

          std::string exeFile, compileCmd;

          std::string directoryPath = getDirectoryPath(currentFile);

#ifdef _WIN32
          exeFile = "\"" + directoryPath + filename + ".exe" + "\"";
          compileCmd = "g++ \"" + currentFile + "\" -o " + exeFile + " -Wall";

#else
          exeFile = "\"" + directoryPath +"./"+ filename + "\"";
          compileCmd = "g++ -O2 -fsanitize=address -g \"" + currentFile + "\" -o " + exeFile + " -Wall ";

#endif

          std::string compileOutput = executeCommand(compileCmd);

          if (!compileOutput.empty())
          {
               std::string compilationOutput = "Compilation Output:\n\n";
               compilationOutput += compileOutput;

               std::ofstream fout("error.txt", std::ios::app);
               fout << compilationOutput << std::endl;
               fout.close();

               if (compilationOutput.find("error") != std::string::npos)
               {
                    return;
               }
          }

          if (!std::filesystem::exists(exeFile.substr(1, exeFile.size() - 2)))
          {
               std::ofstream fout("error.txt", std::ios::app);
               fout << "Compilation Failed!" << std::endl;
               fout.close();
               return;
          }

          int exitCode;
          std ::string error = executeExeFile(exeFile, exitCode);
          if (exitCode)
          {
               #ifdef _WIN32
               if (exitCode == 1)
               {
                    std::ofstream fout("error.txt", std::ios::app);
                    fout << "Execution command error.\n";
                    fout << error;
                    fout << std::endl;
                    fout.close();
               }
               else
               {
                    std::ofstream fout("error.txt", std::ios::app);
                    fout << "Runtime error: \n\n";
                    fout << "Runtime error occured with exit code: " << exitCode;
                    fout << std::endl;
                    fout.close();
               }
               #endif
               return;
          }
          else
          {
               std::ofstream fout("error.txt", std::ios::app);
                 fout << "Compilation and execution are successful.\n"
               << std::endl;
               fout.close();
          }
     }
     void runPythonFile()
     {
#ifdef _WIN32
          std::string pythonCmd = "python";
#else
          std::string pythonCmd = "python3";

#endif

          std::string runCmd = pythonCmd + " -u \"" + currentFile + "\"";

          int exitCode;
          std ::string error = executeExeFile(runCmd, exitCode);
          if (exitCode)
          {
               return;
          }
           else
          {
               std::ofstream fout("error.txt", std::ios::app);
                 fout << "Compilation and execution are successful.\n"
               << std::endl;
               fout.close();
          }
     }
};

void Runner(std::string &filename)
{
     CodeRunner runner;
     runner.setCurrentFile(filename);
     runner.run();
}

// int main()
// {

//      std::string filename;

//      std::cout << "Enter filename: ";
//      std::getline(std::cin, filename);

//      Runner(filename);
// }
