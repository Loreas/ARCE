//
// Created by sergio on 11/05/17.
//

#include "Bot.h"

int main(){
    /*
    eNFA eNFA1 = eNFA();

    RegToeNFA alg = RegToeNFA();

    std::string regex = "a+b+c+d";

    alg.ConvertReTo_eNfa(regex, eNFA1);


    DFA dfa = DFA();
    MSSC(eNFA1, dfa);

    eNFA1.FAtoDot();
    eNFA1.FAtoJSON();

    dfa.FAtoDot();
    dfa.FAtoJSON();
     */

    //// DEBUG & TESTING ZONE ////
    std::string test = "when u get dragged back in2 slaver3";
    Bot bot;
    bot.buildDFA(test);

    return 0;
}
