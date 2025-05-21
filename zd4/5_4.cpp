#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <limits>

using namespace std;

// Класс для работы со справочником регионов
class RegionDirectory {
private:
    // Основное хранилище: регион -> административный центр
    map<string, string> regions;
    
    // Дополнительное хранилище для проверки уникальности центров: центр -> регион
    map<string, string> centers;

public:
    // Метод для создания/изменения административного центра региона
    void change(const string& region, const string& new_center) {
        // Если регион не существует
        if (regions.find(region) == regions.end()) {
            // Создаем новую запись
            regions[region] = new_center;
            centers[new_center] = region;
            cout << "New region " << region << " with administrative center " << new_center << endl;
        } else {
            // Получаем старый центр региона
            string old_center = regions[region];
            
            // Проверяем, что новый центр отличается от старого
            if (old_center == new_center) {
                cerr << "Incorrect" << endl;
                return;
            }
            
            // Удаляем старую запись из centers
            centers.erase(old_center);
            
            // Обновляем центр региона
            regions[region] = new_center;
            centers[new_center] = region;
            
            cout << "Region " << region << " has changed its administrative center from " 
                 << old_center << " to " << new_center << endl;
        }
    }

    // Метод для переименования региона
    void rename(const string& old_region, const string& new_region) {
        // Проверяем условия:
        // 1. Старый регион должен существовать
        // 2. Новый регион не должен существовать
        // 3. Имена не должны совпадать
        if (regions.find(old_region) == regions.end() || 
            regions.find(new_region) != regions.end() || 
            old_region == new_region) {
            cerr << "Incorrect" << endl;
            return;
        }
        
        // Получаем центр старого региона
        string center = regions[old_region];
        
        // Удаляем старую запись
        regions.erase(old_region);
        
        // Создаем новую запись
        regions[new_region] = center;
        centers[center] = new_region;
        
        cout << old_region << " has been renamed to " << new_region << endl;
    }

    // Метод для получения информации о регионе
    void about(const string& region) const {
        auto it = regions.find(region);
        if (it == regions.end()) {
            cerr << "Incorrect" << endl;
        } else {
            cout << region << " has administrative center " << it->second << endl;
        }
    }

    // Метод для вывода всех регионов и их центров
    void all() const {
        for (const auto& pair : regions) {
            cout << pair.first << " - " << pair.second << endl;
        }
    }

    void help() const {
        cout << "\nAVAILABLE COMMANDS:\n"
             << "1. CHANGE <region> <center> - create/change region\n"
             << "2. RENAME <old_region> <new_region> - rename region\n"
             << "3. ABOUT <region> - show region's center\n"
             << "4. ALL - show all regions\n"
             << "5. HELP - show this help\n\n";
    }
};

int main() {
    RegionDirectory directory;
    
    cout << "REGION DIRECTORY PROGRAM\n";
    directory.help();
    
    cout << "Enter number of commands: ";
    int n;
    cin >> n;
    cin.ignore(); // Пропустить оставшийся символ новой строки после числа
    
    for (int i = 0; i < n; ++i) {
        cout << "> ";
        string command;
        cin >> command;
        
        try {
            if (command == "CHANGE") {
                string region, new_center;
                cin >> region;
                getline(cin, new_center);
                new_center = new_center.substr(1); // Удаляем первый пробел
                directory.change(region, new_center);
            } else if (command == "RENAME") {
                string old_region, new_region;
                cin >> old_region >> new_region;
                directory.rename(old_region, new_region);
            } else if (command == "ABOUT") {
                string region;
                cin >> region;
                directory.about(region);
            } else if (command == "ALL") {
                directory.all();
            } else if (command == "HELP") {
                directory.help();
            } else {
                cerr << "Incorrect command. Type HELP for available commands." << endl;
            }
        } catch (const exception& e) {
            cerr << "Incorrect input format. Type HELP for usage info." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    return 0;
}