#include <iostream>
#include "base/Logging.h"
using namespace std;
int main() {
    for(int i = 0; i < 1000000; ++i){
        LOG<<i;
    }
    return 0;
}
