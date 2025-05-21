#include "tram_system.h"
#include "commands.h"
#include <iostream>

void printHelp() {
    std::cout << "Available commands:\n"
              << "CREATE_TRAM <number> <stop1> <stop2> ...\n"
              << "TRAMS_IN_STOP <stop>\n"
              << "STOPS_IN_TRAM <number>\n"
              << "TRAMS\n"
              << "EXIT\n";
}

int main() {
    TramSystem system;
    std::string input;
    
    printHelp();
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        Command cmd = parseCommand(input);
        
        switch (cmd.type) {
            case CommandType::CREATE_TRAM: {
                if (cmd.args.size() < 2) {
                    std::cout << "Error: Need tram number and at least 1 stop\n";
                    break;
                }
                std::vector<std::string> stops(cmd.args.begin() + 1, cmd.args.end());
                system.createTram(cmd.args[0], stops);
                break;
            }
            case CommandType::TRAMS_IN_STOP: {
                if (cmd.args.empty()) {
                    std::cout << "Error: Specify stop name\n";
                    break;
                }
                auto trams = system.getTramsInStop(cmd.args[0]);
                if (trams.empty()) {
                    std::cout << "No trams for this stop\n";
                } else {
                    for (const auto& tram : trams) {
                        std::cout << tram << " ";
                    }
                    std::cout << "\n";
                }
                break;
            }
            case CommandType::STOPS_IN_TRAM: {
                if (cmd.args.empty()) {
                    std::cout << "Error: Specify tram number\n";
                    break;
                }
                auto stops = system.getStopsInTram(cmd.args[0]);
                if (stops.empty()) {
                    std::cout << "No stops for this tram\n";
                } else {
                    for (const auto& [stop, connected] : stops) {
                        std::cout << "Stop " << stop << ": ";
                        for (const auto& tram : connected) {
                            std::cout << tram << " ";
                        }
                        std::cout << "\n";
                    }
                }
                break;
            }
            case CommandType::TRAMS: {
                auto allTrams = system.getAllTrams();
                if (allTrams.empty()) {
                    std::cout << "No trams in system\n";
                } else {
                    for (const auto& [number, stops] : allTrams) {
                        std::cout << "TRAM " << number << ": ";
                        for (const auto& stop : stops) {
                            std::cout << stop << " ";
                        }
                        std::cout << "\n";
                    }
                }
                break;
            }
            case CommandType::UNKNOWN: {
                if (input == "EXIT") return 0;
                std::cout << "Unknown command\n";
                printHelp();
                break;
            }
        }
    }
    return 0;
}