//
// Created by sergio on 11/05/17.
//

#include "Bot.h"
#include "Parser.h"

int main(){

    //// DEBUG & TESTING ZONE ////
    std::string dfaString = "a+b+c";
    std::string test = "ab";
    Bot bot;
    bot.buildDFA(dfaString);
    DFA* botDfa = bot.getDFA();
    //botDfa->FAtoDot();
    std::cout << botDfa->checkString(test);

    //Parser parser = Parser();
    //DFA dfa = parser.parseDFA("DFA.json");


    return 0;
}
