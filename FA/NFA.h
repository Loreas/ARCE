//
// Created by dawid on 13.03.17.
//

#ifndef AUTOMATATHEORY_NFA_H
#define AUTOMATATHEORY_NFA_H

#include "FA.h"

class NFA: public FA {
public:
    NFA();
    virtual bool Validate() {return true;}
};


#endif //AUTOMATATHEORY_NFA_H
