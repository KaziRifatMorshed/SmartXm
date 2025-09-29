
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

     std::string executeExeFile(const std::string &exeCommand, int &runtimeError);
     void runCppOrCFile();
     void runPythonFile();

public:

     void setCurrentFile(const std::string &file);
     static std::string getFileExtension(const std::string &filename);
     static std::string getFileName(const std::string &filepath);
     static std::string getDirectoryPath(const std::string &filepath);
     static bool checkCompiler(const std::string &ext);
     static std::string executeCommand(std::string &command);
     void run();

};


