//
// Created by jonathan on 01.06.17.
//

#ifndef ARCE_COMMAND_H
#define ARCE_COMMAND_H

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

    void buildDFA();

};


#endif //ARCE_COMMAND_H
