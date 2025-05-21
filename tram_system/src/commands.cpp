#include "commands.h"
#include <algorithm>
#include <sstream>

Command parseCommand(const std::string& input) {
    Command cmd;
    std::istringstream iss(input);
    std::string token;
    
    if (iss >> token) {
        // Приводим команду к верхнему регистру
        std::transform(token.begin(), token.end(), token.begin(), ::toupper);
        
        if (token == "CREATE_TRAM") cmd.type = CommandType::CREATE_TRAM;
        else if (token == "TRAMS_IN_STOP") cmd.type = CommandType::TRAMS_IN_STOP;
        else if (token == "STOPS_IN_TRAM") cmd.type = CommandType::STOPS_IN_TRAM;
        else if (token == "TRAMS") cmd.type = CommandType::TRAMS;
        else cmd.type = CommandType::UNKNOWN;
    }

    // Добавляем аргументы
    while (iss >> token) {
        cmd.args.push_back(token);
    }

    return cmd;
}