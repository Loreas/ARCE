//
// Created by sergio on 11/05/17.
//
#include <iostream>
#include <thread>
#include <chrono>

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
    Parser parser;
    std::string dfaString = parser.parseRegex("./customCommands.json");
    std::string test;
    if(argc == 1) test = "List1";
    else if(argc == 2){
        ///// TEMP: Grab the first argument as testing regex
        test = argv[1];
    }

    Bot bot;
    bot.buildDFA(dfaString, true);

    bulkTest("./testInput.txt", *bot.getDFA());

    ENFA lev = levenshteinAutomaton("food", 3);

    ////newline zeer belangrijk anders delay voor de cout gebeurt!!!
    std::cout<<"tel tot 5\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Done";
    return 0;
}
