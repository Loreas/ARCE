//
// Created by jonathan on 11.05.17.
//

#ifndef ARCE_BOT_H
#define ARCE_BOT_H

#include <string>

class Bot {
private:
    std::string JSONpath;

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

    void evaluateCommand();

    void runScript();

    void receiveMsg();

    void sendMsg();

};


#endif //ARCE_BOT_H
