#ifndef TRAM_SYSTEM_H
#define TRAM_SYSTEM_H

#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include "tram.h"
#include "stop.h"

class TramSystem {
private:
    std::map<std::string, Tram> trams; // Словарь, где ключ - имя трамвая, значение - объект трамвая
    std::map<std::string, Stop> stops; // Словарь, где ключ - имя остановки, значение - объект остановки

public:
    void createTram(const std::string& tramName, const std::vector<std::string>& stopNames);
    std::vector<std::string> getTramsInStop(const std::string& stopName) const; // Метод для получения списка трамваев, которые останавливаются на указанной остановке
    std::vector<std::pair<std::string, std::vector<std::string>>> getStopsInTram(const std::string& tramName) const; // Метод для получения списка остановок, на которых останавливается указанный трамвай
    std::map<std::string, std::vector<std::string>> getAllTrams() const; // Метод для получения всех трамваев и их остановок в виде ассоциативного массива
};

#endif // TRAM_SYSTEM_H