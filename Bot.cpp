//
// Created by jonathan on 11.05.17.
//

#include "Bot.h"

std::string Bot::getPath() {
    return JSONpath;
}

DFA* Bot::getDFA() {
    return dfa;
}

void Bot::setPath(std::string path) {
    JSONpath = path;
}

void Bot::buildDFA(std::string regex, bool FAout) {
    ENFA enfa;
    RegToeNFA converter;

    // Use algorithm implemented by D. Miroyan to convert regex to e-NFA
    converter.ConvertReTo_eNfa(regex, enfa);
    if(FAout) enfa.FAtoDot();

    // Use MSSC algorithm implemented by J. Meyer to convert e-NFA to DFA
    DFA* dfaRaw = new DFA;
    MSSC(enfa, *dfaRaw);

    // TODO: Use TFA algorithm implemented by S. Fenoll to optimise DFA

    // Assign DFA to bot
    dfa = dfaRaw;
    if(FAout) dfa->FAtoDot();

    // delete dfaRaw;
}

void Bot::parseCommand(std::string command) {
    // First, check if the command is valid
    if(!dfa->checkString(command)) return;

}

void Bot::runScript() {

}

void Bot::receiveMsg() {

}

void Bot::sendMsg() {

}
