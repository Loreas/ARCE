//
// Created by jonathan on 19.05.17.
//

#include "Parser.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

std::string Parser::parseRegex(std::string filename) {
    std::ifstream is(filename);
    json j;
    std::string resultRegex = "";

    if(!is.is_open()){
        std::cout << "Failed to open file '" << filename << "'.\n";
        return "";
    }
    is >> j;

    for(auto& command : j["commands"]){
        std::string r = command["regex"];
        resultRegex += r + "+";
    }
    // Remove last '+'
    resultRegex.pop_back();

    return resultRegex;
}

DFA Parser::parseDFA(std::string filename) {
    std::ifstream is(filename);
    json j;
    is >> j;

    std::string type = j["type"];

    if(type == "DFA"){
        DFA dfa = DFA();

        std::set<std::string> alph = j["alphabet"];

        dfa.setAlphabet(alph);

        for(auto& s : j["states"]){
            std::string name = s["name"];
            name = "{" + name + "}";
            bool starting = s["starting"];
            bool accept = s["accepting"];

            const State* newState = new State(name, starting, accept);
            dfa.addState(newState);

            if (starting)
                dfa.setStartstate(newState);
        }


        // Temp create all transitions
        typedef std::tuple<std::string, std::string, std::string> transInfo;
        std::vector<transInfo> transitions;
        for(auto& t : j["transitions"]){
            std::string from = t["from"];
            from = "{" + from + "}";
            std::string to = t["to"];
            to = "{" + to + "}";
            std::string c = t["input"];

            transInfo trans = std::make_tuple(from, to, c);
            transitions.push_back(trans);
        }

        // Add transitions
        for(transInfo trans : transitions){
            std::string fromName = std::get<0>(trans);
            std::string toName = std::get<1>(trans);
            const State* from = nullptr;
            const State* to = nullptr;
            // TODO: temp fix
            for(const State* s : dfa.getStates()){
                if(s->getName() == fromName) from = s;
                if(s->getName() == toName) to = s;
                if(to != nullptr and from != nullptr) break;
            }
            std::string c = std::get<2>(trans);
            dfa.addTransition(from, c ,to);
        }

        return dfa;
    }
    else{
        std::cerr << "ERROR: File does not contain a DFA.\n";
    }


}