using System;
using System.Collections.Generic;
using System.Linq;

// Генератор номеров билетов
class TicketNumberGenerator
{
    private Random random = new Random();
    
    public string Generate()
    {
        return $"T{random.Next(0, 1000):D3}";
    }
}

// Структура билета
struct Ticket : IComparable<Ticket>
{
    public string Number;     // Номер билета
    public int Duration;      // Время обработки
    
    // Для сортировки по убыванию времени обработки
    public int CompareTo(Ticket other)
    {
        return other.Duration.CompareTo(this.Duration);
    }
}

// Система управления очередями
class QueueSystem
{
    private int windowsCount;
    private List<Ticket> tickets = new List<Ticket>();
    private List<List<Ticket>> windows;
    private TicketNumberGenerator ticketGen = new TicketNumberGenerator();
    
    // Распределить билеты по окнам
    private void DistributeTickets()
    {
        // Минимальная куча для балансировки нагрузки
        var loadHeap = new SortedSet<(int time, int idx)>(
            Comparer<(int time, int idx)>.Create((a, b) => 
                a.time != b.time ? a.time.CompareTo(b.time) : a.idx.CompareTo(b.idx)));
        
        // Инициализация окон
        for (int i = 0; i < windowsCount; ++i)
        {
            loadHeap.Add((0, i));
        }
        
        // Распределение билетов
        foreach (var ticket in tickets)
        {
            var (time, idx) = loadHeap.Min;
            loadHeap.Remove(loadHeap.Min);
            
            windows[idx].Add(ticket);
            time += ticket.Duration;
            loadHeap.Add((time, idx));
        }
    }
    
    // Вывести результаты распределения
    private void PrintDistribution()
    {
        int[] totalTimes = new int[windowsCount];
        
        for (int i = 0; i < windowsCount; ++i)
        {
            if (windows[i].Count == 0) continue;
            
            // Вычисляем общее время для окна
            foreach (var ticket in windows[i])
            {
                totalTimes[i] += ticket.Duration;
            }
            
            // Выводим информацию об окне
            Console.Write($">>> Окно {i + 1} ({totalTimes[i]} минут): ");
            
            Console.WriteLine(string.Join(", ", windows[i].Select(t => t.Number)));
        }
        
        // Максимальное время обработки
        int maxTime = totalTimes.Max();
        Console.WriteLine($">>> Максимальное время обработки: {maxTime} минут");
    }

    public QueueSystem(int numWindows)
    {
        windowsCount = numWindows;
        windows = new List<List<Ticket>>();
        for (int i = 0; i < windowsCount; i++)
        {
            windows.Add(new List<Ticket>());
        }
    }
    
    // Добавить билет в систему
    public void AddTicket(int duration)
    {
        if (duration <= 0)
        {
            Console.Error.WriteLine("Ошибка: время обработки должно быть положительным");
            return;
        }
        
        Ticket ticket = new Ticket { Number = ticketGen.Generate(), Duration = duration };
        tickets.Add(ticket);
        Console.WriteLine($">>> {ticket.Number}");
    }
    
    // Выполнить распределение
    public void Distribute()
    {
        if (tickets.Count == 0)
        {
            Console.WriteLine(">>> Нет билетов для распределения");
            return;
        }
        
        // Сортировка по времени обработки (по убыванию)
        tickets.Sort();
        
        DistributeTickets();
        PrintDistribution();
    }
}

class Program
{
    static void Main(string[] args)
    {
        // Ввод количества окон
        Console.WriteLine(">>> Введите кол-во окон");
        Console.Write("<<< ");
        if (!int.TryParse(Console.ReadLine(), out int windows) || windows <= 0)
        {
            Console.Error.WriteLine("Ошибка: введите положительное число");
            return;
        }
        
        // Инициализация системы
        QueueSystem system = new QueueSystem(windows);
        
        // Основной цикл обработки команд
        while (true)
        {
            Console.Write("<<< ");
            string input = Console.ReadLine();
            if (string.IsNullOrEmpty(input)) continue;
            
            string[] parts = input.Split(' ', StringSplitOptions.RemoveEmptyEntries);
            string command = parts[0].ToUpper();
            
            if (command == "ENQUEUE" && parts.Length > 1)
            {
                if (int.TryParse(parts[1], out int duration))
                {
                    system.AddTicket(duration);
                }
                else
                {
                    Console.Error.WriteLine("Ошибка: введите число для длительности");
                }
            }
            else if (command == "DISTRIBUTE")
            {
                system.Distribute();
                break;
            }
            else
            {
                Console.Error.WriteLine("Неизвестная команда. Используйте ENQUEUE <длительность> или DISTRIBUTE.");
            }
        }
    }
}