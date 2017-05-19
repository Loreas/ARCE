//
// Created by dawid on 14.03.17.
//

#include <stack>
#include <vector>
#include <set>
#include <tuple>
#include <string.h>

#include "../FA/eNFA.h"
#include "../FA/State.h"
#include "regex_to_enfa.h"
#include <sstream>

RegToeNFA::RegToeNFA() {
    this->stateCounter = 0;
}

void RegToeNFA::ConvertReTo_eNfa(std::string& regex, eNFA& automaton) {
    std::tuple<bool, std::set<std::string>, std::string> alphabet = ValidityAndAlphabetRegex(regex);

    if (std::get<0>(alphabet)) {
        automaton.setAlphabet(std::get<1>(alphabet));
        automaton.setEpsilon(std::get<2>(alphabet));
        this->buildeNFA(regex, automaton);
    }
    else {
        std::cout << "The given RE is invalid" << std::endl;
    }
}

std::tuple<State*, State*> RegToeNFA::buildeNFA(std::string& regex, eNFA& automaton, int currentPos) {
    State* firstState = NULL;
    State* lastState = NULL;
    State* currentFirstState = NULL;
    State* currentLastState = NULL;

    State* concatenationFirst = NULL;
    State* concatenationLast = NULL;

    std::tuple<State*, State*> currentFirstLastStates;

    int skipParenthesis = 0;
    bool regOR = false;
    bool regORHappened = false;
    bool epsilon = false;


    for (currentPos; currentPos < regex.length(); currentPos++) {
        if (regex[currentPos] == '/') {
            epsilon = true;
        }
        else if (regex[currentPos] == '(') {
            if (skipParenthesis != 0) {
                ++skipParenthesis;
                continue;
            }

            currentFirstLastStates = this->buildeNFA(regex, automaton, currentPos+1);
            currentFirstState = std::get<0>(currentFirstLastStates);
            currentLastState = std::get<1>(currentFirstLastStates);

            ++skipParenthesis;
        }
        else if (regex[currentPos] == ')') {
            if (skipParenthesis == 0)
                return std::make_tuple(firstState, lastState);

            --skipParenthesis;
            if (skipParenthesis != 0)
                continue;

            //Peek voor Operator*
            //Excecute operator*
            //Returns new currentFirst, currentLast
            if (this->PeekNextCharStar(regex, currentPos)) {
                currentFirstLastStates = this->OperatorStar(automaton, currentFirstState, currentLastState);
                currentFirstState = std::get<0>(currentFirstLastStates);
                currentLastState = std::get<1>(currentFirstLastStates);
            }

            if (firstState == NULL)
                firstState = currentFirstState;
            if (lastState == NULL)
                lastState = currentLastState;

            //Concatenatie
            if (!regOR && (firstState != currentFirstState) && (lastState != currentLastState)) {
                lastState->setAccepting(false);
                currentFirstState->setStarting(false);
                automaton.addTransition(lastState, automaton.getEpsilon(), currentFirstState);
                lastState = currentLastState;
            }

            //Do operator + if necessecary, returns new first and last state for current parentheses
            //First to last is now the complete regex within the current parentheses
            currentFirstLastStates = this->OperatorPlus(automaton, regORHappened, regOR, firstState, lastState, currentFirstState, currentLastState);
            firstState = std::get<0>(currentFirstLastStates);
            lastState = std::get<1>(currentFirstLastStates);
        }
        else if (regex[currentPos] == '+') {
            if (skipParenthesis != 0)
                continue;
            regOR = true;
        }
        else if (regex[currentPos] == '*') {
            if (skipParenthesis != 0)
                continue;
            continue;
        }
            //current character is element of alphabet
        else if ((automaton.getAlphabet().find(std::string(1, regex[currentPos])) != automaton.getAlphabet().end()) or epsilon) {
            int currentChar;
            if (epsilon) {
                epsilon = false;
                automaton.setEpsilon(std::string(1, regex[currentPos]));
                currentChar = (int)(*automaton.getEpsilon().c_str());
            }
            else
                currentChar = regex[currentPos];

            if (skipParenthesis != 0)
                continue;
            //Create the 2 states needed for a single character
            currentFirstState = new State(std::to_string(this->stateCounter++), true, false);
            automaton.addState(currentFirstState);
            currentLastState = new State(std::to_string(this->stateCounter++), false, true);
            automaton.addState(currentLastState);
            //Add a transition between the 2 states
            automaton.addTransition(currentFirstState, std::string(1, currentChar), currentLastState);

            //Peek voor Operator*
            //Excecute operator*
            //Returns new currentFirst, currentLast
            if (this->PeekNextCharStar(regex, currentPos)) {
                currentFirstLastStates = this->OperatorStar(automaton, currentFirstState, currentLastState);
                currentFirstState = std::get<0>(currentFirstLastStates);
                currentLastState = std::get<1>(currentFirstLastStates);
            }

            //No other state present in current frame
            //CurrentFirst and currentLast are first and last
            //Can't do concatenation or + with other states
            if ((firstState == NULL) && (lastState == NULL)) {
                firstState = currentFirstState;
                lastState = currentLastState;
            }
            else {
                if (regOR) {
                    //Do operator + if necessecary, returns new first and last state for current parentheses
                    currentFirstLastStates = this->OperatorPlus(automaton, regORHappened, regOR, firstState, lastState,
                                                            currentFirstState, currentLastState);
                    firstState = std::get<0>(currentFirstLastStates);
                    lastState = std::get<1>(currentFirstLastStates);

                }
                //Not a +, so a concatenation with last
                else {
                    automaton.addTransition(lastState, automaton.getEpsilon(), currentFirstState);
                    lastState->setAccepting(false);
                    currentFirstState->setStarting(false);
                    lastState = currentLastState;
                }
            }
        }
    }

    // Set the first state of the FA as the starting state
    automaton.setStartstate(firstState);
}

std::tuple<bool, std::set<std::string>, std::string> RegToeNFA::ValidityAndAlphabetRegex(std::string &regex) {
    int parenthesis = 0;
    bool valid = true;
    std::set<std::string> alphabet;
    std::string lastCharacter = "";
    std::string epsilon = "ε";
    bool isEpsilon = false;

    for (const char character: regex) {
        std::string strChar = std::string(1, character);

        if (isEpsilon) {
            epsilon = strChar;
            isEpsilon = false;
        }
        else if (strChar == "(")
            parenthesis++;
        else if (strChar == ")") {
            if ((lastCharacter == "+") or (parenthesis <= 0))  {
                valid = false;
                break;
            }
            parenthesis--;
        }
        else if (strChar == "*")  {
            if ((lastCharacter == "+") || (lastCharacter == "(")) {
                valid = false;
                break;
            }
            ;
        }
        else if (strChar == "+") {
            if (lastCharacter == "(") {
                valid = false;
                break;
            }
            ;
        }
        else if (strChar == "/") {
            isEpsilon = true;
        }
        else alphabet.insert(strChar);

        lastCharacter = strChar;
    }

    if (valid && (parenthesis == 0))
        return std::make_tuple(valid, alphabet, epsilon);
    else
        return std::make_tuple(false, alphabet, "");
}

bool RegToeNFA::PeekNextCharStar(std::string regex, int currentPos) {
    if (currentPos+1 < regex.length()) {
        if (regex[currentPos+1] == '*')
            return true;
    }
    return false;
}



std::tuple<State*, State*> RegToeNFA::OperatorPlus(eNFA& automaton, bool& regORHappened, bool& regOR,
                                                   State* firstState, State* lastState,
                                                   State* currentFirstState, State* currentLastState) {
    //If this is the first + in the current parentheses


    //Create new firststate, connect previous firststate to that new state
    //Create new laststate, connect previous laststate to that new state
    //Connect first and last to currentStates
    if (regOR /*&& !regORHappened*/) {
        //Store first and last state in temp
        //Change properties to false of those states
        State* previousfirst = firstState;
        State* previouslast = lastState;
        previousfirst->setStarting(false);
        previouslast->setAccepting(false);
        //Change properties to false of current first and last states
        currentFirstState->setStarting(false);
        currentLastState->setAccepting(false);

        //Create new first and last states
        //Add these to the automaton with transitions to previous first and last states
        firstState = new State(std::to_string(this->stateCounter++), true, false);
        lastState = new State(std::to_string(this->stateCounter++), false, true);
        automaton.addState(firstState);
        automaton.addState(lastState);

        automaton.addTransition(firstState, automaton.getEpsilon(), previousfirst);
        automaton.addTransition(previouslast, automaton.getEpsilon(), lastState);
        automaton.addTransition(firstState, automaton.getEpsilon(), currentFirstState);
        automaton.addTransition(currentLastState, automaton.getEpsilon(), lastState);

        //regORHappened moet nog gekeken worden hoe de + moet werken
        regORHappened = true;

        regOR = false;
    }

    return std::make_tuple(firstState, lastState);
};

std::tuple<State*, State*> RegToeNFA::OperatorStar(eNFA& automaton, State* currentFirstState,
                                                   State* currentLastState) {

    //2 new states to be able to skip currentFirst -> currentLast
    State* firstState = new State(std::to_string(this->stateCounter++), true, false);
    State* lastState = new State(std::to_string(this->stateCounter++), false, true);
    automaton.addState(firstState);
    automaton.addState(lastState);

    automaton.addTransition(firstState,automaton.getEpsilon(), currentFirstState);
    automaton.addTransition(currentLastState, automaton.getEpsilon(), lastState);
    //Loop back
    automaton.addTransition(currentLastState, automaton.getEpsilon(), currentFirstState);
    //Skip transition
    automaton.addTransition(firstState, automaton.getEpsilon(), lastState);

    currentFirstState->setStarting(false);
    currentLastState->setAccepting(false);

    return std::make_tuple(firstState, lastState);
};