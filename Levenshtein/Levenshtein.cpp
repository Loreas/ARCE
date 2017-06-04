//
// Created by sergio on 30/05/17.
//

#include "Levenshtein.h"
#include "../Parser.h"

std::vector<std::string> Fuzzy::fuzzy(std::string& term){
    std::vector<std::string> possibleTerms;
    for (auto& pair : automata) {
        if (pair.second.checkString(term)) possibleTerms.push_back(pair.first);
    }
    return possibleTerms;
}

void Fuzzy::setupFuzzySearch(std::vector<std::string>& terms, bool upToDate) {
    if (upToDate){
        // Read all DFA's from the .config folder
        for (std::string& term : terms){
            Parser parser;
            std::string filename = "../.config/" + term + ".json";
            DFA dfa = parser.parseDFA(filename);
            automata[term] = dfa;
        }
    }
    else {
        for (std::string &term : terms) {
            int maxDistance = std::ceil(term.size() / 5.0);
            ENFA lev = levenshteinAutomaton(term, maxDistance);
            DFA dfa;
            MSSC(lev, dfa);
            DFA smallDFA;
            tfa(dfa, &smallDFA);
            automata[term] = dfa; // TODO: apply TFA
            // Write to .config folder
            std::string filename = "./.config/" + term;
            smallDFA.FAtoJSON(filename);
        }
    }
}

ENFA Fuzzy::levenshteinAutomaton(std::string& word, int& k) {
    ENFA lev;
    std::set<std::string> alph = {"a", "b", "c", "d", "e", "f", "g", "h", "i",
                                  "j", "k", "l", "m", "n", "o", "p", "q", "r",
                                  "s", "t", "u", "v", "w", "x", "y", "z"};
    lev.setAlphabet(alph);
    // lev.setEpsilon("\u03B5");
    std::map<std::string, State*> states;
    for (int i = 0; i < word.size(); i++) {
        for (int e = 0; e < k + 1; e++) { // e = number of errors
            std::string from_name = std::to_string(i) + "^" + std::to_string(e);
            State* from;
            if (states.count(from_name) != 0) from = states[from_name];
            else {
                bool starting = (i == 0 and e == 0);
                from = new State(from_name, starting, false);
                if (starting) lev.setStartstate(from);
                states[from_name] = from;
            }
            lev.addState(from);

            // Correct character
            std::string to_name = std::to_string(i + 1) + "^" + std::to_string(e);
            State* to;
            if (states.count(to_name) != 0) to = states[to_name];
            else {
                bool accepting = (i + 1 == word.size());
                to = new State(to_name, false, accepting);
                states[to_name] = to;
            }
            lev.addState(to);
            lev.addTransition(from, std::string(1, word[i]), to);
            if (e < k) {
                // Deletion
                to_name = std::to_string(i) + "^" + std::to_string(e + 1);
                if (states.count(to_name) != 0) to = states[to_name];
                else {
                    to = new State(to_name, false, false);
                    states[to_name] = to;
                }
                lev.addState(to);
                lev.addTransition(from, "*", to);

                // Insertion
                to_name = std::to_string(i + 1) + "^" + std::to_string(e + 1);
                if (states.count(to_name) != 0) to = states[to_name];
                else {
                    bool accepting = (i + 1 == word.size());
                    to = new State(to_name, false, accepting);
                    states[to_name] = to;
                }
                lev.addState(to);
                lev.addTransition(from, lev.getEpsilon(), to);

                // Substitution
                to_name = std::to_string(i + 1) + "^" + std::to_string(e + 1);
                if (states.count(to_name) != 0) to = states[to_name];
                else {
                    bool accepting = (i + 1 == word.size());
                    to = new State(to_name, false, accepting);
                    states[to_name] = to;
                }
                lev.addState(to);
                lev.addTransition(from, "*", to);
            }
        }
    }
    for (int e = 0; e < k + 1; e++) {
        if (e < k) {
            std::string from_name = std::to_string(word.size()) + "^" + std::to_string(e);
            State* from;
            if (states.count(from_name) != 0) from = states[from_name];
            else {
                from = new State(from_name, false, true);
            states[from_name] = from;
            }
            lev.addState(from);
            std::string to_name = std::to_string(word.size()) + "^" + std::to_string(e + 1);
            State* to;
            if (states.count(to_name) != 0) to = states[to_name];
            else {
                to = new State(to_name, false, true);
                states[to_name] = to;
            }
            lev.addState(to);
            lev.addTransition(from, "*", to);
        }
    }
    return lev;
}

void Fuzzy::printAutomata() const {
    for(auto& p : automata){
        std::string fn = p.first;
        p.second.FAtoDot(fn);
    }
}