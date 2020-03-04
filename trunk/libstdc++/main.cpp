
/*
#include <functional>
#include <stdlib.h>
#include <swilib.h>
#include <string>
#include <iostream>
#include <fstream>



int main()
{
    std::ofstream myfile;
    myfile.open( "4:\\elf\\example.txt" );

    myfile << "Writing";

    myfile.close();

    kill_elf();
}
*/


extern "C" {

int __dso_handle;


void __cxa_atexit() {}

void __sync_synchronize(){}

bool __atomic_compare_exchange_4(volatile void*, void*, unsigned int, bool, int, int) {
    return false;
}

}


