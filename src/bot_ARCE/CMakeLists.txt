set(BOT_SOURCE_FILES Bot.cpp Bot.h Command.cpp Command.h Parser.cpp Parser.h)

add_library(bot_ARCE ${BOT_SOURCE_FILES})
target_link_libraries(bot_ARCE automata algos)
