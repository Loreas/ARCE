//
// Created by sergio on 11/05/17.
//
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <fstream>
#include "json.hpp"
#include "bot_ARCE/Bot.h"
#include "bot_ARCE/Parser.h"

void bulkTest(std::string fileName, DFA& dfa){
    std::fstream file(fileName);
    if(!file.is_open()) return;

    std::string line;
    while(getline(file, line)){
        if(dfa.checkString(line))
            std::cout << "Accepted string: " << line << std::endl;
        else
            std::cout << "Rejected string: " << line << std::endl;
    }

}

void setupARCE(std::string configFile, bool& upToDate, bool& output, std::string& command_path){
    using json = nlohmann::json;

    // Setting up directory and files
    system("mkdir ./config");
    system("mkdir ./link");
    system("mkdir ./log");
    std::ofstream("./link/link.txt", std::ofstream::out | std::ofstream::trunc);
    std::ofstream("./link/linkToPython.txt", std::ofstream::out | std::ofstream::trunc);

    std::ifstream file(configFile);
    json config;
    file >> config;

    // Checking whether if update is needed
    std::string cmd_file_path = config["commands_file"];
    if (config["ignore_update"]) upToDate = true;
    else if (config["force_update"]) upToDate = false;
    else{
        // Getting the last modified time from the config file
        std::time_t file_mod_time = config["last_updated"];
        // Getting last modified time from OS (Linux only)
        struct stat fbuf;
        stat(cmd_file_path.c_str(), &fbuf);
        time_t OS_mod_time = fbuf.st_mtime;

        /*
         * DO NOT DELETE: This code is used to convert "Y-M-D-h-m-s" timestamp into time_t
         * (which is a long that represents seconds in UNIX time. Should the config file ever get fucked,
         * use this code to manually fix it. Otherwise, don't touch anything. -kusjes, Jona
        std::tm tm;
        std::istringstream ss(lastUpdate);
        ss >> std::get_time(&tm, "%Y-%m-%d-%H-%M-%S");
        std::time_t cur_mod_time = mktime(&tm);
         */

        upToDate = (OS_mod_time <= file_mod_time);

        if(!upToDate){
            config["last_updated"] = OS_mod_time;
        }
    }

    output = config["output"];
    command_path = cmd_file_path;

    // Re-writing the file
    file.close();
    std::ofstream outfile(configFile);
    outfile << std::setw(4) << config << std::endl;
}

int main(unsigned int argc, char* argv[]){

    bool upToDate;
    bool output = true;
    std::string config_file = "ARCE_config.json";
    std::string cmd_file;

    setupARCE(config_file, upToDate, output, cmd_file);

    if (output){
        std::cout << "Starting up ARCE v1.0...\n" << "Output enabled.\n"
                  << "Files up to date: " << upToDate << std::endl
                  << "Reading commands from: " << cmd_file << std::endl;
    }

    Bot bot;
    Parser parser;
    parser.parseCommands(cmd_file, bot, upToDate, output);
    bot.setup(upToDate, output);

    bot.run(output);

    if (output) std::cout << "Exiting ARCE.\n" << std::flush;

    return 0;
}
