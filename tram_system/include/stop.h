#ifndef STOP_H
#define STOP_H

#include <string>
#include <vector>

class Stop {
private:
    std::string name;
    std::vector<std::string> trams;

public:
    Stop() = default; // Добавленный конструктор по умолчанию
    Stop(const std::string& name);
    
    const std::string& getName() const;
    const std::vector<std::string>& getTrams() const;
    
    void addTram(const std::string& tram);
};

#endif // STOP_H