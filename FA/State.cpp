//
// Created by dawid on 13.03.17.
//

#include <algorithm>
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

    std::set<std::string> thisName;
    std::set<std::string> otherName;

    std::string number = "";
    for (const char& ch: this->getName()) {
        if (ch == '{' or ch == ' ') continue;
        else if (ch == '}' or ch == ',') {
            if (number != "") {
                thisName.insert(number);
                number = "";
            }
        } else number += ch;
    }
    if (number != "") thisName.insert(number);

    number = "";
    for (const char& ch: name) {
        if (ch == '{' or ch == ' ') continue;
        else if (ch == '}' or ch == ',') {
            if (number != "") {
                otherName.insert(number);
                number = "";
            }
        } else number += ch;
    }
    if (number != "") otherName.insert(number);

    std::set<std::string> intersection;
    std::set_intersection(thisName.begin(), thisName.end(), otherName.begin(), otherName.end(),
                          std::inserter(intersection, intersection.begin()));

    bool returnValue = (otherName.size() == intersection.size());
    return returnValue;
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