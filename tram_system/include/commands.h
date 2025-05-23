#pragma once
#include <string>
#include <vector>

enum class CommandType {
    CREATE_TRAM,
    TRAMS_IN_STOP,
    STOPS_IN_TRAM,
    TRAMS,
    UNKNOWN
};

struct Command {
    CommandType type;
    std::vector<std::string> args; // Аргументы команды (например, номер трамвая, название остановки и т.д.)
};

Command parseCommand(const std::string& input);