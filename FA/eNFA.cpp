//
// Created by dawid on 13.03.17.
//

#include "eNFA.h"

eNFA::eNFA() {
    this->epsilon = "ε";
    this->setTypeFA("eNFA");
}

void eNFA::setEpsilon(std::string epsilon) {
    if (this->getEpsilon() == "ε")
        this->epsilon = epsilon;
}

std::vector<const State*> eNFA::singleEclose(const State* state) const{
    std::vector<const State*> result = {state};
    // Find all epsilon transitions for this state
    for(auto p : this->getTransitions()){
        if(std::get<0>(p.first) == state and std::get<1>(p.first) == this->getEpsilon()){
            result.insert(result.end(), p.second.begin(), p.second.end());
        }
    }
    return result;
}

std::set<const State*> eNFA::eclose(const State* state) const {
    std::vector<const State*> epsTrans = singleEclose(state);
    std::set<const State*> result;

    for(const State* s : epsTrans){
        std::vector<const State*> temp = singleEclose(s);
        if(temp.size() > 1){
            for(const State* st : temp){
                if(find(epsTrans.begin(), epsTrans.end(), st) == epsTrans.end()){
                    epsTrans.push_back(st);
                }
            }
        }
    }

    result.insert(epsTrans.begin(), epsTrans.end());
    return result;
}

std::set<const State*> eNFA::ecloseSubset(std::vector<const State*> subset) const {
    std::set<const State*> result;
    for(const State* s : subset){
        std::set<const State*> ecl = eclose(s);
        result.insert(ecl.begin(), ecl.end());
    }
    return result;
}