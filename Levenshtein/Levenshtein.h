//
// Created by sergio on 30/05/17.
//

#ifndef ARCE_LEVENSHTEIN_H
#define ARCE_LEVENSHTEIN_H

#include "../FA/FA.h"
#include "../FA/ENFA.h"

ENFA levenshteinAutomaton(std::string word, int k);

#endif //ARCE_LEVENSHTEIN_H
