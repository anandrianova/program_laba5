#ifndef STOP_H
#define STOP_H

#include <string>
#include <vector>

class Stop {
private:
    std::string name; // Название остановки
    std::vector<std::string> trams; // Список трамваев, которые останавливаются на этой остановке

public:
    Stop() = default; // Добавленный конструктор по умолчанию
    Stop(const std::string& name); // Конструктор, который инициализирует остановку с заданным именем
    
    const std::string& getName() const; // Метод для получения имени остановки
    const std::vector<std::string>& getTrams() const; // Метод для получения списка трамваев, которые останавливаются на этой остановке
    
    void addTram(const std::string& tram); // Метод для добавления трамвая в список трамваев на этой остановке
};

#endif // STOP_H