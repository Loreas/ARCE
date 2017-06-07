set(AUTOMATA_SOURCE_FILES DFA.cpp DFA.h ENFA.cpp ENFA.h FA.cpp FA.h NFA.cpp NFA.h State.cpp State.h)

add_library(automata ${AUTOMATA_SOURCE_FILES})
