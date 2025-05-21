#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>

using namespace std;

// Конфигурация склада по варианту 2
const int ZONES_COUNT = 10; //кол-во зон
const int RACKS_COUNT = 3; // кол-во стелажей в зоне
const int SECTIONS_COUNT = 1; //кол-во секций в стелаже
const int SHELVES_COUNT = 5; //кол-во полок в секции
const int MAX_ITEMS_PER_CELL = 10; //max кол-во товаров в одной ячейке
const int TOTAL_CAPACITY = 1500; //вместимость склада

// Структура для хранения информации о ячейке
struct Cell {
    string address;
    map<string, int> items; // товар -> количество
    int totalItems = 0;
};

// Класс для управления складом
class Warehouse {
private:
    vector<Cell> cells; 
    int totalItemsInWarehouse = 0; //общее кол-во тавара на складе

    // Генерация всех возможных адресов ячеек
    void generateCells() {
        const char zoneLetters[ZONES_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
        
        for (char zone : zoneLetters) {
            for (int rack = 1; rack <= RACKS_COUNT; ++rack) {
                for (int section = 1; section <= SECTIONS_COUNT; ++section) {
                    for (int shelf = 1; shelf <= SHELVES_COUNT; ++shelf) {
                        stringstream address;
                        address << zone << rack << section << shelf;
                        cells.push_back({address.str()});
                    }
                }
            }
        }
    }

    // Поиск ячейки по адресу
    Cell* findCell(const string& address) {
        for (auto& cell : cells) {
            if (cell.address == address) {
                return &cell;
            }
        }
        return nullptr;
    }

public:
    Warehouse() {
        generateCells();
    }

    // Добавление товара в ячейку
    bool addItem(const string& itemName, int quantity, const string& address) {
        //Поиск ячейки по адресу
        Cell* cell = findCell(address);
        if (!cell) {
            cout << "Ошибка: Ячейка с адресом " << address << " не найдена." << endl;
            return false;
        }
        // проверка на заполненость ячейки
        if (cell->totalItems + quantity > MAX_ITEMS_PER_CELL) {
            cout << "Ошибка: Превышена вместимость ячейки " << address 
                 << " (текущее количество: " << cell->totalItems 
                 << ", пытаетесь добавить: " << quantity 
                 << ", максимум: " << MAX_ITEMS_PER_CELL << ")." << endl;
            return false;
        }
        
        cell->items[itemName] += quantity;
        cell->totalItems += quantity;
        totalItemsInWarehouse += quantity;
        return true;
    }

    // Удаление товара из ячейки
    bool removeItem(const string& itemName, int quantity, const string& address) {
        Cell* cell = findCell(address);
        if (!cell) {
            cout << "Ошибка: Ячейка с адресом " << address << " не найдена." << endl;
            return false;
        }

        if (cell->items.find(itemName) == cell->items.end() || cell->items[itemName] < quantity) {
            cout << "Ошибка: Недостаточно товара " << itemName << " в ячейке " << address 
                 << " (текущее количество: " << (cell->items.count(itemName) ? cell->items[itemName] : 0) 
                 << ", пытаетесь удалить: " << quantity << ")." << endl;
            return false;
        }
        
        cell->items[itemName] -= quantity;
        cell->totalItems -= quantity;
        totalItemsInWarehouse -= quantity;

        if (cell->items[itemName] == 0) {
            cell->items.erase(itemName);
        }

        return true;
    }

    // Получение информации о складе
    void printInfo() const {
        // Общая загруженность склада
        double warehouseLoad = (static_cast<double>(totalItemsInWarehouse) / TOTAL_CAPACITY) * 100;
        cout << "Общая загруженность склада: " << fixed << setprecision(2) << warehouseLoad << "%" << endl;

        // Загруженность по зонам
        map<char, int> zoneItems;
        vector<string> emptyCells;

        for (const auto& cell : cells) {
            char zone = cell.address[0];
            zoneItems[zone] += cell.totalItems;

            if (cell.totalItems == 0) {
                emptyCells.push_back(cell.address);
            }
        }

        cout << "\nЗагруженность по зонам:" << endl;
        for (const auto& [zone, items] : zoneItems) {
            double zoneLoad = (static_cast<double>(items) / 
                             (cells.size() / ZONES_COUNT * MAX_ITEMS_PER_CELL)) * 100;
            cout << "Зона " << zone << ": " << fixed << setprecision(2) << zoneLoad << "%" << endl;
        }

        // Содержимое непустых ячеек
        cout << "\nСодержимое ячеек:" << endl;
        for (const auto& cell : cells) {
            if (cell.totalItems > 0) {
                cout << "Ячейка " << cell.address << " (всего " << cell.totalItems << "): ";
                for (const auto& [item, quantity] : cell.items) {
                    cout << item << " - " << quantity << ", ";
                }
                cout << endl;
            }
        }

        // Пустые ячейки
        cout << "\nПустые ячейки (" << emptyCells.size() << "): ";
        for (size_t i = 0; i < emptyCells.size(); ++i) {
            if (i > 0 && i % 10 == 0) cout << endl;
            cout << emptyCells[i] << " ";
        }
        cout << endl;
    }
};

// Функция для обработки ввода пользователя
void processCommand(Warehouse& warehouse) {
    string command;
    cout << "\nВведите команду (ADD, REMOVE, INFO, EXIT): ";
    getline(cin, command);

    if (command.empty()) return;

    // Преобразование команды в верхний регистр
    transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == "INFO") {
        warehouse.printInfo();
    }
    else if (command == "EXIT") {
        exit(0);
    }
    else if (command.find("ADD ") == 0 || command.find("REMOVE ") == 0) {
        istringstream iss(command);
        string action, itemName, address;
        int quantity;

        iss >> action >> itemName >> quantity >> address;

        // Проверка адреса
        if (address.size() != 4) {
            cout << "Ошибка: Неверный формат адреса. Должен быть в формате ZRSP(зона-стеллаж-секция-полка), например А1234." << endl;
            return;
        }

        // Проверка количества
        if (quantity <= 0) {
            cout << "Ошибка: Количество должно быть положительным числом." << endl;
            return;
        }

        if (action == "ADD") {
            warehouse.addItem(itemName, quantity, address);
        }
        else if (action == "REMOVE") {
            warehouse.removeItem(itemName, quantity, address);
        }
    }
    else {
        cout << "Ошибка: Неизвестная команда. Доступные команды: ADD, REMOVE, INFO, EXIT." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    Warehouse warehouse;
    cout << "Программа учета товаров на складе" << endl;
    cout << "Конфигурация склада:" << endl;
    cout << "- Зон хранения: " << ZONES_COUNT << endl;
    cout << "- Стеллажей в зоне: " << RACKS_COUNT << endl;
    cout << "- Секций в стеллаже: " << SECTIONS_COUNT << endl;
    cout << "- Полок в секции: " << SHELVES_COUNT << endl;
    cout << "- Максимум в ячейке: " << MAX_ITEMS_PER_CELL << " единиц" << endl;
    cout << "- Общая вместимость: " << TOTAL_CAPACITY << " единиц" << endl;

    while (true) {
        processCommand(warehouse);
    }

    return 0;
}