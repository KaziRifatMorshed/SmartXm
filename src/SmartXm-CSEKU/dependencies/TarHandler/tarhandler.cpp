
#include "tarhandler.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

bool TarHandler::createTar(const string &tarName, const vector<string> &files)
{
    if (files.empty())
    {
        cerr << "No files provided to archive." << endl;
        return false;
    }
    string cmd = "tar -cf '" + tarName + "'";
    for (const auto &file : files)
    {
        cmd += " '" + file + "'";
    }
    int ret = system(cmd.c_str());
    if (ret != 0)
    {
        cerr << "Failed to create tar archive." << endl;
        return false;
    }
    return true;
}

bool TarHandler::extractTar(string path, string tarName)
{
    string cmd = "cd " + path + "&& tar -xf '" + tarName + "'";
    int ret = system(cmd.c_str());
    if (ret != 0)
    {
        cerr << "Failed to extract tar archive." << endl;
        return false;
    }
    cout << "File extracted to: " << tarName << endl;
    return true;
}

// int main(int argc, char *argv[])
// {
//     if (argc < 3)
//     {
//         cout << "Usage:\n  To archive:   " << argv[0] << " c <archive.tar> <file1> [file2 ...]" << endl;
//         cout << "  To extract:   " << argv[0] << " x <archive.tar>" << endl;
//         return 1;
//     }
//     string mode = argv[1];
//     if (mode == "c")
//     {
//         string tarName = argv[2];
//         vector<string> files;
//         for (int i = 3; i < argc; ++i)
//         {
//             files.push_back(argv[i]);
//         }
//         if (!TarHandler::createTar(tarName, files))
//         {
//             return 1;
//         }
//         cout << "Archive created: " << tarName << endl;
//     }
//     else if (mode == "x")
//     {
//         string tarName = argv[2];
//         if (!TarHandler::extractTar(tarName))
//         {
//             return 1;
//         }
//         cout << "Archive extracted: " << tarName << endl;
//     }
//     else
//     {
//         cerr << "Unknown mode: " << mode << endl;
//         return 1;
//     }
//     return 0;
// }
