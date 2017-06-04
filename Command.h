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
    Command(std::string name, std::string desc, std::string cmd, std::string regex,
            std::string lang, std::string exec, std::string end, bool upToDate = false);
    ~Command();

    std::string getName() const;
    std::string getCommand() const;
    std::string getDesc() const;
    std::string getArgRegex() const;
    DFA* getDFA() const;
    std::string getLanguage() const;
    std::string getExecute() const;

    std::string getEndMessage() const;

    void buildDFA(bool upToDate = false, bool output = false);

};


#endif //ARCE_COMMAND_H
