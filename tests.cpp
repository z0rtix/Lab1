#include "tests.hpp"
#include "AVLTree.hpp"
#include "Set.hpp"
#include "Person.hpp"

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <stdexcept>


static int totalTests = 0;

#define TEST_CASE(name, code) \
    do { \
        code; \
        totalTests++; \
        std::cout << "\033[32m[✓]\033[0m " << name << "\n"; \
    } while (0)

static void testInsertAndBalance() {
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

static void testRemoveVariations() {
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

static void testAllTraversals() {
    AVLTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);

    std::vector<int> klp;
    tree.forEachKLP([&](int x) { klp.push_back(x); });
    assert((klp == std::vector<int>{20, 10, 30}));

    std::vector<int> kpl;
    tree.forEachKPL([&](int x) { kpl.push_back(x); });
    assert((kpl == std::vector<int>{20, 30, 10}));

    std::vector<int> lkp;
    tree.forEachLKP([&](int x) { lkp.push_back(x); });
    assert((lkp == std::vector<int>{10, 20, 30}));

    std::vector<int> lpk;
    tree.forEachLPK([&](int x) { lpk.push_back(x); });
    assert((lpk == std::vector<int>{10, 30, 20}));

    std::vector<int> plk;
    tree.forEachPLK([&](int x) { plk.push_back(x); });
    assert((plk == std::vector<int>{30, 10, 20}));

    std::vector<int> pkl;
    tree.forEachPKL([&](int x) { pkl.push_back(x); });
    assert((pkl == std::vector<int>{30, 20, 10}));
}

static void testFunctionalMethods() {
    AVLTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    auto mapped = tree.map([](int x) { return x * 3; });
    assert(mapped.size() == 5);
    assert(mapped.contains(3));
    assert(mapped.contains(15));
    assert(!mapped.contains(1));

    auto filtered = tree.where([](int x) { return x % 2 != 0; });
    assert(filtered.size() == 3);
    assert(filtered.contains(1));
    assert(filtered.contains(3));
    assert(filtered.contains(5));
    assert(!filtered.contains(2));

    int sum = tree.reduce([](int val, int acc) { return acc + val; }, 0);
    assert(sum == 15);

    int product = tree.reduce([](int val, int acc) { return acc * val; }, 1);
    assert(product == 120);
}

static void testSubtreeOperations() {
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

    AVLTree<int> alien;
    alien.insert(20);
    alien.insert(10);
    alien.insert(99);
    assert(tree.containsSubtree(alien) == false);

    bool threw = false;
    try {
        tree.extractSubtree(999);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw == true);
}

static void testSerialization() {
    AVLTree<int> tree;
    tree.insert(100);
    tree.insert(50);
    tree.insert(150);
    tree.insert(25);
    tree.insert(75);

    std::string s = tree.serialize();
    assert(!s.empty());

    AVLTree<int> restored;
    restored.deserialize(s);

    assert(restored.size() == tree.size());
    assert(restored.contains(100));
    assert(restored.contains(50));
    assert(restored.contains(150));
    assert(restored.contains(25));
    assert(restored.contains(75));
    assert(tree.containsSubtree(restored) == true);
}

static void testSetMathOperations() {
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

static void testStudentComplexType() {
    AVLTree<Student> tree;
    Student s1({1, 10}, "Ivan", "Ivanov", 4.5);
    Student s2({1, 5}, "Petr", "Petrov", 3.8);
    Student s3({1, 20}, "Anna", "Sidorova", 4.9);

    assert(tree.insert(s1) == true);
    assert(tree.insert(s2) == true);
    assert(tree.insert(s3) == true);
    assert(tree.size() == 3);

    assert(tree.contains(s1));
    assert(tree.contains(s2));

    auto honors = tree.where([](const Student& s) { return s.getGPA() >= 4.5; });
    assert(honors.size() == 2);
}

void runAllTests() {
    std::cout << "\n========================================\n";
    std::cout << "         ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ        \n";
    std::cout << "========================================\n";
    totalTests = 0;

    TEST_CASE("Вставка элементов и автобалансировка", testInsertAndBalance());
    TEST_CASE("Удаление листа, узла с одним и двумя детьми", testRemoveVariations());
    TEST_CASE("Все 6 порядков обхода (КЛП, КПЛ, ЛКП, ЛПК, ПЛК, ПКЛ)", testAllTraversals());
    TEST_CASE("Функциональные методы: map, where, reduce", testFunctionalMethods());
    TEST_CASE("Извлечение поддерева, проверка вхождения и исключения", testSubtreeOperations());
    TEST_CASE("Сериализация в строку и точное восстановление", testSerialization());
    TEST_CASE("Операции над Set: объединение, пересечение, разность", testSetMathOperations());
    TEST_CASE("Работа со сложным типом Student по составному ID", testStudentComplexType());

    std::cout << "\033[32mВсе " << totalTests << " наборов тестов успешно пройдены!\033[0m\n\n";
}