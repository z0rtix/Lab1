#include "menu.hpp"
#include "AVLTree.hpp"
#include "Set.hpp"
#include "Person.hpp"
#include "tests.hpp"
#include "benchmark.hpp"
#include <iostream>
#include <limits>

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const char* prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            return val;
        }
        std::cout << "Ошибка ввода! Введите целое число.\n";
        clearInput();
    }
}

static void demoStudents() {
    std::cout << "\n--- Демонстрация дерева студентов (Student) ---\n";
    AVLTree<Student> students;
    students.insert(Student({101, 1}, "Alexey", "Smirnov", 4.2));
    students.insert(Student({101, 2}, "Maria", "Kuznetsova", 4.9));
    students.insert(Student({101, 3}, "Dmitry", "Popov", 3.5));
    students.insert(Student({101, 4}, "Elena", "Sokolova", 4.7));

    std::cout << "Все студенты (обход ЛКП по возрастанию ID):\n";
    students.forEachLKP([](const Student& s) {
        std::cout << "  " << s << "\n";
    });

    std::cout << "\nСтуденты с баллом >= 4.5 (метод where):\n";
    AVLTree<Student> best = students.where([](const Student& s) {
        return s.getGPA() >= 4.5;
    });
    best.forEachLKP([](const Student& s) {
        std::cout << "  " << s << "\n";
    });
}

static void demoSets() {
    std::cout << "\n--- Демонстрация операций над множествами (Set) ---\n";
    Set<int> a;
    Set<int> b;

    a.insert(1); a.insert(2); a.insert(3); a.insert(4);
    b.insert(3); b.insert(4); b.insert(5); b.insert(6);

    std::cout << "Множество A: ";
    a.forEach([](int x) { std::cout << x << " "; });
    std::cout << "\nМножество B: ";
    b.forEach([](int x) { std::cout << x << " "; });

    Set<int> un = a.unionWith(b);
    std::cout << "\nОбъединение (A U B): ";
    un.forEach([](int x) { std::cout << x << " "; });

    Set<int> inter = a.intersectWith(b);
    std::cout << "\nПересечение (A n B): ";
    inter.forEach([](int x) { std::cout << x << " "; });

    Set<int> diff = a.differenceWith(b);
    std::cout << "\nРазность (A \\ B): ";
    diff.forEach([](int x) { std::cout << x << " "; });
    std::cout << "\n";
}

void runMenu() {
    AVLTree<int> tree;

    while (true) {
        std::cout << "\n================ МЕНЮ (АВЛ-ДЕРЕВО) ================\n";
        std::cout << "1. Вставить число в дерево\n";
        std::cout << "2. Удалить число из дерева\n";
        std::cout << "3. Проверить наличие числа\n";
        std::cout << "4. Показать дерево и обходы (ЛКП, КЛП)\n";
        std::cout << "5. Применить map (умножить на 2) и where (только четные)\n";
        std::cout << "6. Демонстрация работы со студентами (Student)\n";
        std::cout << "7. Демонстрация работы с множествами (Set)\n";
        std::cout << "8. Запустить модульные тесты\n";
        std::cout << "9. Запустить бенчмарк (замеры времени)\n";
        std::cout << "0. Выход\n";

        int choice = readInt("Выберите пункт: ");

        if (choice == 0) {
            std::cout << "Выход из программы.\n";
            break;
        } else if (choice == 1) {
            int x = readInt("Введите число для вставки: ");
            if (tree.insert(x)) std::cout << "Элемент добавлен.\n";
            else std::cout << "Такой элемент уже есть в дереве.\n";
        } else if (choice == 2) {
            int x = readInt("Введите число для удаления: ");
            if (tree.remove(x)) std::cout << "Элемент удален.\n";
            else std::cout << "Элемент не найден.\n";
        } else if (choice == 3) {
            int x = readInt("Введите число для поиска: ");
            if (tree.contains(x)) std::cout << "Элемент НАЙДЕН в дереве.\n";
            else std::cout << "Элемент НЕ найден.\n";
        } else if (choice == 4) {
            std::cout << "\nСтруктура дерева (повернуто набок, корень слева):\n";
            tree.print();
            std::cout << "Обход ЛКП (по возрастанию): ";
            tree.forEachLKP([](int x) { std::cout << x << " "; });
            std::cout << "\nОбход КЛП (корень-лево-право): ";
            tree.forEachKLP([](int x) { std::cout << x << " "; });
            std::cout << "\nРазмер дерева: " << tree.size() << "\n";
        } else if (choice == 5) {
            if (tree.empty()) {
                std::cout << "Дерево пусто! Сначала добавьте элементы.\n";
            } else {
                AVLTree<int> mapped = tree.map([](int x) { return x * 2; });
                std::cout << "После map (x * 2): ";
                mapped.forEachLKP([](int x) { std::cout << x << " "; });

                AVLTree<int> evens = tree.where([](int x) { return x % 2 == 0; });
                std::cout << "\nПосле where (четные): ";
                evens.forEachLKP([](int x) { std::cout << x << " "; });

                int sum = tree.reduce([](int val, int acc) { return val + acc; }, 0);
                std::cout << "\nСумма элементов (reduce): " << sum << "\n";
            }
        } else if (choice == 6) {
            demoStudents();
        } else if (choice == 7) {
            demoSets();
        } else if (choice == 8) {
            runAllTests();
        } else if (choice == 9) {
            runBenchmarks();
        } else {
            std::cout << "Неверный пункт меню.\n";
        }
    }
}