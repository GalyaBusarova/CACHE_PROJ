#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <cstddef>

// LRU кэш для сравнения с LFU
template<typename T, typename KeyT = int>
class LRUCache {
private:
    struct CacheElem {
        KeyT key;
        T value;
    };

    size_t capacity_;
    std::list<CacheElem> cache_list_;
    std::unordered_map<KeyT, typename std::list<CacheElem>::iterator> hash_table_;

public:
    LRUCache(size_t capacity) : capacity_(capacity) {
        if (capacity_ == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
    }

    bool check_to_hit(KeyT key) {
        auto hit = hash_table_.find(key);
        
        if (hit != hash_table_.end()) {
            // Элемент найден - перемещаем в начало
            cache_list_.splice(cache_list_.begin(), cache_list_, hit->second);
            return true;
        }
        
        // Элемент не найден
        if (hash_table_.size() >= capacity_) {
            // Удаляем самый старый элемент
            KeyT key_to_remove = cache_list_.back().key;
            hash_table_.erase(key_to_remove);
            cache_list_.pop_back();
        }
        
        // Добавляем новый элемент
        CacheElem new_elem = {key, T()};
        cache_list_.push_front(new_elem);
        hash_table_[key] = cache_list_.begin();
        
        return false;
    }

    // Получить значение 
    T get(KeyT key) {
        auto hit = hash_table_.find(key);
        if (hit == hash_table_.end()) {
            return T();
        }
        
        // Перемещаем в начало
        cache_list_.splice(cache_list_.begin(), cache_list_, hit->second);
        return hit->second->value;
    }

    // Положить значение 
    void put(KeyT key, T value) {
        auto hit = hash_table_.find(key);
        
        if (hit != hash_table_.end()) {
            // Обновляем существующий
            hit->second->value = value;
            cache_list_.splice(cache_list_.begin(), cache_list_, hit->second);
            return;
        }
        
        if (hash_table_.size() >= capacity_) {
            // Удаляем самый старый элемент
            KeyT key_to_remove = cache_list_.back().key;
            hash_table_.erase(key_to_remove);
            cache_list_.pop_back();
        }
        
        // Добавляем новый элемент
        CacheElem new_elem = {key, value};
        cache_list_.push_front(new_elem);
        hash_table_[key] = cache_list_.begin();
    }

    bool full() const {
        return hash_table_.size() >= capacity_;
    }

    size_t size() const {
        return hash_table_.size();
    }

    void print_state() const {
        std::cout << "LRU Cache: ";
        for (const auto& elem : cache_list_) {
            std::cout << elem.key << " ";
        }
        std::cout << std::endl;
    }
};

#endif 