#include <bits/stdc++.h>
#include "judge.h"

using namespace std;

void run()
{
     string cf, cp;

     getline(cin, cp);
     getline(cin, cf);
     string pathtestcase;
     string judgeinfopath;
     getline(cin,pathtestcase);
     getline(cin,judgeinfopath);

     Judge judge;
     judge.setCurrentFile(cf);
     judge.setCurrentProblem(cp);
     judge.setJudgeInfoPath(judgeinfopath);
     judge.setPretestCasesPath(pathtestcase);
     judge.runOnTestCases();
     cout << "Done\n";
}
int main()
{
     run();
}
