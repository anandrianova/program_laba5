#ifndef TRAM_SYSTEM_H
#define TRAM_SYSTEM_H

#include <map>
#include <string>
#include <vector>
#include "tram.h"
#include "stop.h"

class TramSystem {
private:
    std::map<std::string, Tram> trams;
    std::map<std::string, Stop> stops;

public:
    void createTram(const std::string& tramName, const std::vector<std::string>& stopNames);
    std::vector<std::string> getTramsInStop(const std::string& stopName) const;
    std::vector<std::pair<std::string, std::vector<std::string>>> getStopsInTram(const std::string& tramName) const;
    std::map<std::string, std::vector<std::string>> getAllTrams() const;
};

#endif // TRAM_SYSTEM_H