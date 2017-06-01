//
// Created by dawid on 13.03.17.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "FA.h"

FA::FA(const FA &otherFA) {
    this->type = otherFA.type;
    this->alphabet = otherFA.alphabet;

    // Deep copy al states

    for (const State* state: otherFA.states) {
        State* newState = new State(state->getName(), state->isStarting(), state->isAccepting());
        this->addState(newState);
        if (newState->isStarting())
            this->startstate = newState;
    }

    // Copy all transitions
    typedef std::tuple<std::string, std::string, std::string> transInfo;
    std::vector<transInfo> transitions;

    for (auto trans: otherFA.transitions) {
        std::string from = std::get<0>(std::get<0>(trans))->getName();
        std::string c = std::get<1>(std::get<0>(trans));

        for (const State* state: std::get<1>(trans)) {
            std::string to = state->getName();

            transInfo newTrans = std::make_tuple(from, to, c);
            transitions.push_back(newTrans);
        }
    }

    // Add transitions
    for(transInfo trans : transitions){
        std::string fromName = std::get<0>(trans);
        std::string toName = std::get<1>(trans);
        const State* from = nullptr;
        const State* to = nullptr;
        // TODO: temp fix
        for(const State* s : this->getStates()){
            if(s->getName() == fromName) from = s;
            if(s->getName() == toName) to = s;
            if(to != nullptr and from != nullptr) break;
        }
        std::string c = std::get<2>(trans);
        this->addTransition(from, c ,to);
    }
}

FA::~FA() {
    for (const State* state: this->states) {
        delete state;
    }
}

void FA::setAlphabet(std::set<std::string> alphabet) {
    this->alphabet = alphabet;
}

void FA::setStartstate(const State *state) {
    this->startstate = state;
}

void FA::addState(const State *state) {
    states.insert(state);
}

void FA::addTransition(const State *stateFrom, std::string character, const State *stateTo) {
    if (character != "*") transitions[std::make_tuple(stateFrom, character)].insert(stateTo);
    else {
        std::set<std::string> alph = {"a", "b", "c", "d", "e", "f", "g", "h", "i",
                                      "j", "k", "l", "m", "n", "o", "p", "q", "r",
                                      "s", "t", "u", "v", "w", "x", "y", "z"};
        for (auto ch : alph) {
            transitions[std::make_tuple(stateFrom, ch)].insert(stateTo);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const FA& Fa) {
    return os;
}

void FA::FAtoJSON() {
    std::ofstream j;
    j.open(this->getTypeFA()+".json");

    j << "{" << std::endl;
    j << "  \"type\": \"" + this->getTypeFA() << "\", " << std::endl;

    j << "  \"alphabet\": [" << std::endl;
    for (const std::string& character: this->getAlphabet()) {
        j << "    \"" + character;
        if (this->getAlphabet().find(character) != --this->getAlphabet().end())
            j << "\",";
        j << std::endl;
    }
    j << "  ]," << std::endl;

    if (this->getTypeFA() == "ENFA")
        j << "  \"eps\": \"" + this->getEpsilon() << "\"," << std::endl;

    j << "  \"states\": [" << std::endl;
    for (const State* state: this->states) {
        j << "    {" << std::endl;
        j << "      \"name\": \"" + state->getName() << "\"," << std::endl;
        j << "      \"starting\": " << state->isStarting() << "," << std::endl;
        j << "      \"accepting\": " << state->isAccepting() << std::endl;
        j << "    }";
        if (this->states.find(state) != --this->states.end()) {
            j << ",";
        }
        j << std::endl;
    }
    j << "  ]," <<std::endl;


    j << "  \"transitions\": [" << std::endl;
    for (auto transition: transitions) {
        for (const State* arrivingState: transition.second) {
            j << "    {" << std::endl;
            j << "      \"from\": \"" + std::get<0>(transition.first)->getName() << "\"," << std::endl;
            j << "      \"to\": \"" + arrivingState->getName() << "\"," << std::endl;
            j << "      \"input\": \"" + std::get<1>(transition.first) << "\"";
            if ((transition.second.find(arrivingState) != --transition.second.end()) &&
                transitions.find(transition.first) != --transitions.end()) {
                j << ","; }
            j << std::endl;
            j << "    }" << std::endl;
        }

    }
    j << "  ]" << std::endl;

    j << "}";
    j.close();
}

void FA::FAtoDot(std::string name) const {
    std::ofstream dot;
    if (name == "") name = this->getTypeFA()+".dot";
    dot.open(name);
    std::string startingState;

    dot << "digraph " + this->getTypeFA() + " {" << std::endl;
    dot << "rankdir=LR;" << std::endl;
    dot << "node [shape = doublecircle]; ";

    startingState = this->getStartstate()->getName();

    for (const State* state :this->states) {
        if (state->isAccepting())
            dot << "\"" + state->getName() + "\"" << " ";
    }
    dot << ";" << std::endl;

    //Extra hidden node to show starting state
    dot << "node [shape = \"none\"] startStateHelper;" << std::endl;
    dot << "startStateHelper [label = \"\"];" << std::endl;

    dot << "node [shape = circle];" << std::endl << std::endl;

    dot << "startStateHelper" << " -> " << "\"";
    dot << startingState << "\"" << ";" << std::endl;

    std::map<std::tuple<const State*, std::string>, std::set<const State*>> transitions = this->getTransitions();
    //For each state, check every possible transition

    for (auto transition: transitions) {
        for (const State *arrivingState: transition.second) {
            dot << "\"" + std::get<0>(transition.first)->getName() << "\"" << " -> " <<
                "\"" << arrivingState->getName() << "\"" << "[label = \"" << std::get<1>(transition.first) << "\"];"
                << std::endl;
        }
    }

    dot << "}";
    dot.close();
}

FA& FA::operator=(const FA& otherFA) {
    this->type = otherFA.type;
    this->alphabet = otherFA.alphabet;

    // Deep copy al states

    for (const State* state: otherFA.states) {
        State* newState = new State(state->getName(), state->isStarting(), state->isAccepting());
        this->addState(newState);
        if (newState->isStarting())
            this->startstate = newState;
    }

    // Copy all transitions
    typedef std::tuple<std::string, std::string, std::string> transInfo;
    std::vector<transInfo> transitions;

    for (auto trans: otherFA.transitions) {
        std::string from = std::get<0>(std::get<0>(trans))->getName();
        std::string c = std::get<1>(std::get<0>(trans));

        for (const State* state: std::get<1>(trans)) {
            std::string to = state->getName();

            transInfo newTrans = std::make_tuple(from, to, c);
            transitions.push_back(newTrans);
        }
    }

    // Add transitions
    for(transInfo trans : transitions){
        std::string fromName = std::get<0>(trans);
        std::string toName = std::get<1>(trans);
        const State* from = nullptr;
        const State* to = nullptr;
        // TODO: temp fix
        for(const State* s : this->getStates()){
            if(s->getName() == fromName) from = s;
            if(s->getName() == toName) to = s;
            if(to != nullptr and from != nullptr) break;
        }
        std::string c = std::get<2>(trans);
        this->addTransition(from, c ,to);
    }

    return (*this);
}