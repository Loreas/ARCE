//
// Created by dawid on 13.03.17.
//

#include "DFA.h"

DFA::DFA() {
    this->setTypeFA("DFA");
}

DFA::DFA(const DFA& otherDFA)
: FA(otherDFA) {

}