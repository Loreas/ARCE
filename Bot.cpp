//
// Created by jonathan on 11.05.17.
//

#include <chrono>
#include <thread>
#include <stdlib.h>
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
    command += credentials[1];
    command += credentials[2];
    std::system(command.c_str());


    while(go){
        file.open("link/link.txt");
        if(isEmpty(file)) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            file.close();
        }
        else{
            file.close();
            c++;
            if(c > 1000000) go = false;
            std::string line;
            while(getline(file, line)){
                std::cout << line << std::endl;
            }
        }
    }
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
