using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;

namespace WarehouseManagement
{
//Конфигурация склада по варианту 2
    static class WarehouseConfig
    {
        public const int ZonesCount = 10;         //Количество зон 
        public const int RacksCount = 3;          //кол-во стелажей в зоне
        public const int SectionsCount = 1;       //кол-во секций в стелаже
        public const int ShelvesCount = 5;        //кол-во полок в секции
        public const int MaxItemsPerCell = 10;    //max кол-во товаров в одной ячейке
        public const int TotalCapacity = 1500;    //вместимость склада
    }

// Структура для хранения информации о ячейке
    class Cell
    {
        public string Address { get; set; }                  
        public Dictionary<string, int> Items { get; set; }    //товар -> количество
        public int TotalItems { get; set; } = 0;          
        public Cell(string address)
        {
            Address = address;
            Items = new Dictionary<string, int>();
        }
    }

// Класс для управления складом
    class Warehouse
    {
        private List<Cell> cells = new List<Cell>();          // Список всех ячеек склада
        private int totalItemsInWarehouse = 0;        //общее кол-во тавара на складе

        // Генерация всех возможных адресов ячеек на основе конфигурации
        private void GenerateCells()
        {
            char[] zoneLetters = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };

            // Генерация адресов в формате ZRSP (Зона-Стеллаж-Секция-Полка)
            foreach (char zone in zoneLetters)
            {
                for (int rack = 1; rack <= WarehouseConfig.RacksCount; rack++)
                {
                    for (int section = 1; section <= WarehouseConfig.SectionsCount; section++)
                    {
                        for (int shelf = 1; shelf <= WarehouseConfig.ShelvesCount; shelf++)
                        {
                            string address = $"{zone}{rack}{section}{shelf}";
                            cells.Add(new Cell(address));
                        }
                    }
                }
            }
        }

        // Поиск ячейки по адресу (возвращает ячейку или null если не найдена)
        private Cell FindCell(string address)
        {
            return cells.FirstOrDefault(c => c.Address == address);
        }

        // Конструктор - создает склад с заданной конфигурацией
        public Warehouse()
        {
            GenerateCells();
        }

        // Добавление товара в ячейку
        public bool AddItem(string itemName, int quantity, string address)
        {
            // Поиск ячейки по адресу
            Cell cell = FindCell(address);
            if (cell == null)
            {
                Console.WriteLine($"Ошибка: Ячейка с адресом {address} не найдена.");
                return false;
            }

            // Проверка на переполнение ячейки
            if (cell.TotalItems + quantity > WarehouseConfig.MaxItemsPerCell)
            {
                Console.WriteLine($"Ошибка: Превышена вместимость ячейки {address} " +
                    $"(текущее количество: {cell.TotalItems}, " +
                    $"пытаетесь добавить: {quantity}, " +
                    $"максимум: {WarehouseConfig.MaxItemsPerCell}).");
                return false;
            }

            // Добавление товара
            if (cell.Items.ContainsKey(itemName))
                cell.Items[itemName] += quantity;
            else
                cell.Items.Add(itemName, quantity);

            cell.TotalItems += quantity;
            totalItemsInWarehouse += quantity;
            return true;
        }

        // Удаление товара из ячейки
        public bool RemoveItem(string itemName, int quantity, string address)
        {
            // Поиск ячейки по адресу
            Cell cell = FindCell(address);
            if (cell == null)
            {
                Console.WriteLine($"Ошибка: Ячейка с адресом {address} не найдена.");
                return false;
            }

            // Проверка наличия достаточного количества товара
            if (!cell.Items.ContainsKey(itemName) || cell.Items[itemName] < quantity)
            {
                int currentQuantity = cell.Items.ContainsKey(itemName) ? cell.Items[itemName] : 0;
                Console.WriteLine($"Ошибка: Недостаточно товара {itemName} в ячейке {address} " +
                    $"(текущее количество: {currentQuantity}, " +
                    $"пытаетесь удалить: {quantity}).");
                return false;
            }

            // Удаление товара
            cell.Items[itemName] -= quantity;
            cell.TotalItems -= quantity;
            totalItemsInWarehouse -= quantity;

            // Если товара больше не осталось - удаляем запись
            if (cell.Items[itemName] == 0)
            {
                cell.Items.Remove(itemName);
            }

            return true;
        }

        // Вывод информации о состоянии склада
        public void PrintInfo()
        {
            // 1. Общая загруженность склада в процентах
            double warehouseLoad = (double)totalItemsInWarehouse / WarehouseConfig.TotalCapacity * 100;
            Console.WriteLine($"Общая загруженность склада: {warehouseLoad.ToString("F2", CultureInfo.InvariantCulture)}%");

            // 2. Загруженность по зонам
            Dictionary<char, int> zoneItems = new Dictionary<char, int>();  // Количество товаров по зонам
            List<string> emptyCells = new List<string>();                  // Список пустых ячеек

            // Сбор статистики по ячейкам
            foreach (var cell in cells)
            {
                char zone = cell.Address[0];  // Первый символ адреса - зона

                if (zoneItems.ContainsKey(zone))
                    zoneItems[zone] += cell.TotalItems;
                else
                    zoneItems.Add(zone, cell.TotalItems);

                if (cell.TotalItems == 0)
                {
                    emptyCells.Add(cell.Address);
                }
            }

            // Вывод загруженности по зонам
            Console.WriteLine("\nЗагруженность по зонам:");
            foreach (var zone in zoneItems)
            {
                double zoneLoad = (double)zone.Value / 
                                (cells.Count / WarehouseConfig.ZonesCount * WarehouseConfig.MaxItemsPerCell) * 100;
                Console.WriteLine($"Зона {zone.Key}: {zoneLoad.ToString("F2", CultureInfo.InvariantCulture)}%");
            }

            // 3. Содержимое непустых ячеек
            Console.WriteLine("\nСодержимое ячеек:");
            foreach (var cell in cells)
            {
                if (cell.TotalItems > 0)
                {
                    Console.Write($"Ячейка {cell.Address} (всего {cell.TotalItems}): ");
                    foreach (var item in cell.Items)
                    {
                        Console.Write($"{item.Key} - {item.Value}, ");
                    }
                    Console.WriteLine();
                }
            }

            // 4. Список пустых ячеек
            Console.WriteLine($"\nПустые ячейки ({emptyCells.Count}): ");
            for (int i = 0; i < emptyCells.Count; i++)
            {
                if (i > 0 && i % 10 == 0) Console.WriteLine();  // Перенос строки каждые 10 ячеек
                Console.Write($"{emptyCells[i]} ");
            }
            Console.WriteLine();
        }
    }

    class Program
    {
// Функция для обработки ввода пользователя
        static void ProcessCommand(Warehouse warehouse)
        {
            Console.Write("\nВведите команду (ADD, REMOVE, INFO, EXIT): ");
            string command = Console.ReadLine();

            if (string.IsNullOrEmpty(command)) return;

            // Приведение команды к верхнему регистру
            command = command.ToUpper();

            if (command == "INFO")
            {
                warehouse.PrintInfo();  // Вывод информации о складе
            }
            else if (command == "EXIT")
            {
                Environment.Exit(0);  // Завершение программы
            }
            else if (command.StartsWith("ADD ") || command.StartsWith("REMOVE "))
            {
                // Разбор команды ADD/REMOVE
                string[] parts = command.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                
                if (parts.Length != 4)
                {
                    Console.WriteLine("Ошибка: Неверный формат команды. Используйте: ADD/REMOVE [товар] [количество] [адрес]");
                    return;
                }

                string action = parts[0];
                string itemName = parts[1];
                string address = parts[3];

                if (!int.TryParse(parts[2], out int quantity))
                {
                    Console.WriteLine("Ошибка: Количество должно быть целым числом.");
                    return;
                }

                // Проверка формата адреса (должен быть 4 символа: ZRSP)
                if (address.Length != 4)
                {
                    Console.WriteLine("Ошибка: Неверный формат адреса. Должен быть в формате ZRSP, например А1234.");
                    return;
                }

                // Проверка что количество положительное
                if (quantity <= 0)
                {
                    Console.WriteLine("Ошибка: Количество должно быть положительным числом.");
                    return;
                }

                // Выполнение операции
                if (action == "ADD")
                {
                    warehouse.AddItem(itemName, quantity, address);
                }
                else if (action == "REMOVE")
                {
                    warehouse.RemoveItem(itemName, quantity, address);
                }
            }
            else
            {
                Console.WriteLine("Ошибка: Неизвестная команда. Доступные команды: ADD, REMOVE, INFO, EXIT.");
            }
        }

        static void Main(string[] args)
        {
            Console.OutputEncoding = Encoding.UTF8;
            CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;

            Warehouse warehouse = new Warehouse();  // Создание объекта склада

            // Вывод информации о конфигурации склада
            Console.WriteLine("Программа учета товаров на складе");
            Console.WriteLine("Конфигурация склада:");
            Console.WriteLine($"- Зон хранения: {WarehouseConfig.ZonesCount}");
            Console.WriteLine($"- Стеллажей в зоне: {WarehouseConfig.RacksCount}");
            Console.WriteLine($"- Секций в стеллаже: {WarehouseConfig.SectionsCount}");
            Console.WriteLine($"- Полок в секции: {WarehouseConfig.ShelvesCount}");
            Console.WriteLine($"- Максимум в ячейке: {WarehouseConfig.MaxItemsPerCell} единиц");
            Console.WriteLine($"- Общая вместимость: {WarehouseConfig.TotalCapacity} единиц");

            // Основной цикл обработки команд
            while (true)
            {
                ProcessCommand(warehouse);
            }
        }
    }
}