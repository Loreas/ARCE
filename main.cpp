//
// Created by sergio on 11/05/17.
//

#include "Bot.h"
#include "Parser.h"
#include "TFA/tfa.h"

int main(){
    // Parser parser = Parser();
    // DFA dfa = parser.parseDFA("DFA.json");

    //// DEBUG & TESTING ZONE ////
    Parser parser;
    std::string dfaString = parser.parseRegex("./customCommands.json");
    std::string test = "a+ab";

    Bot bot;
    bot.buildDFA(dfaString, true);
    bot.parseCommand(test);

    /*
    DFA dfa = parser.parseDFA("DFA.json");
    DFA lol = tfa(dfa);
     */

    return 0;
}
