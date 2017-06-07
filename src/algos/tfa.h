#include "../automata/FA.h"
#include "../automata/DFA.h"
#include "../automata/State.h"
#include <vector>

void tfa(DFA& dfa, DFA* return_dfa);

void addNewGroup(std::set<const State *> &newStateGroup, std::set<std::set<const State *>> &newStatesGrouped);
