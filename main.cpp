//
// Created by sergio on 11/05/17.
//

#include "Bot.h"

int main(){
    
    //// DEBUG & TESTING ZONE ////
    std::string dfaString = "a+b+c";
    std::string test = "ab";
    Bot bot;
    bot.buildDFA(dfaString);
    DFA* botDfa = bot.getDFA();
    //botDfa->FAtoDot();
    std::cout << botDfa->checkString(test);

    return 0;
}
