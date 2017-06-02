//
// Created by jonathan on 11.05.17.
//

#include <chrono>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Bot.h"

Bot::~Bot() {
    delete dfa;
    for(auto p : commands){
        delete p.second;
    }
}

std::string Bot::getPath() {
    return JSONpath;
}

DFA* Bot::getDFA() {
    return dfa;
}

Fuzzy* Bot::getFuzzy() {
    return fuzzy;
}

void Bot::setPath(std::string path) {
    JSONpath = path;
}

void Bot::addCommand(Command *command) {
    commands[command->getName()] = command;
}

void Bot::setup(bool output){

    // Checking if the json has been updated
    bool upToDate = false; // TODO: Fix this

    // Setting up DFA and FuzzySearch
    std::vector<std::string> cmdNames;
    std::string cmdRegex;
    for(auto p : commands){
        cmdNames.push_back(p.second->getCommand());
        cmdRegex += p.second->getCommand() + "+";
    }
    cmdRegex.pop_back();

    // Build complete DFA
    buildDFA(cmdRegex, output);

    // Build Fuzzy
    Fuzzy* f = new Fuzzy();
    f->setupFuzzySearch(cmdNames, upToDate);
    this->fuzzy = f;
}

void Bot::buildDFA(std::string regex, bool FAout) {
    ENFA enfa;
    RegToeNFA converter;

    // Use algorithm implemented by D. Miroyan to convert regex to e-NFA
    converter.ConvertReTo_eNfa(regex, enfa);
    if(FAout) enfa.FAtoDot("BotENFA");

    // Use MSSC algorithm implemented by J. Meyer to convert e-NFA to DFA
    DFA* dfaRaw = new DFA;
    MSSC(enfa, *dfaRaw);

    // TODO: Use TFA algorithm implemented by S. Fenoll to optimise DFA

    // Assign DFA to bot
    dfa = dfaRaw;
    if(FAout) dfa->FAtoDot("BotDFA");

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

void Bot::checkforupdates() {

}

bool Bot::isEmpty(std::ifstream &file) {
    return file.peek() == std::ifstream::traits_type::eof();
}

std::vector<std::string> Bot::parseLink() {
    std::ifstream file;
    file.open("link/link.txt");
    std::string line;
    std::vector<std::string> result;
    while (getline(file,line)) {
        result.push_back(line);
    }
    file.close();
    std::ofstream ofs;
    ////clears content of the file
    ofs.open("link/link.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    return result;

}

void Bot::run(){
    std::ifstream file;
    bool go = true;
    int c = 0;
    std::vector<std::string> credentials;
    credentials = parseCredentials();
    std::string command = "python bot/fb_bot.py ";
    command += credentials[0];
    command += " " + credentials[1];
    command += " " + credentials[2];
    FILE *in;
    in = popen(command.c_str(), "r");

    while(go){
        file.open("link/link.txt");
        if(isEmpty(file)) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            file.close();
        }
        else{
            c++;
            if(c > 1000) go = false;
            std::string line;
            while(getline(file, line)){
                std::cout << line << "\n";
                if(line == "exit"){
                    std::cout << "Exiting\n";
                    // Clear the file (overwrite with empty file)
                    file.close();
                    file.open("link/link.txt", std::ofstream::out | std::ofstream::trunc);
                    file.close();
                    std::exit(0);
                }
            }

            file.close();
            // Clear the file (overwrite with empty file)
            file.open("link/link.txt", std::ofstream::out | std::ofstream::trunc);
            file.close();
        }
    }

    pclose(in);
}

std::vector<std::string> Bot::parseCredentials() {
    std::ifstream file;
    std::string line;
    std::vector<std::string> result;
    file.open("./bot/credentials.txt");
    while(!file.eof()){
        getline(file, line);
        result.push_back(line);

    }
    return result;
}
