//
// Created by jonathan on 19.05.17.
//

#ifndef ARCE_PARSER_H
#define ARCE_PARSER_H

#include "../json.hpp"
#include "../automata/FA.h"
#include "../automata/DFA.h"
#include "../automata/ENFA.h"
#include "Bot.h"
#include "Command.h"

class Parser {
private:

public:
    Parser() {}

    /**
     *  @brief Parse a json file, combine all 'regex' fields into one regex
     *  @param The filename of the file to be parsed
     *  @return The regex (string)
     */
     std::vector<std::string> parseCommands(std::string filename, Bot& bot, bool upToDate = false, bool output = false);


    /**
     *  @brief Parse a json file, create a DFA from the given file
     *  @param The filename of the file to be parsed
     *  @return The dfa (DFA)
     */
    DFA parseDFA(std::string filename);
};


#endif //ARCE_PARSER_H
