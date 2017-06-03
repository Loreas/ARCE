//
// Created by jonathan on 01.06.17.
//

#include "Command.h"


Command::Command(std::string name, std::string desc, std::string cmd, std::string regex, std::string lang, std::string exec, std::string end) {
    this->name = name;
    this->desc = desc;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    command_regex = cmd;
    arg_regex = regex;
    language = lang;
    execute = exec;
    end_message = end;
    buildDFA();
}

Command::~Command() {
    delete this->arg_dfa;
}

DFA* Command::getDFA() const {
    return arg_dfa;
}

std::string Command::getName() const {
    return name;
}

std::string Command::getCommand() const {
    return command_regex;
}

std::string Command::getDesc() const {
    return desc;
}

std::string Command::getArgRegex() const {
    return arg_regex;
}

std::string Command::getExecute() const {
    return execute;
}

<<<<<<< HEAD
=======
std::string Command::getEndMessage() const {
    return end_message;
}

>>>>>>> 8514dbbdbdd2039f2cdc1077ae0d6edf4fa79862
std::string Command::getLanguage() const {
    return language;
}

void Command::buildDFA() {
    // Make ENFA out of the regex
    ENFA enfa;
    RegToeNFA converter;
    std::string regex = getArgRegex();
    converter.ConvertReTo_eNfa(regex, enfa);

    // Use MSSC to make DFA
    DFA* dfa_raw = new DFA();
    MSSC(enfa, *dfa_raw);

    // TODO: Use TFA on DFA

    // Set dfa
    this->arg_dfa = dfa_raw;

    // TODO: Delete dfa_raw!!
}