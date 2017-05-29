//
// Created by dawid on 14.03.17.
//

#include <stack>
#include <vector>
#include <set>
#include <tuple>
#include <string.h>

#include "../FA/ENFA.h"
#include "../FA/State.h"
#include "regex_to_enfa.h"
#include <sstream>

RegToeNFA::RegToeNFA() {
    this->stateCounter = 0;
}

void RegToeNFA::ConvertReTo_eNfa(std::string& regex, ENFA& automaton) {
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

std::tuple<State*, State*> RegToeNFA::buildeNFA(std::string &regex, ENFA &automaton, int currentPos) {
    State* firstState = NULL;
    State* lastState = NULL;

    State* currentFirstState = NULL;
    State* currentLastState = NULL;

    State* previousFirstState = NULL;
    State* previousLastState = NULL;

    std::tuple<State*, State*> currentFirstLastStates;

    int skipParenthesis = 0;
    int posParenthesis = 0;
    bool regOR = false;
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

            posParenthesis = currentPos;
            currentFirstLastStates = this->buildeNFA(regex, automaton, currentPos + 1);
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

            // No other state present in current frame
            // CurrentFirst and currentLast are first and last
            // Can't do checkConcatenationPrevious or + with other states
            if ((firstState == NULL) && (lastState == NULL)) {
                firstState = currentFirstState;
                lastState = currentLastState;
            }
            else {
                // Check if concatenation with previous
                if (this->checkConcatenationPrevious(automaton, regex, posParenthesis)) {
                    automaton.addTransition(previousLastState, automaton.getEpsilon(), currentFirstState);
                    previousLastState->setAccepting(false);
                    currentFirstState->setStarting(false);
                    currentFirstState = previousFirstState;
                }
                // Check if concatenation with next, continue if so
                if (this->checkConcatenationNext(automaton, regex, currentPos)) {
                    previousFirstState = currentFirstState;
                    previousLastState = currentLastState;
                }
                    // No concatenation with next, so operator + can be performed
                else if (regOR) {
                    currentFirstLastStates = this->OperatorPlus(automaton, regOR, firstState, lastState,
                                                                currentFirstState, currentLastState);
                    firstState = std::get<0>(currentFirstLastStates);
                    lastState = std::get<1>(currentFirstLastStates);

                    regOR = false;
                }
                else {
                    firstState = currentFirstState;
                    lastState = currentLastState;
                }
            }
            previousFirstState = currentFirstState;
            previousLastState = currentLastState;
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

            // Create the 2 states needed for a single character
            currentFirstState = new State(std::to_string(this->stateCounter++), true, false);
            automaton.addState(currentFirstState);
            currentLastState = new State(std::to_string(this->stateCounter++), false, true);
            automaton.addState(currentLastState);
            //Add a transition between the 2 states
            automaton.addTransition(currentFirstState, std::string(1, currentChar), currentLastState);

            // Peek voor Operator*
            // Excecute operator*
            // Returns new currentFirst, currentLast
            if (this->PeekNextCharStar(regex, currentPos)) {
                currentFirstLastStates = this->OperatorStar(automaton, currentFirstState, currentLastState);
                currentFirstState = std::get<0>(currentFirstLastStates);
                currentLastState = std::get<1>(currentFirstLastStates);
            }

            // No other state present in current frame
            // CurrentFirst and currentLast are first and last
            // Can't do checkConcatenationPrevious or + with other states
            if ((firstState == NULL) && (lastState == NULL)) {
                firstState = currentFirstState;
                lastState = currentLastState;
            }
            else {
                // Check if concatenation with previous
                if (this->checkConcatenationPrevious(automaton, regex, currentPos)) {
                    automaton.addTransition(previousLastState, automaton.getEpsilon(), currentFirstState);
                    previousLastState->setAccepting(false);
                    currentFirstState->setStarting(false);
                    currentFirstState = previousFirstState;
                }
                    // Check if concatenation with next, continue if so
                if (this->checkConcatenationNext(automaton, regex, currentPos)) {
                    previousFirstState = currentFirstState;
                    previousLastState = currentLastState;
                }
                // No concatenation with next, so operator + can be performed
                else if (regOR) {
                    currentFirstLastStates = this->OperatorPlus(automaton, regOR, firstState, lastState,
                                                                currentFirstState, currentLastState);
                    firstState = std::get<0>(currentFirstLastStates);
                    lastState = std::get<1>(currentFirstLastStates);

                    regOR = false;
                }
                else {
                    firstState = currentFirstState;
                    lastState = currentLastState;
                }

            }
            previousFirstState = currentFirstState;
            previousLastState = currentLastState;
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

bool RegToeNFA::checkConcatenationPrevious(ENFA &automaton, std::string regex, int currentPos) {
    if (currentPos - 1 >= 0) {
        // Check if concatenation with previous ')', '*' or character from the alphabet
        if ((regex[currentPos-1] == ')') or (regex[currentPos-1] == '*') or
            ((automaton.getAlphabet().find(std::string(1, regex[currentPos-1]))
              != automaton.getAlphabet().end()))) {
            return true;
        }
    }
    return false;
}

bool RegToeNFA::checkConcatenationNext(ENFA &automaton, std::string regex, int currentPos) {
    if (currentPos + 1 < regex.size()) {
        // Check if concatenation with next '(' or next character from the alphabet
        if ((regex[currentPos + 1] == '(') or
            ((automaton.getAlphabet().find(std::string(1, regex[currentPos + 1]))
              != automaton.getAlphabet().end()))) {
            return true;
        }
    }


    // Voor geval "a**" moet een loop geschreven

    // In case of next char '*' check character after it
    if (currentPos + 2 < regex.size() and regex[currentPos + 1] == '*') {
        if ((regex[currentPos + 2] == '(') or
            ((automaton.getAlphabet().find(std::string(1, regex[currentPos + 2]))
              != automaton.getAlphabet().end()))) {
            return true;
        }

    }

    return false;
}

std::tuple<State *, State *> RegToeNFA::OperatorPlus(ENFA &automaton, bool &regOR,
                                                     State *firstState, State *lastState,
                                                     State *currentFirstState, State *currentLastState) {
    //If this is the first + in the current parentheses


    //Create new firststate, connect previous firststate to that new state
    //Create new laststate, connect previous laststate to that new state
    //Connect first and last to currentStates
    if (regOR) {
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


        regOR = false;
    }

    return std::make_tuple(firstState, lastState);
};

std::tuple<State*, State*> RegToeNFA::OperatorStar(ENFA& automaton, State* currentFirstState,
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