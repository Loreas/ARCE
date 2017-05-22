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
                    }
                }
            }
        }
        lastDist++;
    }

    set_state newStates;
    std::map<std::string, const State*> temp_trans;

    for (int r = 0; r <= table_size - 1; r++) {
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

    return return_dfa;
}
