//
// Created by jonathan on 19.05.17.
//

#ifndef ARCE_MSSC_H
#define ARCE_MSSC_H

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <algorithm>

#include "../automata/FA.h"
#include "../automata/State.h"
#include "../automata/DFA.h"
#include "../automata/ENFA.h"

std::string getSSname(std::vector<const State*>& ss);

void MSSC(ENFA& enfa, DFA& dfa);

#endif //ARCE_MSSC_H
