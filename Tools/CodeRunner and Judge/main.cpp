#include <bits/stdc++.h>
#include "judge.h"

using namespace std;

void run()
{
     string cf, cp;

     getline(cin, cp);
     getline(cin, cf);

     Judge judge;
     judge.setCurrentFile(cf);
     judge.setCurrentProblem(cp);
     judge.runOnTestCases();
     cout << "Done\n";
}
int main()
{
     run();
}