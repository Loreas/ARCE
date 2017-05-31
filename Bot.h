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
#include <fstream>

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

    /**
     * @brief Checks for updates in the link.txt after x amount of seconds.
     *
     */

    void checkforupdates();

    /**
     * @brief Checks if the file is empty.
     * @param ifstream file.
     * @return bool whether or not the file file is empty.
     */
    bool isEmpty(std::ifstream &file);

    /**
     * @brief Parses every command in the txt file.
     * @return vector van commands
     */
    std::vector<std::string> parseLink();

    /**
     *  @brief The 'main loop' for the bot (will constantly check for messages and parse them
     */
     void run();


};


#endif //ARCE_BOT_H