//
// Created by jonathan on 11.05.17.
//

#ifndef ARCE_BOT_H
#define ARCE_BOT_H

#include <string>
#include "FA/DFA.h"
#include "FA/eNFA.h"
#include "MSSC/MSSC.h"
#include "regex_to_enfa/regex_to_enfa.h"

class Bot {
private:
    std::string JSONpath;
    DFA dfa;

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

    /**
     *
     * @param path The path to the JSON file linking commands to their scripts
     */
    void setPath(std::string path);

    /**
     * @brief Makes the given regex into an eNFA, which is then used to build a DFA that is saved by the bot
     * @param regex
     */
    void buildDFA(std::string regex);

    /**
     * @brief Check if a string is accepted by the DFA
     */
    void evaluateCommand();

    void runScript();

    void receiveMsg();

    void sendMsg();

};


#endif //ARCE_BOT_H
