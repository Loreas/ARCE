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