#ifndef TARHANDLER_H
#define TARHANDLER_H

#include <string>
#include <vector>
using namespace std;

#define WM_STOP (WM_USER + 1)

class TarHandler
{
public:
    // Create a tar archive from a list of files
    static bool createTar(const string &tarName, const vector<string> &files);

    // Extract a tar archive to the given directory
    static bool extractTar(string &path, string &tarName);
};

#endif // TARHANDLER_H
