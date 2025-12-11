#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>

#include "hash.h"      
#include "lru_cache.h" 

class CacheBenchmark {
private:
    std::vector<int> trace_;
    
    bool load_trace(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
            return false;
        }
        
        trace_.clear();
        int key;
        while (file >> key) {
            trace_.push_back(key);
        }
        
        std::cout << "Загружено " << trace_.size() << " запросов" << std::endl;
        return true;
    }
    
    // Запуск теста для LRU
    void run_lru_test(size_t cache_size, size_t& hits, double& time_ms) {
        auto start = std::chrono::high_resolution_clock::now();
        
        LRUCache<int> cache(cache_size);
        hits = 0;
        
        for (int key : trace_) {
            if (cache.check_to_hit(key)) {
                hits++;
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        time_ms = static_cast<double>(duration.count());
    }
    
    // Запуск теста для LFU
    void run_lfu_test(size_t cache_size, size_t& hits, double& time_ms) {
        auto start = std::chrono::high_resolution_clock::now();
        
        cache_t<int> cache(cache_size);
        hits = 0;
        
        for (int key : trace_) {
            if (cache.check_to_hit(key)) {
                hits++;
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        time_ms = static_cast<double>(duration.count());
    }
    
    // Вывод таблицы
    void print_results_table(size_t cache_size, 
                            const std::string& algo1, size_t hits1, double time1, double rate1,
                            const std::string& algo2, size_t hits2, double time2, double rate2) {
        
        std::cout << "\n" << std::string(70, '=') << std::endl;
        std::cout << "  РЕЗУЛЬТАТЫ БЕНЧМАРКА (размер кэша = " << cache_size << ")" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        
        std::cout << std::left << std::setw(20) << "Алгоритм" 
                  << std::setw(15) << "Хиты" 
                  << std::setw(15) << "Промахи"
                  << std::setw(15) << "Hit Rate %" 
                  << std::setw(15) << "Время (мс)" 
                  << std::endl;
        
        std::cout << std::string(70, '-') << std::endl;
        
        // LRU
        std::cout << std::left << std::setw(20) << algo1
                  << std::setw(15) << hits1
                  << std::setw(15) << (trace_.size() - hits1)
                  << std::setw(15) << std::fixed << std::setprecision(2) << rate1
                  << std::setw(15) << std::setprecision(1) << time1
                  << std::endl;
        
        // LFU
        std::cout << std::left << std::setw(20) << algo2
                  << std::setw(15) << hits2
                  << std::setw(15) << (trace_.size() - hits2)
                  << std::setw(15) << std::fixed << std::setprecision(2) << rate2
                  << std::setw(15) << std::setprecision(1) << time2
                  << std::endl;
        
        std::cout << std::string(70, '=') << std::endl;
        
        // Сравнение
        std::cout << "\nСРАВНЕНИЕ:" << std::endl;
        std::cout << "  " << algo2 << " лучше " << algo1 << " на: " 
                  << std::fixed << std::setprecision(2) << (rate2 - rate1) 
                  << "% (" << std::setprecision(1) << ((rate2 / rate1 - 1) * 100) 
                  << "% улучшения)" << std::endl;
        
        if (time2 < time1) {
            std::cout << "  " << algo2 << " быстрее " << algo1 << " на: "
                      << std::fixed << std::setprecision(1) << (time1 - time2) 
                      << " мс (" << std::setprecision(1) << ((1 - time2/time1) * 100)
                      << "% быстрее)" << std::endl;
        } else {
            std::cout << "  " << algo1 << " быстрее " << algo2 << " на: "
                      << std::fixed << std::setprecision(1) << (time2 - time1) 
                      << " мс (" << std::setprecision(1) << ((1 - time1/time2) * 100)
                      << "% быстрее)" << std::endl;
        }
    }

public:
    // Основной метод запуска бенчмарка
    void run(const std::string& trace_file, const std::vector<size_t>& cache_sizes) {
        std::cout << "🚀 ЗАПУСК БЕНЧМАРКА LRU vs LFU" << std::endl;
        std::cout << "Трасса: " << trace_file << std::endl;
        
        if (!load_trace(trace_file)) {
            return;
        }
        
        size_t total_requests = trace_.size();
        std::cout << "Всего запросов: " << total_requests << std::endl;
        
        // Запускаем для каждого размера кэша
        for (size_t cache_size : cache_sizes) {
            std::cout << "\n📊 Тестирование с размером кэша = " << cache_size << " ..." << std::endl;
            
            // Тест LRU
            size_t lru_hits;
            double lru_time;
            run_lru_test(cache_size, lru_hits, lru_time);
            double lru_rate = 100.0 * lru_hits / total_requests;
            
            // Тест LFU
            size_t lfu_hits;
            double lfu_time;
            run_lfu_test(cache_size, lfu_hits, lfu_time);
            double lfu_rate = 100.0 * lfu_hits / total_requests;
            
            // Выводим результаты
            print_results_table(cache_size,
                              "LRU", lru_hits, lru_time, lru_rate,
                              "LFU", lfu_hits, lfu_time, lfu_rate);
        }
        
        // Дополнительно: сравнение при разных размерах
        std::cout << "\n\nСРАВНЕНИЕ ПРИ РАЗНЫХ РАЗМЕРАХ КЭША:" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        std::cout << std::left << std::setw(10) << "Размер" 
                  << std::setw(15) << "LRU Hit %" 
                  << std::setw(15) << "LFU Hit %"
                  << std::setw(20) << "Преимущество LFU" 
                  << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (size_t cache_size : cache_sizes) {
            size_t lru_hits, lfu_hits;
            double lru_time, lfu_time;
            
            run_lru_test(cache_size, lru_hits, lru_time);
            run_lfu_test(cache_size, lfu_hits, lfu_time);
            
            double lru_rate = 100.0 * lru_hits / total_requests;
            double lfu_rate = 100.0 * lfu_hits / total_requests;
            double advantage = lfu_rate - lru_rate;
            
            std::cout << std::left << std::setw(10) << cache_size
                      << std::setw(15) << std::fixed << std::setprecision(2) << lru_rate
                      << std::setw(15) << lfu_rate
                      << std::setw(20) << std::setprecision(2) << advantage << "%"
                      << std::endl;
        }
    }
    
    // Быстрый тест как в задании
    void quick_test(const std::string& trace_file, size_t cache_size = 1000) {
        std::cout << "\n🔬 БЫСТРЫЙ ТЕСТ (как в задании):" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        if (!load_trace(trace_file)) {
            return;
        }
        
        // Тест LRU
        cache_t<int> lfu_cache(cache_size);
        LRUCache<int> lru_cache(cache_size);
        
        int lru_hits = 0, lfu_hits = 0;
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int key : trace_) {
            if (lru_cache.check_to_hit(key)) lru_hits++;
        }
        auto lru_end = std::chrono::high_resolution_clock::now();
        
        for (int key : trace_) {
            if (lfu_cache.check_to_hit(key)) lfu_hits++;
        }
        auto lfu_end = std::chrono::high_resolution_clock::now();
        
        auto lru_time = std::chrono::duration_cast<std::chrono::milliseconds>(lru_end - start);
        auto lfu_time = std::chrono::duration_cast<std::chrono::milliseconds>(lfu_end - lru_end);
        
        double lru_rate = 100.0 * lru_hits / trace_.size();
        double lfu_rate = 100.0 * lfu_hits / trace_.size();
        
        std::cout << "LRU  (size=" << cache_size << "): hit rate = " 
                  << std::fixed << std::setprecision(1) << lru_rate 
                  << "% (" << lru_time.count() << " ms)" << std::endl;
        
        std::cout << "LFU  (size=" << cache_size << "): hit rate = " 
                  << std::fixed << std::setprecision(1) << lfu_rate 
                  << "% (" << lfu_time.count() << " ms)" << std::endl;
        
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "LFU лучше LRU на: " << std::fixed << std::setprecision(2) 
                  << (lfu_rate - lru_rate) << "%" << std::endl;
    }
};