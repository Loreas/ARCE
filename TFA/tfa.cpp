//
// Created by sergio on 11/05/17.
//

#include "tfa.h"
#include <algorithm>

void tfa(DFA& dfa, DFA* return_dfa) {
    typedef const State* obj_state;
    typedef std::vector<State*> vec_state;
    typedef std::set<const State*> set_state;
    typedef std::map<std::tuple<const State*, std::string>, std::set<const State*>> map_trans;

    return_dfa->setAlphabet(dfa.getAlphabet());
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
                if (std::get<2>(statePairs[r][c]) == lastDist) {
                    obj_state first = std::get<0>(statePairs[r][c]);
                    obj_state second = std::get<1>(statePairs[r][c]);
                    for (auto& ch: dfa.getAlphabet()) {
                        for (int i = 0; i < state_amount - 1; i++) {
                            bool checkFirst ;

                            set_state transition1 = transitions[std::make_tuple(states[i], ch)];
                            if (transition1.find(first) != transition1.end()) checkFirst = false;
                            else if (transition1.find(second) != transition1.end()) checkFirst = true;
                                // No valid transition for current cell
                            else continue;

                            for (int j = i+1; j < state_amount; j++) {
                                set_state transition2 = transitions[std::make_tuple(states[j], ch)];
                                if (checkFirst) {
                                    if (transition2.find(first) != transition2.end()) {
                                        if (std::get<2>(statePairs[j-1][i]) == -1) {
                                            std::get<2>(statePairs[j-1][i]) = lastDist + 1;
                                            newDist = true;
                                        }
                                    }
                                } else if (transition2.find(second) != transition2.end()) {
                                    if (std::get<2>(statePairs[j-1][i]) == -1) {
                                        std::get<2>(statePairs[j-1][i]) = lastDist + 1;
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
            addNewGroup(newStateGroup, newStatesGrouped);
        }
    }

    // Nieuwe staten kolommen
    for (int c = 0; c <= table_size - 1; c++){
        bool equivalentFound = false;
        std::set<const State*> newStateGroup;

        // First state is same one for whole collumn
        const State* fst_state = std::get<0>(statePairs[table_size-1][c]);
        newStateGroup.insert(fst_state);
        for (int r = 0; r <= c; r++) {
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
            unusedStates.erase(std::get<0>(statePairs[table_size-1][c]));
            addNewGroup(newStateGroup, newStatesGrouped);
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
        std::set<std::string> newName;
        bool starting = false;
        bool accepting = false;
        for (const State* state : stateGroup) {
            std::string name = state->getName();
            if (name.front() == '{') name.erase(0, 1);
            if (name.back() == '}') name.pop_back();
            newName.insert(name);

            if (state->isStarting()) starting = true;
            if (state->isAccepting()) accepting = true;
        }
        const State* newState = new State(newName, starting, accepting);
        return_dfa->addState(newState);
        newStates.insert(newState);

        if (starting) return_dfa->setStartstate(newState);
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
            if (name.front() == '{') name.erase(0, 1);
            if (name.back() == '}') name.pop_back();

            if (newState->containsName(name)) {
                firstState = state;
                break;
            }
        }

        for (auto& ch: return_dfa->getAlphabet()) {
            const State* state = (*transitions[std::make_tuple(firstState, ch)].begin());
            std::string name = state->getName();
            if (name.front() == '{') name.erase(0, 1);
            if (name.back() == '}') name.pop_back();

            // Find new state containing the found name
            for (const State* newState2: newStates) {
                // If found add transion from newState to newState2
                if (newState2->getName().find(name) != std::string::npos) {
                    return_dfa->addTransition(newState, ch, newState2);
                    break;
                }
            }
        }
     }

    return_dfa->renameStates();
}

void addNewGroup(std::set<const State *> &newStateGroup, std::set<std::set<const State *>> &newStatesGrouped) {
    // Check if grouping exist that contains some elements of the new group
    // if true, add to that group
    // else add as new group

    for (const std::set<const State*>& existingStateGroup: newStatesGrouped) {
        // Check if a grouping exist that contains some of the newly grouped states
        std::set<const State*> set_intersection;
        std::set_intersection(existingStateGroup.begin(), existingStateGroup.end(), newStateGroup.begin(), newStateGroup.end(), std::inserter(set_intersection, set_intersection.begin()));
        if (set_intersection.size() > 0){
            for (const State* state: existingStateGroup) {
                newStateGroup.insert(state);
            }
            newStatesGrouped.erase(existingStateGroup);
            break;
        }
    }

    newStatesGrouped.insert(newStateGroup);
}