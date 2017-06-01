//
// Created by sergio on 30/05/17.
//

#ifndef ARCE_LEVENSHTEIN_H
#define ARCE_LEVENSHTEIN_H

#include "../FA/FA.h"
#include "../FA/ENFA.h"

bool fuzzy(std::string term);

void setupFuzzySearch(std::vector<std::string> terms, bool upToDate);

ENFA levenshteinAutomaton(std::string word, int k);

#endif //ARCE_LEVENSHTEIN_H
