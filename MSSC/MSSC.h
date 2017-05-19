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

#include "../FA/FA.h"
#include "../FA/State.h"
#include "../FA/DFA.h"
#include "../FA/eNFA.h"

std::string getSSname(std::vector<const State*> ss);

DFA MSSC(eNFA& enfa, DFA& dfa);

#endif //ARCE_MSSC_H
