#include "tram_system.h"
#include <algorithm>

void TramSystem::createTram(const std::string& tramName, const std::vector<std::string>& stopNames) {
    // Проверка на минимальное количество остановок
    if (stopNames.size() < 2) {
        throw std::invalid_argument("Tram must have at least 2 stops");
    }

    // Проверка на существование трамвая
    if (trams.find(tramName) != trams.end()) {
        throw std::invalid_argument("Tram with name '" + tramName + "' already exists");
    }

    // Проверка на уникальность соседних остановок
    if (std::adjacent_find(stopNames.begin(), stopNames.end()) != stopNames.end()) {
        throw std::invalid_argument("Consecutive stops cannot be identical");
    }

    // Создаем трамвай
    trams.emplace(tramName, Tram(tramName, stopNames));
    
    // Обновляем информацию об остановках
    for (const auto& stopName : stopNames) {
        auto it = stops.find(stopName);
        if (it == stops.end()) {
            stops.emplace(stopName, Stop(stopName));
            stops.at(stopName).addTram(tramName);
        } else {
            it->second.addTram(tramName);
        }
    }
}

std::vector<std::string> TramSystem::getTramsInStop(const std::string& stopName) const {
    auto it = stops.find(stopName);
    if (it != stops.end()) {
        return it->second.getTrams();
    }
    return {};
}

std::vector<std::pair<std::string, std::vector<std::string>>> TramSystem::getStopsInTram(const std::string& tramName) const {
    std::vector<std::pair<std::string, std::vector<std::string>>> result;
    
    auto tramIt = trams.find(tramName);
    if (tramIt == trams.end()) {
        return result;
    }
    
    for (const auto& stopName : tramIt->second.getStops()) {
        auto stopIt = stops.find(stopName);
        if (stopIt != stops.end()) {
            std::vector<std::string> otherTrams;
            for (const auto& tram : stopIt->second.getTrams()) {
                if (tram != tramName) {
                    otherTrams.push_back(tram);
                }
            }
            result.emplace_back(stopName, otherTrams);
        }
    }
    
    return result;
}

std::map<std::string, std::vector<std::string>> TramSystem::getAllTrams() const {
    std::map<std::string, std::vector<std::string>> result;
    
    for (const auto& tramPair : trams) {
        result[tramPair.first] = tramPair.second.getStops();
    }
    
    return result;
}