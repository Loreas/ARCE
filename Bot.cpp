//
// Created by jonathan on 11.05.17.
//

#include "Bot.h"

std::string Bot::getPath() {
    return JSONpath;
}

void Bot::setPath(std::string path) {

}

void Bot::buildDFA(std::string regex) {
    eNFA ENFA;
    RegToeNFA converter;

    // Use algorithm implemented by D. Miroyan to convert regex to e-NFA
    converter.ConvertReTo_eNfa(regex, ENFA);

    // Use MSSC algorithm implemented by J. Meyer to convert e-NFA to DFA
    DFA dfaRaw;
    MSSC(ENFA, dfaRaw);

    // TODO: Use TFA algorithm implemented by S. Fenoll to optimise DFA

    // Assign DFA to bot
    dfa = dfaRaw;

}

void Bot::evaluateCommand() {

}

void Bot::runScript() {

}

void Bot::receiveMsg() {

}

void Bot::sendMsg() {

}
