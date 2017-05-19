//
// Created by dawid on 13.03.17.
//

#ifndef AUTOMATATHEORY_DFA_H
#define AUTOMATATHEORY_DFA_H

#include "FA.h"

class DFA: public FA{
public:
    DFA();
    virtual bool Validate() {return true;}
    virtual bool checkString(std::string s);
};


#endif //AUTOMATATHEORY_DFA_H
