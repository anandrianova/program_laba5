#ifndef TRAM_H
#define TRAM_H

#include <string>
#include <vector>

class Tram {
private:
    std::string name;
    std::vector<std::string> stops;

public:
    Tram(const std::string& name, const std::vector<std::string>& stops);
    
    const std::string& getName() const;
    const std::vector<std::string>& getStops() const;
    
    bool passesThrough(const std::string& stop) const;
};

#endif // TRAM_H