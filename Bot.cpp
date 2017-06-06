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
#include "Parser.h"

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

void Bot::setup(bool upToDate, bool output){

    if(output) std::cout << "Setting up bot...\n";

    std::vector<std::string> cmdNames;
    std::string cmdRegex;
    for (auto p : commands) {
        cmdNames.push_back(p.second->getCommand());
        cmdRegex += p.second->getCommand() + "+";
    }
    cmdRegex.pop_back();
    if(upToDate){
        // Read the bot DFA from json
        Parser parser;
        DFA* dfa = new DFA;
        *dfa = parser.parseDFA("./.config/botDFA.json");
        this->dfa = dfa;

        for (auto p : commands)
            cmdNames.push_back(p.second->getCommand());
    }
    else {
        // Build complete DFA
        buildDFA(cmdRegex, output);

        // Save DFA to json
        this->dfa->FAtoJSON("./.config/botDFA");
    }

    // Build Fuzzy
    Fuzzy* f = new Fuzzy();
    f->setupFuzzySearch(cmdNames, upToDate);
    this->fuzzy = f;

    if(output) std::cout << "Setup complete.\n";
}

void Bot::buildDFA(std::string regex, bool FAout) {
    ENFA enfa;
    RegToeNFA converter;

    // Use algorithm implemented by D. Miroyan to convert regex to e-NFA
    converter.ConvertReTo_eNfa(regex, enfa);
    if(FAout) enfa.FAtoDot("BotENFA");

    // Use MSSC algorithm implemented by J. Meyer to convert e-NFA to DFA
    DFA dfaRaw;
    MSSC(enfa, dfaRaw);

    // Use TFA algorithm implemented by S. Fenoll to optimise DFA
    DFA* dfa = new DFA;
    // TODO: tfa(dfaRaw, dfa);
    *dfa = dfaRaw;

    // Assign DFA to bot
    this->dfa = dfa;
    if(FAout) dfa->FAtoDot("BotDFA");

}

bool Bot::checkCommand(std::string& command) const {
    return dfa->checkString(command);
}

std::string Bot::executeCommand(std::vector<std::string>& command) {
    Command* com = commands.at(command.at(0)); // The vector contains: {command, arg1, arg2...}
    std::string arguments = "";
    bool errorFound = false;
    int missingArguments = 0;
    std::string errorMessage = "";

    // Check if correct amount of arguments given
    if (com->getAmmountArgs() == -1) {
    }
    else if (command.size()-1 < com->getAmmountArgs()) {
        errorMessage += "Command \"" + command[0] + "\" is missing " + std::to_string(std::abs((int)(com->getAmmountArgs() - command.size() - 1))) + " arguments\n";
        errorMessage += com->getName() + " command has the following form: " + com->getForm() + "\n";
        return errorMessage;
    }
    else if (command.size()-1 > com->getAmmountArgs()) {
        errorMessage += "Command \"" + command[0] + "\" has " + std::to_string((int)(command.size() - 1 - com->getAmmountArgs())) + " to many arguments\n";
        errorMessage += com->getName() + " command has the following form: " + com->getForm() + "\n";
        return errorMessage;
    }
    else {
        for (int i = 1; i <= com->getAmmountArgs(); i++) {
            if (!com->getDFA()->checkString(command[i])) {
                errorMessage +=
                    "Argument " + std::to_string(i) + ": \"" + command[i] + "\" is not of the right form.\n";
                errorFound = true;
            }
            arguments += " " + command[i];
        }
    }
    if (errorFound) return errorMessage;

    std::string exec = com->getExecute() + arguments;
    system(exec.c_str());
    return "";
}

bool Bot::isEmpty(std::ifstream &file) {
    return file.peek() == std::ifstream::traits_type::eof();
}

std::vector<std::string> Bot::parseLink(bool output) {
    std::ifstream file;
    file.open("link/link.txt");
    std::string line;
    std::vector<std::string> result;
    while (getline(file,line)) {
        if(output) std::cout << line << std::endl;
        result.push_back(line);
    }
    file.close();
    std::ofstream ofs;
    ////clears content of the file
    ofs.open("link/link.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    return result;

}

void Bot::run(bool CMIoutput){
    if (CMIoutput) std::cout << "Starting ARCE\n";

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
    if (CMIoutput) std::cout << "Subprocess started.\n";
    std::string output;

    // Main loop
    while(go){
        file.open("link/link.txt");
        if(isEmpty(file)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            file.close();
            continue;
        }
        else{
             std::vector<std::string> commands = parseLink(true);
             for(std::string c : commands) {
                 // Transform string into lowercase
                 std::transform(c.begin(), c.end(), c.begin(), ::tolower);
                 // Check if command is valid
                 std::stringstream ss(c);
                 std::string word;
                 std::vector<std::string> words;
                 while(ss >> word){
                     words.push_back(word);
                 }
                 if(!checkCommand(words[0])) {
                     std::vector<std::string> typos = fuzzy->fuzzy(words[0]);
                     if (typos.size() != 0) {
                         output = "Did you mean:\n";
                         for (auto typo : typos) {
                             output += typo + "\n";
                         }
                     }
                     else {
                         output = "I don't know the command '" + words[0] + "'.\n";
                     }
                 }
                 else {
                    if (words[0] == "exit"){
                        output = "";
                        go = false;
                    }
                    else if (words[0] == "adduser") {
                        if (words.size() == 3)
                            output = c + "\n";
                        else
                           output = "addUser command expects following arguments: addUser name surname\n";
                    }
                    else if (words[0] == "removeuser") {
                        if (words.size() == 3)
                            output = c + "\n";
                        else
                            output = "removeUser command expects following arguments: removeUser name surname\n";
                    }
                    else if (words[0] == "log"){
                        if (words.size() == 2)
                            output = c + "\n";
                        else
                            output = "log command expects following arguments: log start/stop/date";
                    }
                    else {
                        output = executeCommand(words);
                    }
                 }
                 if (CMIoutput) std::cout << output;
                 std::ofstream link("link/linkToPython.txt", std::ofstream::app);
                 link << output;
                 link.close();
             }
        }

            file.close();
            // Clear the file (overwrite with empty file)
            file.open("link/link.txt", std::ofstream::out | std::ofstream::trunc);
            file.close();
    }

    file.close();
    // Clear the file (overwrite with empty file)
    file.open("link/linkToPython.txt", std::ofstream::out | std::ofstream::trunc);
    file.close();

    pclose(in);
    if (CMIoutput) std::cout << "Exiting main loop\n";
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
