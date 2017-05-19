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

    std::string regex = "a+b+c+d";

    alg.ConvertReTo_eNfa(regex, eNFA1);


    DFA dfa = MSSC(eNFA1);

    eNFA1.FAtoDot();

    dfa.FAtoDot();

    return 0;
}
