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
    parser.parseCommands("./customCommands.json", bot);
    bot.setup(true);

    std::string test;
    if(argc == 1) test = "template";
    else if(argc == 2){
        ///// TEMP: Grab the first argument as testing regex
        test = argv[1];
    }

    /*
    std::string arg = "a0";
    std::vector<std::string> commands = {test, arg};
    std::cout << bot.executeCommand(commands) << std::endl;
     */

    bot.run();

    /*
    Fuzzy* f = bot.getFuzzy();
    //f->printAutomata();
    std::string fuzzyTest = "templade";
    std::vector<std::string> suggestions = f->fuzzy(fuzzyTest);
    for(std::string s : suggestions){
        std::cout << s << ", ";
    }
    std::cout << std::endl;
    */


    //// TFA tests ////
    /*
    ENFA enfa;
    DFA dfa1;
    DFA dfa2;
    Parser parser;
    RegToeNFA regToeNFA;
    std::string regex = "(abc)*+(def)*";

    regToeNFA.ConvertReTo_eNfa(regex, enfa);
    MSSC(enfa, dfa1);

    dfa1.FAtoDot("dfa1");

    tfa(dfa1, &dfa2);
    dfa2.FAtoDot("dfa2");
    */
    return 0;
}
