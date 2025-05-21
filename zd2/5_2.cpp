#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <iomanip>
#include <random>
#include <limits>

using namespace std;

// Генератор номеров билетов
class TicketNumberGenerator {
private:
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> dis;
    
public:
    TicketNumberGenerator() : gen(rd()), dis(0, 999) {}
    
    string generate() {
        stringstream ss;
        ss << "T" << setw(3) << setfill('0') << dis(gen);
        return ss.str();
    }
};

// Структура билета
struct Ticket {
    string number;     // Номер билета
    int duration;      // Время обработки
    
    // Для сортировки по убыванию времени обработки
    bool operator>(const Ticket& other) const {
        return duration > other.duration;
    }
};

// Система управления очередями
class QueueSystem {
private:
    int windowsCount;
    vector<Ticket> tickets;
    vector<vector<Ticket>> windows;
    TicketNumberGenerator ticketGen;
    
    // Распределить билеты по окнам
    void distributeTickets() {
        // Минимальная куча для балансировки нагрузки
        priority_queue<pair<int, int>, 
                      vector<pair<int, int>>, 
                      greater<pair<int, int>>> loadHeap;
        
        // Инициализация окон
        for (int i = 0; i < windowsCount; ++i) {
            loadHeap.push({0, i});
        }
        
        // Распределение билетов
        for (const auto& ticket : tickets) {
            auto [time, idx] = loadHeap.top();
            loadHeap.pop();
            
            windows[idx].push_back(ticket);
            time += ticket.duration;
            loadHeap.push({time, idx});
        }
    }
    
    // Вывести результаты распределения
    void printDistribution() const {
        vector<int> totalTimes(windowsCount, 0);
        
        for (int i = 0; i < windowsCount; ++i) {
            if (windows[i].empty()) continue;
            
            // Вычисляем общее время для окна
            for (const auto& ticket : windows[i]) {
                totalTimes[i] += ticket.duration;
            }
            
            // Выводим информацию об окне
            cout << ">>> Окно " << (i + 1) 
                 << " (" << totalTimes[i] << " минут): ";
            
            bool first = true;
            for (const auto& ticket : windows[i]) {
                if (!first) cout << ", ";
                cout << ticket.number;
                first = false;
            }
            cout << endl;
        }
        
        // Максимальное время обработки
        int maxTime = *max_element(totalTimes.begin(), totalTimes.end());
        cout << ">>> Максимальное время обработки: " 
             << maxTime << " минут" << endl;
    }

public:
    QueueSystem(int numWindows) : windowsCount(numWindows) {
        windows.resize(windowsCount);
    }
    
    // Добавить билет в систему
    void addTicket(int duration) {
        if (duration <= 0) {
            cerr << "Ошибка: время обработки должно быть положительным" << endl;
            return;
        }
        
        Ticket ticket{ticketGen.generate(), duration};
        tickets.push_back(ticket);
        cout << ">>> " << ticket.number << endl;
    }
    
    // Выполнить распределение
    void distribute() {
        if (tickets.empty()) {
            cout << ">>> Нет билетов для распределения" << endl;
            return;
        }
        
        // Сортировка по времени обработки (по убыванию)
        sort(tickets.begin(), tickets.end(), greater<Ticket>());
        
        distributeTickets();
        printDistribution();
    }
};

// Обработчик команд
class CommandProcessor {
private:
    QueueSystem& system;
    
    void processEnqueue() {
        int duration;
        if (!(cin >> duration)) {
            cerr << "Ошибка: введите число для длительности" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        system.addTicket(duration);
    }
    
    void processDistribute() {
        system.distribute();
    }
    
public:
    CommandProcessor(QueueSystem& sys) : system(sys) {}
    
    bool processCommand(const string& cmd) {
        string upperCmd = cmd;
        transform(upperCmd.begin(), upperCmd.end(), upperCmd.begin(), ::toupper);
        
        if (upperCmd == "ENQUEUE") {
            processEnqueue();
            return true;
        }
        else if (upperCmd == "DISTRIBUTE") {
            processDistribute();
            return false; // Завершить цикл
        }
        else {
            cerr << "Неизвестная команда. Используйте ENQUEUE или DISTRIBUTE." << endl;
            return true;
        }
    }
};

int main() {
    // Ввод количества окон
    cout << ">>> Введите кол-во окон" << endl << "<<< ";
    int windows;
    if (!(cin >> windows) || windows <= 0) {
        cerr << "Ошибка: введите положительное число" << endl;
        return 1;
    }
    
    // Инициализация системы
    QueueSystem system(windows);
    CommandProcessor processor(system);
    
    // Основной цикл обработки команд
    string command;
    bool continueProcessing = true;
    
    while (continueProcessing) {
        cout << "<<< ";
        if (!(cin >> command)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        continueProcessing = processor.processCommand(command);
    }
    
    return 0;
}