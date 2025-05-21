#include "tram.h"

Tram::Tram(const std::string& name, const std::vector<std::string>& stops)
    : name(name), stops(stops) {}

const std::string& Tram::getName() const {
    return name;
}

const std::vector<std::string>& Tram::getStops() const {
    return stops;
}

bool Tram::passesThrough(const std::string& stop) const {
    for (const auto& s : stops) {
        if (s == stop) {
            return true;
        }
    }
    return false;
}