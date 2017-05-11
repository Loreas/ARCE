//
// Created by dawid on 13.03.17.
//

#ifndef AUTOMATATHEORY_ENFA_H
#define AUTOMATATHEORY_ENFA_H

#include <iostream>
#include "FA.h"

class eNFA: public FA{
private:
    std::string epsilon;
public:
    eNFA();
    virtual bool Validate() {return true;}
    virtual const std::string getEpsilon() const {return this->epsilon;}
};


#endif //AUTOMATATHEORY_ENFA_H
