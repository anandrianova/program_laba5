#ifndef TRAM_H
#define TRAM_H

#include <string>
#include <vector>

class Tram {
private:
    std::string name; // Имя трамвая
    std::vector<std::string> stops; // Список остановок, на которых останавливается трамвай

public:
    Tram(const std::string& name, const std::vector<std::string>& stops); // Конструктор класса Tram, принимающий имя трамвая и список остановок
    
    const std::string& getName() const; // Метод для получения имени трамвая
    const std::vector<std::string>& getStops() const; // Метод для получения списка остановок трамвая
    
    bool passesThrough(const std::string& stop) const; // Метод для проверки, проходит ли трамвай через указанную остановку
};

#endif // TRAM_H