#include "benchmark.hpp"
#include "AVLTree.hpp"

#include <iostream>
#include <chrono>
#include <cstdlib>


static void runSingleBenchmark(size_t count) {
	int* values = new int[count];
	std::srand(42);
	for (size_t i = 0; i < count; i++) {
		values[i] = std::rand() % static_cast<int>(count * 10);
	}

	AVLTree<int> tree;

	auto startInsert = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < count; i++) {
		tree.insert(values[i]);
	}
	auto endInsert = std::chrono::high_resolution_clock::now();
	auto insertMs = std::chrono::duration_cast<std::chrono::milliseconds>(endInsert - startInsert).count();

	size_t foundCount = 0;
	auto startSearch = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < count; i++) {
		if (tree.contains(values[i])) {
			foundCount++;
		}
	}
	auto endSearch = std::chrono::high_resolution_clock::now();
	auto searchMs = std::chrono::duration_cast<std::chrono::milliseconds>(endSearch - startSearch).count();

	std::cout << "Элементов: " << count
	          << " | Вставка: " << insertMs << " мс"
	          << " | Поиск: " << searchMs << " мс"
	          << " | Найдено: " << foundCount << "\n";

	delete[] values;
}


void runBenchmarks() {
	std::cout << "\n======================================================\n";
	std::cout << "             ЗАМЕРЫ ПРОИЗВОДИТЕЛЬНОСТИ                \n";
	std::cout << "======================================================\n";

	runSingleBenchmark(10000);
	runSingleBenchmark(50000);
	runSingleBenchmark(100000);
	runSingleBenchmark(500000);
	runSingleBenchmark(1000000);

	std::cout << "------------------------------------------------------\n\n";
}