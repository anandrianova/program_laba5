#include "stop.h"

Stop::Stop(const std::string& name) : name(name) {}

const std::string& Stop::getName() const {
    return name;
}

const std::vector<std::string>& Stop::getTrams() const {
    return trams;
}

void Stop::addTram(const std::string& tram) {
    trams.push_back(tram);
}