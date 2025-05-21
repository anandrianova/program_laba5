using System;
using System.Collections.Generic;

// Класс для работы со справочником регионов
class RegionDirectory
{
    // Основное хранилище: регион -> административный центр
    private Dictionary<string, string> regions = new Dictionary<string, string>();
    
    // Дополнительное хранилище для проверки уникальности центров: центр -> регион
    private Dictionary<string, string> centers = new Dictionary<string, string>();

    // Метод для создания/изменения административного центра региона
    public void Change(string region, string newCenter)
    {
        // Если регион не существует
        if (!regions.ContainsKey(region))
        {
            // Создаем новую запись
            regions[region] = newCenter;
            centers[newCenter] = region;
            Console.WriteLine($"New region {region} with administrative center {newCenter}");
        }
        else
        {
            // Получаем старый центр региона
            string oldCenter = regions[region];
            
            // Проверяем, что новый центр отличается от старого
            if (oldCenter == newCenter)
            {
                Console.Error.WriteLine("Incorrect");
                return;
            }
            
            // Удаляем старую запись из centers
            centers.Remove(oldCenter);
            
            // Обновляем центр региона
            regions[region] = newCenter;
            centers[newCenter] = region;
            
            Console.WriteLine($"Region {region} has changed its administrative center from {oldCenter} to {newCenter}");
        }
    }

    // Метод для переименования региона
    public void Rename(string oldRegion, string newRegion)
    {
        // Проверяем условия:
        // 1. Старый регион должен существовать
        // 2. Новый регион не должен существовать
        // 3. Имена не должны совпадать
        if (!regions.ContainsKey(oldRegion) || 
            regions.ContainsKey(newRegion) || 
            oldRegion == newRegion)
        {
            Console.Error.WriteLine("Incorrect");
            return;
        }
        
        // Получаем центр старого региона
        string center = regions[oldRegion];
        
        // Удаляем старую запись
        regions.Remove(oldRegion);
        
        // Создаем новую запись
        regions[newRegion] = center;
        centers[center] = newRegion;
        
        Console.WriteLine($"{oldRegion} has been renamed to {newRegion}");
    }

    // Метод для получения информации о регионе
    public void About(string region)
    {
        if (regions.TryGetValue(region, out string center))
        {
            Console.WriteLine($"{region} has administrative center {center}");
        }
        else
        {
            Console.Error.WriteLine("Incorrect");
        }
    }

    // Метод для вывода всех регионов и их центров
    public void All()
    {
        foreach (var pair in regions)
        {
            Console.WriteLine($"{pair.Key} - {pair.Value}");
        }
    }

    public void Help()
    {
        Console.WriteLine("\nAVAILABLE COMMANDS:");
        Console.WriteLine("1. CHANGE <region> <center> - create/change region");
        Console.WriteLine("2. RENAME <old_region> <new_region> - rename region");
        Console.WriteLine("3. ABOUT <region> - show region's center");
        Console.WriteLine("4. ALL - show all regions");
        Console.WriteLine("5. HELP - show this help\n");
    }
}

class Program
{
    static void Main(string[] args)
    {
        RegionDirectory directory = new RegionDirectory();
        
        Console.WriteLine("REGION DIRECTORY PROGRAM");
        directory.Help();
        
        Console.Write("Enter number of commands: ");
        int n = int.Parse(Console.ReadLine());
        
        for (int i = 0; i < n; i++)
        {
            Console.Write("> ");
            string input = Console.ReadLine();
            string[] parts = input.Split(new[] {' '}, 3, StringSplitOptions.RemoveEmptyEntries);
            
            if (parts.Length == 0) continue;
            
            string command = parts[0].ToUpper();
            
            try
            {
                switch (command)
                {
                    case "CHANGE":
                        if (parts.Length != 3)
                        {
                            Console.Error.WriteLine("Incorrect command format. Use: CHANGE <region> <center>");
                            continue;
                        }
                        directory.Change(parts[1], parts[2]);
                        break;
                        
                    case "RENAME":
                        if (parts.Length != 3)
                        {
                            Console.Error.WriteLine("Incorrect command format. Use: RENAME <old_region> <new_region>");
                            continue;
                        }
                        directory.Rename(parts[1], parts[2]);
                        break;
                        
                    case "ABOUT":
                        if (parts.Length != 2)
                        {
                            Console.Error.WriteLine("Incorrect command format. Use: ABOUT <region>");
                            continue;
                        }
                        directory.About(parts[1]);
                        break;
                        
                    case "ALL":
                        directory.All();
                        break;
                        
                    case "HELP":
                        directory.Help();
                        break;
                        
                    default:
                        Console.Error.WriteLine("Incorrect command. Type HELP for available commands.");
                        break;
                }
            }
            catch (Exception e)
            {
                Console.Error.WriteLine($"Incorrect input format: {e.Message}. Type HELP for usage info.");
            }
        }
    }
}