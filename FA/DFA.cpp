//
// Created by dawid on 13.03.17.
//

#include "DFA.h"

DFA::DFA() {
    this->setTypeFA("DFA");
}

DFA::DFA(const DFA& otherDFA)
: FA(otherDFA) {}

bool DFA::checkString(std::string s) {

    const std::set<std::string> alph = getAlphabet();
    const State* curState = getStartstate();
    std::map<std::tuple<const State*, std::string>, std::set<const State*>> transitions = getTransitions();

    for(int curI = 0; curI < s.size(); curI++){
        std::string curChar(1, s[curI]);
        if(alph.find(curChar) == alph.end()) return false;
        std::tuple<const State*, std::string> arg = std::make_tuple(curState, curChar);
        // Check if the current state has a transition using this char
        if(transitions.count(arg) != 0){
            curState = *(transitions.at(arg).begin()); // TODO : oh shit, look at this -S. Fenoll
        }
    }
    return curState->isAccepting();
}
