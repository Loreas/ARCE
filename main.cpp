//
// Created by sergio on 11/05/17.
//
#include <iostream>
#include <thread>
#include <chrono>

#include "Bot.h"
#include "Parser.h"
#include "TFA/tfa.h"

int main(unsigned int argc, char* argv[]){

    //// DEBUG & TESTING ZONE ////
    Parser parser;
    std::string dfaString = parser.parseRegex("./customCommands.json");
    std::string test;
    if(argc == 1) test = "a";
    else if(argc == 2){
        ///// TEMP: Grab the first argument as testing regex
        test = argv[1];
    }

    Bot bot;
    bot.buildDFA(dfaString, true);
    bot.parseCommand(test);

    ////newline zeer belangrijk anders delay voor de cout gebeurt!!!
    std::cout<<"tel tot 5\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Done";
    return 0;
}
