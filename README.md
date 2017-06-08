# Toepassingsopdracht Talen en automaten

Made by:
* [Loreas Clonen](https://github.com/Loreas)
* [Sergio Fenoll](https://github.com/sergiofenoll)
* [Jonathan Meyer](https://github.com/MeyerJon)
* [Dawid Miroyan](https://github.com/DawidMiroyan)

## Dependencies:
* [JSON.hpp](https://github.com/nlohmann/json)
* A program that communicates with your chat program of choice
  * For testing purposes we use: [fbchat](https://github.com/carpedm20/fbchat)
* CMake
* Make


## Installation:

_No permanent installation supported yet._

_You will have to run the executable from the source directory._

```
git clone https://github.com/Loreas/ARCE.git
cd ARCE/
cmake .
make
./ARCE
```

## How it works:
The bot reads all incoming messages. Messages starting with a chosen character (currently set as '!') should be handled by the bot, which writes it to a txt file (link/link.txt).

Now the ARCE program reads the file and validates both the command and its arguments with a Deterministic Finite Automaton. If it is valid, the program will execute the script that is associated with the command.

If the command is invalid (a typo has been made) it will be ran through a Levenshtein automaton to find a possible correct match which will then be sent to the chat.

If the arguments are invalid (not compliant with the pre-defined regex) the correct form will be sent to the chat. 

## Adding scripts:
Scripts (and executables) should be placed in the scripts folder. All languages should be supported, as long as they can write to a file.

The output should be written to link/linkToBot.txt, this way the chat bot can send the output. If you can't/don't want to write to a file, you can also send your output to the file like this ``` ./scripts/executable > ./link/linkToBot.txt ```.

Remember to also add an entry in commands.json using this format:

```
{
      "name": "Example dice roll command",
      "description": "This is an example command for a dice roll",
      "command": "exampleRoll",
      "arguments": "(1+2+3+4+5+6+7+8+9)(0+1+2+3+4+5+6+7+8+9)*",
      "amountArgs": "1",
      "form": "exampleRoll <value>",
      "language": "python3",
      "execute": "python3 ./scripts/exampleRoll.py"
}
```

## Possible commands:
* !ball <question>
* !roll <number>
* !log start
* !log stop
* !log <year-month-day>
* !log list
* !note <sentence>

