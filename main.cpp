//
// Created by sergio on 11/05/17.
//
#include <iostream>
#include <stdio.h>
#include <iostream>

#include <fstream>
#include "Bot.h"
#include "Parser.h"
#include "Levenshtein/Levenshtein.h"
#include "MSSC/MSSC.h"
#include "TFA/tfa.h"

void bulkTest(std::string fileName, DFA& dfa){
    std::fstream file(fileName);
    if(!file.is_open()) return;

    std::string line;
    while(getline(file, line)){
        if(dfa.checkString(line))
            std::cout << "Accepted string: " << line << std::endl;
        else
            std::cout << "Rejected string: " << line << std::endl;
    }

}

int main(unsigned int argc, char* argv[]){


    //// DEBUG & TESTING ZONE ////
    Bot bot;
    Parser parser;
    parser.parseCommands("./customCommands.json", bot, true);
    bot.setup(true);

    std::string test;
    if(argc == 1) test = "addnote";
    else if(argc == 2){
        ///// TEMP: Grab the first argument as testing regex
        test = argv[1];
    }

    std::string arg1 = "test";
    std::string arg2 = "testingnote";
    std::vector<std::string> commands = {test, arg1, arg2};
    std::cout << bot.executeCommand(commands) << std::endl;

    return 0;
}
