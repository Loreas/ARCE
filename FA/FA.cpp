//
// Created by dawid on 13.03.17.
//

#include <iostream>
#include <fstream>

#include "FA.h"


FA::~FA() {
    for (State* state: this->states) {
        delete state;
    }
}

void FA::setAlphabet(std::set<std::string> alphabet) {
    this->alphabet = alphabet;
}

void FA::addState(State *state) {
    states.insert(state);
}

void FA::addTransition(State *stateFrom, std::string character, State *stateTo) {
    transitions[std::make_tuple(stateFrom, character)].insert(stateTo);
}

std::ostream& operator<<(std::ostream& os, const FA& Fa) {
    return os;
}

void FA::FAtoDot() {
    std::ofstream dot;
    dot.open(this->getTypeFA()+".dot");
    std::string startingState;

    dot << "digraph " << this->getTypeFA() << " {" << std::endl;
    dot << "rankdir=LR;" << std::endl;
    dot << "node [shape = doublecircle]; ";

    for (State* state :this->states) {
        if (state->isAccepting())
            dot << state->getName() << " ";
        if (state->isStarting())
            startingState = state->getName();
    }
    dot << ";" << std::endl;

    //Extra hidden node to show starting state
    dot << "node [shape = \"none\"] startStateHelper;" << std::endl;
    dot << "startStateHelper [label = \"\"];" << std::endl;

    dot << "node [shape = circle];" << std::endl << std::endl;

    dot << "startStateHelper" << " -> " << startingState << ";" << std::endl;

    std::map<std::tuple<State*, std::string>, std::set<State*>> transitions = this->getTransitions();
    //For each state, check every possible transition

    for (auto transition: transitions) {
        for (State *arrivingState: transition.second) {
            dot << std::get<0>(transition.first)->getName() << " -> " << arrivingState->getName()
                << "[label = \"" << std::get<1>(transition.first) << "\"];" << std::endl;
        }
    }

    dot << "}";
    dot.close();
}

void FA::FAtoJSON() {
    std::ofstream j;
    j.open(this->getTypeFA()+".json");

    j << "{" << std::endl;
    j << "  \"type\": \"" << this->getTypeFA() << "\", " << std::endl;

    j << "  \"alphabet\": [" << std::endl;
    for (const std::string& character: this->getAlphabet()) {
        j << "    \"" << character;
        if (this->getAlphabet().find(character) != --this->getAlphabet().end())
            j << "\",";
        j << std::endl;
    }
    j << "  ]," << std::endl;

    if (this->getTypeFA() == "eNFA")
        j << "  \"eps\": \"" << this->getEpsilon() << "\"," << std::endl;

    j << "  \"states\": [" << std::endl;
    for (State* state: this->states) {
        j << "    {" << std::endl;
        j << "      \"name\": \"" << state->getName() << "\"," << std::endl;
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
        for (State* arrivingState: transition.second) {
            j << "    {" << std::endl;
            j << "      \"from\": \"" << std::get<0>(transition.first)->getName() << "\"," << std::endl;
            j << "      \"to\": \"" << arrivingState->getName() << "\"," << std::endl;
            j << "      \"input\": \"" << std::get<1>(transition.first) << "\"";
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