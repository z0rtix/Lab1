#include "tests.hpp"
#include "AVLTree.hpp"
#include "Set.hpp"
#include "Person.hpp"
#include <iostream>
#include <cassert>
#include <stdexcept>

static int totalTests = 0;

static void checkInsertAndBalance() {
    AVLTree<int> tree;
    for (int i = 1; i <= 15; i++) {
        assert(tree.insert(i) == true);
    }
    assert(tree.size() == 15);
    assert(tree.insert(5) == false);
    assert(tree.size() == 15);
    assert(tree.contains(1));
    assert(tree.contains(8));
    assert(tree.contains(15));
    assert(!tree.contains(16));
    assert(!tree.contains(0));
}

static void checkRemove() {
    AVLTree<int> tree;
    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(10);
    tree.insert(30);
    tree.insert(60);
    tree.insert(90);

    assert(tree.remove(10) == true);
    assert(!tree.contains(10));
    assert(tree.size() == 6);

    assert(tree.remove(25) == true);
    assert(!tree.contains(25));
    assert(tree.contains(30));
    assert(tree.size() == 5);

    assert(tree.remove(50) == true);
    assert(!tree.contains(50));
    assert(tree.contains(60));
    assert(tree.contains(75));
    assert(tree.size() == 4);

    assert(tree.remove(999) == false);
    assert(tree.size() == 4);
}

static void checkTraversals() {
    AVLTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);

    int res[3];
    int idx = 0;

    idx = 0;
    tree.forEachKLP([&](int x) { res[idx++] = x; });
    assert(res[0] == 20 && res[1] == 10 && res[2] == 30);

    idx = 0;
    tree.forEachKPL([&](int x) { res[idx++] = x; });
    assert(res[0] == 20 && res[1] == 30 && res[2] == 10);

    idx = 0;
    tree.forEachLKP([&](int x) { res[idx++] = x; });
    assert(res[0] == 10 && res[1] == 20 && res[2] == 30);

    idx = 0;
    tree.forEachLPK([&](int x) { res[idx++] = x; });
    assert(res[0] == 10 && res[1] == 30 && res[2] == 20);

    idx = 0;
    tree.forEachPLK([&](int x) { res[idx++] = x; });
    assert(res[0] == 30 && res[1] == 10 && res[2] == 20);

    idx = 0;
    tree.forEachPKL([&](int x) { res[idx++] = x; });
    assert(res[0] == 30 && res[1] == 20 && res[2] == 10);
}

static void checkFunctional() {
    AVLTree<int> tree;
    for (int i = 1; i <= 5; i++) {
        tree.insert(i);
    }

    AVLTree<int> mapped = tree.map([](int x) { return x * 3; });
    assert(mapped.size() == 5);
    assert(mapped.contains(3));
    assert(mapped.contains(15));
    assert(!mapped.contains(1));

    AVLTree<int> filtered = tree.where([](int x) { return x % 2 != 0; });
    assert(filtered.size() == 3);
    assert(filtered.contains(1));
    assert(filtered.contains(3));
    assert(filtered.contains(5));
    assert(!filtered.contains(2));

    int sum = tree.reduce([](int val, int acc) { return acc + val; }, 0);
    assert(sum == 15);
}

static void checkSubtrees() {
    AVLTree<int> tree;
    tree.insert(40);
    tree.insert(20);
    tree.insert(60);
    tree.insert(10);
    tree.insert(30);

    AVLTree<int> sub = tree.extractSubtree(20);
    assert(sub.size() == 3);
    assert(sub.contains(20));
    assert(sub.contains(10));
    assert(sub.contains(30));
    assert(tree.containsSubtree(sub) == true);

    AVLTree<int> other;
    other.insert(20);
    other.insert(10);
    other.insert(99);
    assert(tree.containsSubtree(other) == false);
}

static void checkSet() {
    Set<int> a;
    Set<int> b;

    for (int i = 1; i <= 5; i++) a.insert(i);
    for (int i = 4; i <= 8; i++) b.insert(i);

    Set<int> un = a.unionWith(b);
    assert(un.size() == 8);

    Set<int> inter = a.intersectWith(b);
    assert(inter.size() == 2);
    assert(inter.contains(4));
    assert(inter.contains(5));
    assert(!inter.contains(3));

    Set<int> diff = a.differenceWith(b);
    assert(diff.size() == 3);
    assert(diff.contains(1));
    assert(diff.contains(2));
    assert(diff.contains(3));
    assert(!diff.contains(4));

    Set<int> sub;
    sub.insert(1);
    sub.insert(2);
    assert(sub.isSubsetOf(a) == true);
    assert(a.isSubsetOf(sub) == false);
}

static void checkStudents() {
    AVLTree<Student> tree;
    Student s1({1, 10}, "Ivan", "Ivanov", 4.5);
    Student s2({1, 5}, "Petr", "Petrov", 3.8);
    Student s3({1, 20}, "Anna", "Sidorova", 4.9);

    assert(tree.insert(s1) == true);
    assert(tree.insert(s2) == true);
    assert(tree.insert(s3) == true);
    assert(tree.size() == 3);
    assert(tree.contains(s1));

    AVLTree<Student> honors = tree.where([](const Student& s) { return s.getGPA() >= 4.5; });
    assert(honors.size() == 2);
}

static void checkCopyMoveAndExceptions() {
    AVLTree<int> emptyTree;
    assert(emptyTree.empty() == true);
    assert(emptyTree.size() == 0);
    assert(emptyTree.contains(10) == false);
    assert(emptyTree.remove(10) == false);

    bool mapError = false;
    try {
        emptyTree.map([](int x) { return x * 2; });
    } catch (const std::runtime_error&) {
        mapError = true;
    }
    assert(mapError == true);

    bool subError = false;
    try {
        emptyTree.extractSubtree(42);
    } catch (const std::invalid_argument&) {
        subError = true;
    }
    assert(subError == true);

    AVLTree<int> t1;
    t1.insert(10);
    t1.insert(20);

    AVLTree<int> t2 = t1;
    assert(t2.size() == 2);
    assert(t2.contains(10));

    AVLTree<int> t3 = std::move(t1);
    assert(t1.empty() == true);
    assert(t1.size() == 0);
    assert(t3.size() == 2);
    assert(t3.contains(20));
}

static void runTest(const char* name, void (*func)()) {
    func();
    totalTests++;
    std::cout << "\033[32m[✓]\033[0m " << name << "\n";
}

void runAllTests() {
    std::cout << "\n========================================\n";
    std::cout << "         ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ        \n";
    std::cout << "========================================\n";
    totalTests = 0;

    runTest("Вставка элементов и балансировка", checkInsertAndBalance);
    runTest("Удаление листа, узла с 1 и 2 детьми", checkRemove);
    runTest("Все 6 вариантов обхода дерева", checkTraversals);
    runTest("Методы map, where, reduce", checkFunctional);
    runTest("Извлечение и поиск поддерева", checkSubtrees);
    runTest("Множество Set: объединение, пересечение, разность", checkSet);
    runTest("Хранение и фильтрация типа Student", checkStudents);
    runTest("Копирование, перемещение и исключения", checkCopyMoveAndExceptions);

    std::cout << "\033[32mВсе " << totalTests << " тестов успешно пройдены!\033[0m\n\n";
}