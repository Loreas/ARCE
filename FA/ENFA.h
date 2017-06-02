//
// Created by dawid on 13.03.17.
//

#ifndef AUTOMATATHEORY_ENFA_H
#define AUTOMATATHEORY_ENFA_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "FA.h"

class ENFA: public FA{
private:
    std::string epsilon;
public:
    ENFA();
    ENFA(const ENFA& otherENFA);
    virtual bool Validate() {return true;}
    virtual const std::string getEpsilon() const {return this->epsilon;}
    virtual void setEpsilon(std::string  epsilon);

    std::vector<const State*> singleEclose(const State* state) const;
    std::set<const State*> brokenEclose(const State* state) const;
    std::set<const State*> ecloseSubset(std::vector<const State*> subset) const;
    std::set<const State*> eclose(const State* state) const;

    ENFA& operator=(const ENFA& otherENFA);
};


#endif //AUTOMATATHEORY_ENFA_H
