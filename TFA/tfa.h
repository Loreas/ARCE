#include "../FA/FA.h"
#include "../FA/DFA.h"
#include "../FA/State.h"
#include <vector>

void tfa(DFA& dfa, DFA* return_dfa);

void addNewGroup(std::set<const State *> &newStateGroup, std::set<std::set<const State *>> &newStatesGrouped);