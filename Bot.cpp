//
// Created by jonathan on 11.05.17.
//

#include <chrono>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
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
    commands[command->getCommand()] = command;
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

    // Use TFA algorithm implemented by S. Fenoll to optimise DFA
    DFA* dfa = new DFA;
    //tfa(dfaRaw, dfa);

    // Assign DFA to bot
    this->dfa = dfaRaw;
    if(FAout) dfaRaw->FAtoDot("BotDFA");

}

bool Bot::checkCommand(std::string& command) const {
    return dfa->checkString(command);
}

std::string Bot::executeCommand(std::vector<std::string>& command) {
    Command* com = commands.at(command.at(0)); // The vector contains: {command, arg1, arg2...}
    std::string arguments;
    for (int i = 1; i < command.size(); i++) {
        if (!com->getDFA()->checkString(command[i])) {
            return "Argument \"" + command[i] + "\" is not of the right form.\n";
        }
        arguments += " " + command[i];
    }

    std::string exec = com->getExecute() + arguments;
    system(exec.c_str());
    return com->getEndMessage();

}

void Bot::runScript() {

}

void Bot::receiveMsg() {

}

void Bot::sendMsg() {

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
    std::vector<std::string> credentials;
    credentials = parseCredentials();

    // Starting up FBChat as a subprocess
    std::string command = "python bot/fb_bot.py ";
    command += credentials[0];
    command += " " + credentials[1];
    command += " " + credentials[2];
    FILE *in;
    in = popen(command.c_str(), "r");
    std::string output;

    // Main loop
    while(go){
        file.open("link/link.txt");
        if(isEmpty(file)) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            file.close();
        }
        else{
             std::vector<std::string> commands = parseLink();
             for(std::string c : commands) {
                 // Check if command is valid
                 std::stringstream ss(c);
                 std::string word;
                 std::vector<std::string> words;
                 while(ss >> word){
                     words.push_back(word);
                 }
                 if(!checkCommand(words[0])) {
                     std::vector<std::string> typos = fuzzy->fuzzy(c);
                     if (typos.size() != 0) {
                         output = "Did you mean:\n";
                         for (auto typo : typos) {
                             output += typo + "\n";
                         }
                     }
                     else {
                         output = "Cannot find command.\n";
                     }
                 }
                 else {
                    if (words[0] == "adduser") {
                        if (c.size() == 3)
                            output = c + "\n";
                        else
                            output = "addUser command expects following arguments: addUser name surname\n";
                    }
                    else if (words[0] == "removeuser") {
                        if (c.size() == 3)
                            output = c + "\n";
                        else
                            output = "removeUser command expects following arguments: removeUser name surname\n";
                    }
                    else if (words[0] == "log"){
                        if (c.size() == 2)
                            output = c;
                        else
                            output = "log command expects following arguments: log start/stop/date";
                    }
                    else {
                        output = executeCommand(words) + "\n";
                    }
                 }
                 std::ofstream link("link/linkToPython.txt");
                 link << output;
                 link.close();
             }
        }

            file.close();
            // Clear the file (overwrite with empty file)
            file.open("link/link.txt", std::ofstream::out | std::ofstream::trunc);
            file.close();
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
