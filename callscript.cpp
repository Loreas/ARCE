#include <iostream>
using namespace std;
void call_script(string filename) {

    string command = "python " + filename;

    system(command.c_str());
}

void run_all_scripts(){
    string command = "for f in *.py;do python ";
           command += "$f";
           command += "; done";

    system(command.c_str());


}
