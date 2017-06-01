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
    const State* startstate;
    std::string type;
    std::set<std::string> alphabet;
    std::set<const State*> states;
    std::map<std::tuple<const State*, std::string>, std::set<const State*>> transitions;

public:
    FA() {
        this->type = "FA";
    }
    FA(const FA& otherFA);
    ~FA();
    virtual bool Validate() {return false;}
    virtual const std::string getEpsilon() const {return "";}
    friend std::ostream& operator<<(std::ostream& os, const FA& Fa);

    void setAlphabet(std::set<std::string> alphabet);
    void setStartstate(const State *state);
    void addState(const State *state);
    void addTransition(const State *stateFrom, std::string character, const State *stateTo);

    const std::set<std::string>& getAlphabet() const {return this->alphabet;}
    const std::set<const State*>& getStates() const {return this->states;}
    const std::map<std::tuple<const State*, std::string>, std::set<const State*>>& getTransitions() const {return this->transitions;}
    const std::string& getTypeFA() const {return this->type;}
    const State* getStartstate() const {return this->startstate;}

    void FAtoDot(std::string name = "") const;
    void FAtoJSON();

    virtual bool checkString(std::string s) {return false;}

    virtual FA& operator=(const FA& otherFA);
protected:
    void setTypeFA(std::string type) {this->type = type;}
};


#endif //AUTOMATATHEORY_FA_H
