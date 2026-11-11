#include "benchmark.hpp"
#include "AVLTree.hpp"

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <string>


struct BenchmarkRow {
	size_t elements;
	long long insertMs;
	long long removeMs;
	long long avlSearchMs;
	long long arraySearchMs;
};

static bool linearSearchInArray(const int* arr, size_t size, int target) {
	for (size_t i = 0; i < size; i++) {
		if (arr[i] == target) return true;
	}
	return false;
}

static void printPaddedCell(long long value, int width) {
	std::string text = std::to_string(value);
	std::cout << "| " << text;
	int spaces = width - static_cast<int>(text.length()) - 1;
	for (int i = 0; i < spaces; i++) {
		std::cout << " ";
	}
}


void runBenchmarks() {
	std::cout << "\n\033[36m========================================================================================\033[0m\n";
	std::cout << "\033[36m             БЕНЧМАРК: АВЛ-ДЕРЕВО O(log N) ПРОТИВ ЛИНЕЙНОГО МАССИВА O(N)                \033[0m\n";
	std::cout << "\033[36m========================================================================================\033[0m\n";
	std::cout << "Выполняется замер времени на объемах от 25 000 до 1 000 000 элементов...\n\n";

	{
		AVLTree<int> warmupTree;
		for (int i = 0; i < 20000; i++) warmupTree.insert(i);
	}

	const int stepsCount = 5;
	size_t sizes[stepsCount] = {25000, 50000, 100000, 500000, 1000000};
	BenchmarkRow rows[stepsCount];

	for (int step = 0; step < stepsCount; step++) {
		size_t count = sizes[step];
		int* values = new int[count];
		std::srand(42);
		for (size_t i = 0; i < count; i++) {
			values[i] = (std::rand() * 32768 + std::rand()) % static_cast<int>(count * 10);
		}

		AVLTree<int> tree;

		auto startIns = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < count; i++) {
			tree.insert(values[i]);
		}
		auto endIns = std::chrono::high_resolution_clock::now();
		long long insTime = std::chrono::duration_cast<std::chrono::milliseconds>(endIns - startIns).count();

		size_t searchQueries = 3000;
		volatile size_t foundAvl = 0;
		auto startAvlSrch = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < searchQueries; i++) {
			size_t idx = (i * 997) % count;
			if (tree.contains(values[idx])) {
				foundAvl = foundAvl + 1;
			}
		}
		auto endAvlSrch = std::chrono::high_resolution_clock::now();
		long long avlSrchTime = std::chrono::duration_cast<std::chrono::microseconds>(endAvlSrch - startAvlSrch).count();

		volatile size_t foundArr = 0;
		auto startArrSrch = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < searchQueries; i++) {
			size_t idx = (i * 997) % count;
			if (linearSearchInArray(values, count, values[idx])) {
				foundArr = foundArr + 1;
			}
		}
		auto endArrSrch = std::chrono::high_resolution_clock::now();
		long long arrSrchTime = std::chrono::duration_cast<std::chrono::microseconds>(endArrSrch - startArrSrch).count();

		size_t removeQueries = count / 2;
		auto startRem = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < removeQueries; i++) {
			tree.remove(values[i]);
		}
		auto endRem = std::chrono::high_resolution_clock::now();
		long long remTime = std::chrono::duration_cast<std::chrono::milliseconds>(endRem - startRem).count();

		rows[step] = {count, insTime, remTime, avlSrchTime, arrSrchTime};
		delete[] values;
	}

	std::cout << "+---------------+-----------------+-----------------+---------------------+---------------------+\n";
	std::cout << "| Элементов (N) | Вставка N (мс)  | Удаление (мс)   | Поиск АВЛ 3k (мкс)  | Поиск Массив (мкс)  |\n";
	std::cout << "+---------------+-----------------+-----------------+---------------------+---------------------+\n";

	for (int step = 0; step < stepsCount; step++) {
		printPaddedCell(static_cast<long long>(rows[step].elements), 15);
		printPaddedCell(rows[step].insertMs, 17);
		printPaddedCell(rows[step].removeMs, 17);
		printPaddedCell(rows[step].avlSearchMs, 21);
		printPaddedCell(rows[step].arraySearchMs, 21);
		std::cout << "|\n";
	}

	std::cout << "+---------------+-----------------+-----------------+---------------------+---------------------+\n\n";
}