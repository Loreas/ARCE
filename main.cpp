//
// Created by sergio on 11/05/17.
//

#include "FA/DFA.h"
#include "FA/eNFA.h"
#include "regex_to_enfa/regex_to_enfa.h"
#include "MSSC/Fase1.cpp"

int main(){

    eNFA eNFA1 = eNFA();

    RegToeNFA alg = RegToeNFA();

    std::string regex = "a+b+cd";

    alg.ConvertReTo_eNfa(regex, eNFA1);


    DFA dfa = DFA();
    MSSC(eNFA1, dfa);

    eNFA1.FAtoDot();
    eNFA1.FAtoJSON();

    dfa.FAtoDot();
    dfa.FAtoJSON();

    return 0;
}
