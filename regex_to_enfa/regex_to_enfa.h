//
// Created by dawid on 14.03.17.
//

#ifndef AUTOMATATHEORY_RE_TO_ENFA_H
#define AUTOMATATHEORY_RE_TO_ENFA_H

#include <stack>
#include <vector>
#include <set>
#include <tuple>

#include "../FA/eNFA.h"
#include "../FA/State.h"


class RegToeNFA {
private:
    int stateCounter;
public:
    RegToeNFA();

    void ConvertReTo_eNfa(std::string& regex, eNFA& automaton);
    std::tuple<State*, State*> buildeNFA(std::string& regex, eNFA& automaton, int currentPos=0);
    std::tuple<bool, std::set<std::string>, std::string> ValidityAndAlphabetRegex(std::string &regex);
    bool PeekNextCharStar(std::string regex, int currentPos);
    std::tuple<State*, State*> OperatorPlus(eNFA& automaton, bool& regORHappened, bool& regOR,
                                            State* firstState, State* lastState,
                                            State* currentFirst, State* currentLast);
    std::tuple<State*, State*> OperatorStar(eNFA& automation,
                                                       State* currentFirstState, State* currentLastState);
};
#endif //AUTOMATATHEORY_RE_TO_ENFA_H
