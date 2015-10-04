#include <iostream>
#include "Regex.h"
#include "stack.h"
#include "linkTable.h"
#include <cstring>
//2015-10-4
// regex -> nfa
// nfa ->dfa
// minimize dfa ===== unfinished
using namespace std;

int main()
{
   // cout << "fuck";
    RGX* regular_experession = new RGX();
    regular_experession->run();

    return 0;
}
