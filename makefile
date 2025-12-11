CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET = lfu
TEST_TARGET = lfu_test
BENCHMARK_TARGET = benchmark
LRU_TARGET = lru_demo
TRACE_GENERATOR = trace_generator.py

# Основные цели
all: $(TARGET) $(BENCHMARK_TARGET)

# Основная программа LFU (ваша оригинальная)
$(TARGET): lfu.cpp hash.h ideal_cache.h
	$(CXX) $(CXXFLAGS) lfu.cpp -o $(TARGET)

# Тесты
$(TEST_TARGET): tests.cpp hash.h ideal_cache.h
	$(CXX) $(CXXFLAGS) tests.cpp -o $(TEST_TARGET)

# Бенчмарк LRU vs LFU
$(BENCHMARK_TARGET): main_benchmark.cpp benchmark.cpp lru_cache.h hash.h
	$(CXX) $(CXXFLAGS) main_benchmark.cpp -o $(BENCHMARK_TARGET)

# Демо программа для LRU
$(LRU_TARGET): main_lru.cpp lru_cache.h
	$(CXX) $(CXXFLAGS) main_lru.cpp -o $(LRU_TARGET)

# Запуск основной программы LFU
run: $(TARGET)
	./$(TARGET)

# Запуск с тестовым файлом
run-file: $(TARGET)
ifndef TEST
	@echo "Укажите тестовый файл: make run-file TEST=файл.dat"
	@exit 1
endif
	@echo "Запуск на файле: $(TEST)"
	./$(TARGET) < $(TEST)

# Запуск тестов
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Генерация трассы для бенчмарка
generate-trace:
	@echo "Генерация тестовой трассы..."
	@if command -v python3 >/dev/null 2>&1; then \
		python3 $(TRACE_GENERATOR); \
	elif command -v python >/dev/null 2>&1; then \
		python $(TRACE_GENERATOR); \
	else \
		echo "Ошибка: Python не найден"; \
		exit 1; \
	fi

# Быстрый бенчмарк (только size=1000)
quick-benchmark: $(BENCHMARK_TARGET)
	@if [ ! -f trace.txt ]; then \
		echo "Трасса не найдена, генерирую..."; \
		$(MAKE) generate-trace; \
	fi
	./$(BENCHMARK_TARGET) --quick

# Полный бенчмарк со всеми размерами кэша
full-benchmark: $(BENCHMARK_TARGET)
	@if [ ! -f trace.txt ]; then \
		echo "Трасса не найдена, генерирую..."; \
		$(MAKE) generate-trace; \
	fi
	./$(BENCHMARK_TARGET)

# Бенчмарк с конкретным файлом трассы
benchmark-file: $(BENCHMARK_TARGET)
ifndef TRACE
	@echo "Укажите файл трассы: make benchmark-file TRACE=файл.txt"
	@exit 1
endif
	./$(BENCHMARK_TARGET) --trace $(TRACE)

# Запуск демо LRU
run-lru: $(LRU_TARGET)
	./$(LRU_TARGET)

# Все тесты: обычные тесты + бенчмарк
test-all: test full-benchmark

# Полный цикл: генерация трассы, компиляция, все тесты
all-in-one: generate-trace all test-all

# Очистка
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(BENCHMARK_TARGET) $(LRU_TARGET) \
	      trace.txt benchmark_results.csv *.o

# Справка
help:
	@echo "Доступные команды:"
	@echo "  make                    - Скомпилировать все программы"
	@echo "  make run                - Запустить основную программу LFU"
	@echo "  make run-file TEST=файл - Запустить LFU на тестовом файле"
	@echo "  make test               - Запустить модульные тесты"
	@echo ""
	@echo "  make generate-trace     - Сгенерировать тестовую трассу"
	@echo "  make quick-benchmark    - Быстрый бенчмарк (size=1000)"
	@echo "  make full-benchmark     - Полный бенчмарк"
	@echo "  make benchmark-file TRACE=файл - Бенчмарк с указанной трассой"
	@echo ""
	@echo "  make run-lru            - Запустить демо LRU"
	@echo "  make test-all           - Все тесты (модульные + бенчмарк)"
	@echo "  make all-in-one         - Полный цикл тестирования"
	@echo "  make clean              - Очистка"
	@echo "  make help               - Эта справка"

.PHONY: all run run-file test generate-trace quick-benchmark full-benchmark \
        benchmark-file run-lru test-all all-in-one clean help