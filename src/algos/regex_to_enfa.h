//
// Created by dawid on 14.03.17.
//

#ifndef AUTOMATATHEORY_RE_TO_ENFA_H
#define AUTOMATATHEORY_RE_TO_ENFA_H

#include <stack>
#include <vector>
#include <set>
#include <tuple>

#include "../automata/ENFA.h"
#include "../automata/State.h"


class RegToeNFA {
private:
    int stateCounter;

    std::tuple<State*, State*> buildeNFA(std::string &regex, ENFA &automaton, int currentPos = 0);

    std::tuple<bool, std::set<std::string>, std::string> ValidityAndAlphabetRegex(std::string &regex);

    bool checkConcatenationPrevious(ENFA &automaton, std::string regex, int currentPos);

    bool checkConcatenationNext(ENFA &automaton, std::string regex, int currentPos);

    bool PeekNextCharStar(std::string regex, int currentPos);

    std::tuple<State *, State *> OperatorPlus(ENFA &automaton, bool &regOR, State *firstState, State *lastState, State *currentFirst,
                                              State *currentLast);

    std::tuple<State*, State*> OperatorStar(ENFA& automation, State* currentFirstState,
                                            State* currentLastState);
public:
    RegToeNFA();

    void ConvertReTo_eNfa(std::string& regex, ENFA& automaton);

};
#endif //AUTOMATATHEORY_RE_TO_ENFA_H
