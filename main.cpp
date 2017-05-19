//
// Created by sergio on 11/05/17.
//

#include "Bot.h"

int main(){

    //// DEBUG & TESTING ZONE ////
    std::string dfaString = "a+b+c";
    std::string test = "when u get dragged back in2 slaver3";
    Bot bot;
    bot.buildDFA(dfaString);
    DFA* botDfa = bot.getDFA();
    //botDfa->FAtoDot();
    botDfa->checkString(test);

    return 0;
}
