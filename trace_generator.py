import numpy as np
import sys

def generate_trace(filename="trace.txt", num_requests=100000, num_unique_keys=10000, zipf_param=1.1):
    """
    Генерирует трассу запросов с распределением Ципфа
    """
    print(f"Генерация трассы {filename}...")
    print(f"  Запросов: {num_requests:,}")
    print(f"  Уникальных ключей: {num_unique_keys:,}")
    print(f"  Параметр Ципфа: {zipf_param}")
    
    # Генерация ключей с распределением Ципфа
    keys = np.random.zipf(a=zipf_param, size=num_requests)
    keys = keys % num_unique_keys  # Ограничиваем диапазон
    
    # Сохраняем в файл
    np.savetxt(filename, keys, fmt="%d")
    
    # Анализ распределения
    unique, counts = np.unique(keys, return_counts=True)
    print(f"\nАнализ распределения:")
    print(f"  Самые популярные ключи:")
    top_indices = np.argsort(counts)[-5:][::-1]
    for i, idx in enumerate(top_indices):
        print(f"    {i+1}. Ключ {unique[idx]}: {counts[idx]} запросов ({counts[idx]/num_requests*100:.2f}%)")
    
    print(f"\nТрасса сохранена в {filename}")
    return len(unique)

if __name__ == "__main__":
    # Можно передать параметры через аргументы командной строки
    if len(sys.argv) > 1:
        filename = sys.argv[1]
        num_requests = int(sys.argv[2]) if len(sys.argv) > 2 else 100000
        num_unique_keys = int(sys.argv[3]) if len(sys.argv) > 3 else 10000
        zipf_param = float(sys.argv[4]) if len(sys.argv) > 4 else 1.1
    else:
        filename = "trace.txt"
        num_requests = 100000
        num_unique_keys = 10000
        zipf_param = 1.1
    
    generate_trace(filename, num_requests, num_unique_keys, zipf_param)