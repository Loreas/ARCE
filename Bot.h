//
// Created by jonathan on 11.05.17.
//

#ifndef ARCE_BOT_H
#define ARCE_BOT_H

#include <string>
#include "FA/DFA.h"
#include "FA/ENFA.h"
#include "MSSC/MSSC.h"
#include "regex_to_enfa/regex_to_enfa.h"

class Bot {
private:
    std::string JSONpath;
    DFA* dfa;

public:

    /**
     * @brief Constructor
     */
    Bot() {};

    /**
     *
     * @return The path to the JSON file linking commands to their scripts
     */
    std::string getPath();

    DFA* getDFA();

    /**
     *
     * @param path The path to the JSON file linking commands to their scripts
     */
    void setPath(std::string path);

    /**
     * @brief Makes the given regex into an eNFA, which is then used to build a DFA that is saved by the bot
     * @param regex The regex to be used
     * @param FAout Boolean whether or not the ENFA and DFA should be outputted as dotfiles
     */
    void buildDFA(std::string regex, bool FAout = false);

    /**
     * @brief Checks if a command is valid and if so, executes it correctly
     */
    void parseCommand(std::string command);

    void runScript();

    void receiveMsg();

    void sendMsg();

};


#endif //ARCE_BOT_H
