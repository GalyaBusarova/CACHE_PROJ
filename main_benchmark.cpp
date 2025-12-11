#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

#include "benchmark.cpp"

int main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << "  БЕНЧМАРК: LRU vs LFU КЭШИРОВАНИЕ\n";
    std::cout << "========================================\n\n";
    
    CacheBenchmark benchmark;
    
    // Параметры по умолчанию
    std::string trace_file = "trace.txt";
    bool generate_trace = false;
    bool quick_mode = false;
    
    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--generate" || arg == "-g") {
            generate_trace = true;
        }
        else if (arg == "--trace" || arg == "-t") {
            if (i + 1 < argc) {
                trace_file = argv[++i];
            }
        }
        else if (arg == "--quick" || arg == "-q") {
            quick_mode = true;
        }
        else if (arg == "--help" || arg == "-h") {
            std::cout << "Использование:\n";
            std::cout << "  " << argv[0] << " [опции]\n\n";
            std::cout << "Опции:\n";
            std::cout << "  -g, --generate      Сгенерировать новую трассу\n";
            std::cout << "  -t, --trace ФАЙЛ    Использовать указанный файл трассы\n";
            std::cout << "  -q, --quick         Быстрый тест (только size=1000)\n";
            std::cout << "  -h, --help          Показать эту справку\n";
            std::cout << "\nПримеры:\n";
            std::cout << "  " << argv[0] << " --generate\n";
            std::cout << "  " << argv[0] << " --trace my_trace.txt --quick\n";
            std::cout << "  " << argv[0] << "                    # Полный тест\n";
            return 0;
        }
    }
    
    // Генерация трассы если нужно
    if (generate_trace) {
        std::cout << "Генерация трассы...\n";
        std::cout << "Запустите Python скрипт:\n";
        std::cout << "  python trace_generator.py " << trace_file << "\n\n";
        
        std::string command = "python trace_generator.py " + trace_file;
        int result = system(command.c_str());
        
        if (result != 0) {
            std::cout << "Не удалось сгенерировать трассу.\n";
            std::cout << "Убедитесь, что установлены Python и numpy.\n";
            std::cout << "Или создайте trace.txt вручную с 100000 чисел.\n";
        }
    }
    
    // Проверяем существование трассы
    std::ifstream test_file(trace_file);
    if (!test_file.good()) {
        std::cout << "Файл трассы '" << trace_file << "' не найден.\n";
        std::cout << "Сгенерируйте его командой:\n";
        std::cout << "  python trace_generator.py\n";
        std::cout << "Или скачайте примерный trace.txt\n";
        return 1;
    }
    
    // Запуск бенчмарка
    try {
        if (quick_mode) {
            // Быстрый тест (как в задании)
            benchmark.quick_test(trace_file, 1000);
        } else {
            // Полный тест с разными размерами кэша
            std::vector<size_t> cache_sizes = {100, 500, 1000, 2000, 5000};
            benchmark.run(trace_file, cache_sizes);
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\nБенчмарк завершен!\n";
    return 0;
}