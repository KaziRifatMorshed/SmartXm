#include<bits/stdc++.h>
#include "codeRunner.h"

int main()
{
     std::string file;
     getline(std::cin,file);
     CodeRunner runner;
     runner.setCurrentFile(file);
     runner.run(); 
}