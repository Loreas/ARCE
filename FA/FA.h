//
// Created by dawid on 13.03.17.
//

#ifndef AUTOMATATHEORY_FA_H
#define AUTOMATATHEORY_FA_H

#include <iostream>
#include <set>
#include <map>
#include "State.h"
#include <tuple>

class FA {
private:
    State* startstate;
    std::string type;
    std::set<std::string> alphabet;
    std::set<State*> states;
    std::map<std::tuple<State*, std::string>, std::set<State*>> transitions;

public:
    FA() {
        this->type = "FA";
    }
    ~FA();
    virtual bool Validate() {return false;}
    virtual const std::string getEpsilon() const {return "";}
    friend std::ostream& operator<<(std::ostream& os, const FA& Fa);

    void setAlphabet(std::set<std::string> alphabet);
    void addState(State* state);
    void addTransition(State* stateFrom, std::string character, State* stateTo);

    const std::set<std::string>& getAlphabet() const {return this->alphabet;}
    const std::set<State*>& getStates() const {return this->states;}
    const std::map<std::tuple<State*, std::string>, std::set<State*>>& getTransitions() const {return this->transitions;}
    const std::string& getTypeFA() const {return this->type;}
    const State* getStartstate() const {return this->startstate;}

    void FAtoDot();
    void FAtoJSON();

protected:
    void setTypeFA(std::string type) {this->type = type;}
};


#endif //AUTOMATATHEORY_FA_H
