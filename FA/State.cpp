//
// Created by dawid on 13.03.17.
//

#include "State.h"

State::State() {
    this->starting = false;
    this->accepting = false;
}

State::State(std::string name, bool starting, bool accepting) {
    this->name.insert(name);
    this->starting = starting;
    this->accepting = accepting;
}

State::State(std::set<std::string> name, bool starting, bool accepting) {
    this->name = name;
    this->starting = starting;
    this->accepting = accepting;
}

const std::string State::getName() const {
    std::string fullName = "";
    for (std::string name: this->name) {
        fullName += name;
        if (this->name.find(name) != (--this->name.end()))
            fullName += ", ";
    }

    return fullName;
}

bool State::isStarting() const {
    return starting;
}

bool State::isAccepting() const {
    return accepting;
}

bool State::containsName(const std::string name) const{
    if (this->name.find(name) != this->name.end())
        return true;
    else return false;
}

void State::setName(const std::set<std::string> &name) {
    this->name = name;
}

void State::setStarting(bool starting) {
    this->starting = starting;
}

void State::setAccepting(bool accepting) {
    this->accepting = accepting;
}

State& State::operator=(const State& rhs){
    this->starting = rhs.starting;
    this->accepting = rhs.accepting;
    this->name = rhs.name;

    return (*this);
}