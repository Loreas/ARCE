//
// Created by sergio on 11/05/17.
//

#include "tfa.h"
#include <algorithm>

DFA tfa(DFA& dfa) {
    typedef const State* obj_state;
    typedef std::vector<State*> vec_state;
    typedef std::set<const State*> set_state;
    typedef std::map<std::tuple<const State*, std::string>, std::set<const State*>> map_trans;

    DFA return_dfa;
    return_dfa.setAlphabet(dfa.getAlphabet());
    map_trans transitions = dfa.getTransitions();

    std::vector<const State*> states;
    states.assign(dfa.getStates().begin(), dfa.getStates().end());

    int state_amount = dfa.getStates().size();

    std::vector<std::vector<std::tuple<const State*, const State*, int>>> statePairs;

    // Matrix initializtion
    for (int i = 0; i < state_amount - 1; i++) {
        std::vector<std::tuple<const State*, const State*, int>> pairs(i + 1);
        statePairs.push_back(pairs);
    }
    int table_size = statePairs.size();

    for (int r = 0; r <= table_size - 1; r++) {
        for (int c = 0; c <= r; c++) {
            std::get<0>(statePairs[r][c]) = states[c];
            std::get<1>(statePairs[r][c]) = states[r + 1];
            std::get<2>(statePairs[r][c]) = -1;
            if (states[c]->isAccepting() xor states[r + 1]->isAccepting()) {
                std::get<2>(statePairs[r][c]) = 0;
            }
        }
    }

    // Dit deel zou fix moeten zijn
    // Dist == distinguishable
    bool newDist = true;
    int lastDist = 0;
    while (newDist) {
        newDist = false;
        for (int r = 0; r <= table_size - 1; r++) {
            for (int c = 0; c <= r; c++) {
                if (std::get<2>(statePairs[c][r]) == lastDist) {
                    obj_state first = std::get<0>(statePairs[r][c]);
                    obj_state second = std::get<1>(statePairs[r][c]);
                    int newR, newC; // column/row of newly found distinguishable state
                    for (auto& ch: dfa.getAlphabet()) {
                        /*
                        bool fstUsed = false;
                        bool sndUsed = false;
                        for (auto it = states.begin(); it != states.end(); it++) {
                            set_state transition = transitions[std::make_tuple(*it, ch)];
                            if (transition.find(first) != transition.end() and !fstUsed) {
                                fstUsed = true;
                                int temp = it - states.begin();
                                if (temp != state_amount - 1) {
                                    newC = temp;
                                }
                                else {
                                    newC = newR;
                                    newR = temp - 1;
                                }
                            }
                            else if (transition.find(second) != transition.end() and !sndUsed) {
                                sndUsed = true;
                                int temp = it - states.begin();
                                if (temp != 0) {
                                    newR = temp - 1;
                                }
                                else {
                                    newR = newC;
                                    newC = temp;
                                }
                            }
                        }
                        if (fstUsed and sndUsed) {
                            std::get<2>(statePairs[newR][newC]) = lastDist + 1;
                            newDist = true;
                        }
                         */
                        for (int i = 0; i < state_amount - 1; i++) {
                            bool checkFirst ;

                            set_state transition1 = transitions[std::make_tuple(states[i+1], ch)];
                            if (transition1.find(first) != transition1.end()) checkFirst = false;
                            else if (transition1.find(second) != transition1.end()) checkFirst = true;
                                // No valid transition for current cell
                            else continue;

                            for (int j = 0; j <= i; j++) {
                                set_state transition2 = transitions[std::make_tuple(states[j], ch)];
                                if (checkFirst) {
                                    if (transition2.find(first) != transition2.end()) {
                                        if (std::get<2>(statePairs[i][j]) == -1) {
                                            std::get<2>(statePairs[i][j]) = lastDist + 1;
                                            newDist = true;
                                        }
                                    }
                                } else if (transition2.find(second) != transition2.end()) {
                                    if (std::get<2>(statePairs[i][j]) == -1) {
                                        std::get<2>(statePairs[i][j]) = lastDist + 1;
                                        newDist = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        lastDist++;
    }

    set_state newStates;
    std::map<std::string, const State*> temp_trans;

    std::set<std::set<const State*>> newStatesGrouped;
    std::set<const State*> unusedStates(states.begin(), states.end());


    // Nieuwe staten rijen
    for (int r = 0; r <= table_size - 1; r++){
        bool equivalentFound = false;
        std::set<const State*> newStateGroup;

        // Second state is same one for whole row
        const State* fst_state = std::get<1>(statePairs[r][0]);
        newStateGroup.insert(fst_state);
        for (int c = 0; c <= r; c++) {
            // Equivalent states
            if (std::get<2>(statePairs[r][c]) == -1) {
                equivalentFound = true;
                const State* snd_state = std::get<0>(statePairs[r][c]);
                newStateGroup.insert(snd_state);
                // this state is used, no longer to be added as single state
                unusedStates.erase(snd_state);
            }
        }

        if (equivalentFound) {
            // this state is used, no longer to be added as single state
            unusedStates.erase(std::get<1>(statePairs[r][0]));
            if (newStatesGrouped.find(newStateGroup) == newStatesGrouped.end()) {
                newStatesGrouped.insert(newStateGroup);
            }
        }
    }

    // Nieuwe staten kolommen
    for (int c = 0; c <= table_size - 1; c++){
        bool equivalentFound = false;
        std::set<const State*> newStateGroup;

        // First state is same one for whole collumn
        const State* fst_state = std::get<0>(statePairs[0][c]);
        newStateGroup.insert(fst_state);
        for (int r = c; r < table_size ; r++) {
            // Equivalent states
            if (std::get<2>(statePairs[r][c]) == -1) {
                equivalentFound = true;
                const State* snd_state = std::get<1>(statePairs[r][c]);
                newStateGroup.insert(snd_state);
                // this state is used, no longer to be added as single state
                unusedStates.erase(snd_state);
            }
        }

        if (equivalentFound) {
            // this state is used, no longer to be added as single state
            unusedStates.erase(std::get<0>(statePairs[0][c]));
            if (newStatesGrouped.find(newStateGroup) == newStatesGrouped.end()) {
                newStatesGrouped.insert(newStateGroup);
            }
        }
    }

    // Ongebruikte staten
    for (const State* state : unusedStates) {
        std::set<const State*> newStateGroup;
        newStateGroup.insert(state);
        newStatesGrouped.insert(newStateGroup);
    }

    // Create new states from the created groups
    for (std::set<const State*> stateGroup : newStatesGrouped) {
        std::string newName = "{";
        bool starting = false;
        bool accepting = false;
        for (const State* state : stateGroup) {
            std::string name = state->getName();
            name.erase(0, 1);
            name.pop_back();
            newName += name + ", ";

            if (state->isStarting()) starting = true;
            if (state->isAccepting()) accepting = true;
        }
        // Remove last ", "
        newName.pop_back(); newName.pop_back();
        newName += "}";
        const State* newState = new State(newName, starting, accepting);
        return_dfa.addState(newState);
        newStates.insert(newState);

        if (starting) return_dfa.setStartstate(newState);
    }

    // Create the new transitions

    /*  for each newState
     *      find an original state for that group
     *      for each char
     *          find transition
     *          find newState containing that name
     *          add Transition
     */

    for (const State* newState: newStates) {
        const State* firstState;
        // Find an original state of this one
        for (const State* state: states) {
            std::string name = state->getName();
            name.erase(0, 1);
            name.pop_back();

            if (newState->getName().find(name) != std::string::npos) {
                firstState = state;
                break;
            }
        }

        for (auto& ch: return_dfa.getAlphabet()) {
            const State* state = (*transitions[std::make_tuple(firstState, ch)].begin());
            std::string name = state->getName();
            name.erase(0, 1);
            name.pop_back();

            // Find new state containing the found name
            for (const State* newState2: newStates) {
                // If found add transion from newState to newState2
                if (newState2->getName().find(name) != std::string::npos) {
                    return_dfa.addTransition(newState, ch, newState2);
                    break;
                }
            }
        }
     }


    /*
     * for (int r = 0; r <= table_size - 1; r++) {
        for (int c = 0; c <= r; c++) {
            auto tup = statePairs[r][c];
            auto first = std::get<0>(tup);
            auto second = std::get<1>(tup);
            // Equivalent states
            if (std::get<2>(statePairs[r][c]) == -1) {
                std::string name = "";
                bool starting = false;
                bool accepting = false;
                auto fst_name = first->getName();
                auto snd_name = second->getName();
                fst_name.pop_back();
                snd_name.erase(0, 1);
                name = fst_name + ", " + snd_name;
                if (first->isStarting() or second->isStarting())
                    starting = true;
                if (first->isAccepting() or second->isAccepting())
                    accepting = true;
                obj_state eq_state = new State(name, starting, accepting);
                return_dfa.addState(eq_state);
                if (starting) return_dfa.setStartstate(eq_state);
                temp_trans[name] = eq_state;
            }
            // Distinguishable states
            else {
                return_dfa.addState(first);
                if (first->isStarting()) return_dfa.setStartstate(first);
                temp_trans[first->getName()] = first;
                return_dfa.addState(second);
                if (second->isStarting()) return_dfa.setStartstate(second);
                temp_trans[second->getName()] = second;
            }
        }
    }

    for (int r = 0; r <= table_size - 1; r++) {
        for (int c = 0; c <= r; c++) {
            auto tup = statePairs[r][c];
            auto first = std::get<0>(tup);
            auto second = std::get<1>(tup);
            for (auto& ch : dfa.getAlphabet()) {
                // Equivalent states
                if (std::get<2>(statePairs[r][c]) == -1) {
                    std::string name = "";
                    bool starting = false;
                    bool accepting = false;
                    auto fst_name = first->getName();
                    auto snd_name = second->getName();
                    fst_name.pop_back();
                    snd_name.erase(0, 1);
                    // name = fst_name + ", " + snd_name;
                    name.append(fst_name);
                    name.append(", ");
                    name.append(snd_name);
                    std::string to_name = "";
                    auto fst_trans = (transitions[std::make_tuple(first, ch)].begin());
                    auto snd_trans = (transitions[std::make_tuple(second, ch)].begin());
                    auto fst_to_name = (*fst_trans)->getName();
                    auto snd_to_name = (*snd_trans)->getName();
                    fst_to_name.pop_back();
                    snd_to_name.erase(0, 1);
                    to_name = fst_to_name + ", " + snd_to_name;
                    return_dfa.addTransition(temp_trans[name], ch, temp_trans[to_name]);
                }
                // Distinguishable states
                else {
                    return_dfa.addTransition(first, ch, (*transitions[std::make_tuple(first, ch)].begin()));
                    return_dfa.addTransition(second, ch, (*transitions[std::make_tuple(second, ch)].begin()));
                }
            }
        }
    }
    // TODO: Remove testing below
    for (int r = 0; r <= table_size - 1; r++) {
        for (int c = 0; c <= r; c++) {
            std::cout << std::get<2>(statePairs[r][c]);
            if (c != r) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    */

    return return_dfa;
}
