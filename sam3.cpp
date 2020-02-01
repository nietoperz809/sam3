#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<char*> split (char* in) 
{
    vector<char*> result;
    result.push_back((char*)"-dummy-");
    char* s = strtok(in, " ");
    while (s != NULL)
    {
        result.push_back(s);
        s = strtok(NULL, " ");
    }
    return result;
}

extern "C" int samMain(int argc, char** argv);

void callMain (char* in)
{
    vector<char*> vs = split (in);
    char** a = &vs[0];
    samMain(vs.size(), a);
    //for (auto i : vs) cout << i << endl;
    //exit(0);
}

int main (int argc, char** argv)
{
    //const char* test[] = { "dummy", "peter", "hello", "world" };
    //return samMain(4, (char**)test);

    return samMain(argc, argv);

    //char arg[] = "move back motherfucker";
    //vector<char*> vs = split(arg);
    //char** arr = &split(arg)[0];

    ////for (auto i : vs) cout << i << endl;
    //exit(0);
    //callMain (arg);
    
 /*   const char *sound[] = { "", "peter", "ist", "lieb" };
    
    samMain (4, (char**)sound);
    std::cout << "Hello World!\n";*/
}

