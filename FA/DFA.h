//
// Created by dawid on 13.03.17.
//

#ifndef AUTOMATATHEORY_DFA_H
#define AUTOMATATHEORY_DFA_H

#include "FA.h"

class DFA: public FA{
public:
    DFA();
    DFA(const DFA& otherDFA);
    const State* delta(const State* from, std::string c);
    virtual bool Validate() {return true;}
    virtual bool checkString(std::string s);

    DFA& operator=(const DFA& otherDFA);
};


#endif //AUTOMATATHEORY_DFA_H
