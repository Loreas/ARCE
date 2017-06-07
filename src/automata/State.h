//
// Created by dawid on 13.03.17.
//

#ifndef AUTOMATATHEORY_STATE_H
#define AUTOMATATHEORY_STATE_H

#include <algorithm>
#include <iostream>
#include <set>

class State {
private:
    std::set<std::string> name;
    bool starting;
    bool accepting;
public:
    State();
    State(std::string name, bool starting, bool accepting);
    State(std::set<std::string> name, bool starting, bool accepting);

    const std::string getName() const;
    bool isStarting() const;
    bool isAccepting() const;
    bool containsName(const std::string name) const;

    void setName(const std::set<std::string> &name);
    void setStarting(bool starting);
    void setAccepting(bool accepting);

    State& operator=(const State& rhs);
};

#endif //AUTOMATATHEORY_STATE_H
