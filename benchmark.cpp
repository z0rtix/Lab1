#include "benchmark.hpp"
#include "AVLTree.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>


void runBenchmark() {
    std::cout << "\n======================================================================\n";
    std::cout << "                 НАГРУЗОЧНОЕ ТЕСТИРОВАНИЕ ПРОИЗВОДИТЕЛЬНОСТИ          \n";
    std::cout << "======================================================================\n";
    std::cout << "  N элементов  |   Вставка (ms)   |  Поиск 100k (ms) |    Статус      \n";
    std::cout << "---------------+------------------+------------------+----------------\n";

    size_t testSizes[] = {10000, 50000, 100000, 500000, 1000000};
    std::mt19937_64 rng(1337);
    std::uniform_int_distribution<int> dist(-2000000000, 2000000000);

    for (size_t idx = 0; idx < 5; idx++) {
        size_t n = testSizes[idx];
        std::vector<int> data(n);
        for (size_t i = 0; i < n; i++) {
            data[i] = dist(rng);
        }

        AVLTree<int> tree;

        auto t0 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n; i++) {
            tree.insert(data[i]);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto insertMs = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

        const size_t searchQueries = 100000;
        volatile size_t hits = 0;

        auto t2 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < searchQueries; i++) {
            if (tree.contains(data[i % n])) {
                hits = hits + 1;
            }
        }
        auto t3 = std::chrono::high_resolution_clock::now();
        auto searchMs = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();

        std::cout << "  " << std::setw(12) << std::left << n
                  << " |  " << std::setw(15) << std::left << insertMs
                  << " |  " << std::setw(15) << std::left << searchMs
                  << " |  \033[32mOK (hits: " << hits << ")\033[0m\n";
    }

    std::cout << "======================================================================\n\n";
}