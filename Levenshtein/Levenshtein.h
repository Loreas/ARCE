//
// Created by sergio on 30/05/17.
//

#ifndef ARCE_LEVENSHTEIN_H
#define ARCE_LEVENSHTEIN_H

#include <cmath>

#include "../MSSC/MSSC.h"
#include "../FA/FA.h"
#include "../FA/ENFA.h"
#include "../FA/DFA.h"
#include "../TFA/tfa.h"


class Fuzzy {
private:

    std::map<std::string, DFA> automata;

    ENFA levenshteinAutomaton(std::string& word, int& k);

public:
    void setupFuzzySearch(std::vector<std::string>& terms, bool upToDate);

    std::vector<std::string> fuzzy(std::string& term);

    void printAutomata() const;
};

#endif //ARCE_LEVENSHTEIN_H
