//
// Created by jonathan on 01.06.17.
//

#ifndef ARCE_COMMAND_H
#define ARCE_COMMAND_H
#include <algorithm>
#include "./FA/DFA.h"
#include "./FA/ENFA.h"
#include "./TFA/tfa.h"
#include "MSSC/MSSC.h"
#include "regex_to_enfa/regex_to_enfa.h"


class Command {
private:
    std::string name;
    std::string command_regex;
    std::string desc;
    std::string arg_regex;
    DFA* arg_dfa;
    std::string language;
    std::string execute;
    std::string end_message;

public:
    Command() {};
    Command(std::string name, std::string desc, std::string cmd, std::string arg_regex, std::string lang, std::string exec);
    ~Command();

    std::string getName() const;
    std::string getCommand() const;
    std::string getDesc() const;
    std::string getArgRegex() const;
    DFA* getDFA() const;
    std::string getLanguage() const;
    std::string getExecute() const;
<<<<<<< HEAD
=======
    std::string getEndMessage() const;
>>>>>>> 8514dbbdbdd2039f2cdc1077ae0d6edf4fa79862

    void buildDFA();

};


#endif //ARCE_COMMAND_H
