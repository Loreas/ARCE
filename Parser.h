//
// Created by jonathan on 19.05.17.
//

#ifndef ARCE_PARSER_H
#define ARCE_PARSER_H

#include "json.hpp"

class Parser {
private:

public:
    Parser() {}

    /**
     *  @brief Parse a json files, combine all 'regex' fields into one regex
     *  @param The filename of the file to be parsed
     *  @return The regex (string)
     */
     std::string parseRegex(std::string filename);

};


#endif //ARCE_PARSER_H
