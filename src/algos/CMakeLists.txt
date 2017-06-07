set(ALGOS_SOURCE_FILES Levenshtein.cpp Levenshtein.h MSSC.cpp MSSC.h regex_to_enfa.cpp regex_to_enfa.h tfa.cpp tfa.h)

add_library(algos ${ALGOS_SOURCE_FILES})

target_link_libraries(algos automata bot_ARCE)
