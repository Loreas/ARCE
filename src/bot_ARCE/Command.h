//
// Created by jonathan on 01.06.17.
//

#ifndef ARCE_COMMAND_H
#define ARCE_COMMAND_H
#include <algorithm>
#include "../automata/DFA.h"
#include "../automata/ENFA.h"
#include "../algos/tfa.h"
#include "../algos/MSSC.h"
#include "../algos/regex_to_enfa.h"


class Command {
private:
    std::string name;
    std::string command_regex;
    std::string desc;
    std::string arg_regex;
    int ammount_args;
    std::string form;
    DFA* arg_dfa;
    std::string language;
    std::string execute;

public:
    Command() {};
    Command(std::string name, std::string desc, std::string cmd, std::string regex, int ammountArgs,
                std::string commandForm, std::string lang, std::string exec, bool upToDate);
    ~Command();

    std::string getName() const;
    std::string getCommand() const;
    std::string getDesc() const;
    std::string getArgRegex() const;
    int getAmmountArgs() const;
    std::string getForm() const;
    DFA* getDFA() const;
    std::string getLanguage() const;
    std::string getExecute() const;
    void buildDFA(bool upToDate = false, bool output = false);

};


#endif //ARCE_COMMAND_H
