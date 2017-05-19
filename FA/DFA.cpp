//
// Created by dawid on 13.03.17.
//

#include "DFA.h"

DFA::DFA() {
    this->setTypeFA("DFA");
}

bool DFA::checkString(std::string s) {

    for(int curI = 0; curI < s.size(); curI++){
        std::string curChar(1, s[curI]);
        std::cout << curChar << ", ";
    }
    std::cout << ".\n";
}