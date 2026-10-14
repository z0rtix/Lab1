#include "menu.hpp"
#include "AVLTree.hpp"
#include "Set.hpp"
#include "Person.hpp"

#include <iostream>
#include <string>


static void treeMenuInt() {
    AVLTree<int> tree;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n----------------------------------------\n";
        std::cout << "       РЕЖИМ: АВЛ-ДЕРЕВО (ЧИСЛА INT)    \n";
        std::cout << "----------------------------------------\n";
        std::cout << "1. Вставить число\n";
        std::cout << "2. Удалить число\n";
        std::cout << "3. Проверить наличие числа\n";
        std::cout << "4. Вывести дерево псевдографикой\n";
        std::cout << "5. Вывести все 6 порядков обхода\n";
        std::cout << "6. Применить map (умножить все на 2)\n";
        std::cout << "7. Применить where (оставить только > 0)\n";
        std::cout << "8. Применить reduce (посчитать сумму)\n";
        std::cout << "9. Сериализация в строку и десериализация\n";
        std::cout << "10. Заполнить тестовыми числами автоматически\n";
        std::cout << "0. Назад в главное меню\n";
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            std::cout << "Введите число для вставки: ";
            int val;
            std::cin >> val;
            if (tree.insert(val)) {
                std::cout << "-> Число " << val << " успешно добавлено.\n";
            } else {
                std::cout << "-> Элемент " << val << " уже присутствует в дереве.\n";
            }
        } else if (choice == 2) {
            std::cout << "Введите число для удаления: ";
            int val;
            std::cin >> val;
            if (tree.remove(val)) {
                std::cout << "-> Элемент " << val << " удален.\n";
            } else {
                std::cout << "-> Элемент не найден.\n";
            }
        } else if (choice == 3) {
            std::cout << "Введите число для поиска: ";
            int val;
            std::cin >> val;
            std::cout << (tree.contains(val) ? "-> Найдено!\n" : "-> Отсутствует.\n");
        } else if (choice == 4) {
            std::cout << "\nСтруктура дерева:\n";
            tree.print(std::cout);
            std::cout << "Всего узлов: " << tree.size() << "\n";
        } else if (choice == 5) {
            std::cout << "\nКЛП: "; tree.forEachKLP([](int x) { std::cout << x << " "; });
            std::cout << "\nКПЛ: "; tree.forEachKPL([](int x) { std::cout << x << " "; });
            std::cout << "\nЛКП: "; tree.forEachLKP([](int x) { std::cout << x << " "; });
            std::cout << "\nЛПК: "; tree.forEachLPK([](int x) { std::cout << x << " "; });
            std::cout << "\nПЛК: "; tree.forEachPLK([](int x) { std::cout << x << " "; });
            std::cout << "\nПКЛ: "; tree.forEachPKL([](int x) { std::cout << x << " "; });
            std::cout << "\n";
        } else if (choice == 6) {
            try {
                auto doubled = tree.map([](int x) { return x * 2; });
                std::cout << "\nРезультат map (*2):\n";
                doubled.print(std::cout);
            } catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        } else if (choice == 7) {
            auto positiveOnly = tree.where([](int x) { return x > 0; });
            std::cout << "\nРезультат where (> 0):\n";
            positiveOnly.print(std::cout);
        } else if (choice == 8) {
            int sum = tree.reduce([](int val, int acc) { return acc + val; }, 0);
            std::cout << "-> Сумма всех элементов дерева: " << sum << "\n";
        } else if (choice == 9) {
            std::string serialized = tree.serialize();
            std::cout << "-> Сериализованная строка: " << serialized << "\n";
            AVLTree<int> restored;
            restored.deserialize(serialized);
            std::cout << "-> Восстановленное дерево:\n";
            restored.print(std::cout);
        } else if (choice == 10) {
            int demo[] = {50, 20, 70, 10, 30, 60, 80};
            for (int i = 0; i < 7; i++) {
                tree.insert(demo[i]);
            }
            std::cout << "-> Добавлены элементы {50, 20, 70, 10, 30, 60, 80}.\n";
        }
    }
}

static void treeMenuStudent() {
    AVLTree<Student> tree;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n----------------------------------------\n";
        std::cout << "     РЕЖИМ: АВЛ-ДЕРЕВО (СТУДЕНТЫ)       \n";
        std::cout << "----------------------------------------\n";
        std::cout << "1. Добавить студента вручную\n";
        std::cout << "2. Загрузить демо-список студентов\n";
        std::cout << "3. Вывести дерево студентов\n";
        std::cout << "4. Вывести список студентов по порядку ID\n";
        std::cout << "5. Фильтр отличников (GPA >= 4.5 через where)\n";
        std::cout << "0. Назад в главное меню\n";
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            int series, number;
            std::string first, last;
            double gpa;
            std::cout << "Серия зачетки: "; std::cin >> series;
            std::cout << "Номер зачетки: "; std::cin >> number;
            std::cout << "Имя: "; std::cin >> first;
            std::cout << "Фамилия: "; std::cin >> last;
            std::cout << "Средний балл: "; std::cin >> gpa;
            tree.insert(Student({series, number}, first, last, gpa));
            std::cout << "-> Студент добавлен.\n";
        } else if (choice == 2) {
            tree.insert(Student({4510, 100200}, "Иван", "Иванов", 4.6));
            tree.insert(Student({4510, 100100}, "Петр", "Петров", 3.8));
            tree.insert(Student({4510, 100300}, "Анна", "Сидорова", 4.9));
            tree.insert(Student({4510, 100050}, "Ольга", "Смирнова", 4.2));
            std::cout << "-> Загружены 4 студента.\n";
        } else if (choice == 3) {
            std::cout << "\nДерево студентов:\n";
            tree.print(std::cout);
        } else if (choice == 4) {
            std::cout << "\nСтуденты по ID:\n";
            tree.forEachLKP([](const Student& s) {
                std::cout << s << "\n";
            });
        } else if (choice == 5) {
            std::cout << "\nОтличники (GPA >= 4.5):\n";
            auto honors = tree.where([](const Student& s) {
                return s.getGPA() >= 4.5;
            });
            honors.forEachLKP([](const Student& s) {
                std::cout << s << "\n";
            });
        }
    }
}

static void setMenuDemo() {
    Set<int> setA;
    Set<int> setB;

    int arrA[] = {1, 2, 3, 4, 5};
    int arrB[] = {4, 5, 6, 7, 8};
    for (int i = 0; i < 5; i++) {
        setA.insert(arrA[i]);
        setB.insert(arrB[i]);
    }

    std::cout << "\n----------------------------------------\n";
    std::cout << "        ДЕМОНСТРАЦИЯ МНОЖЕСТВ (SET)     \n";
    std::cout << "----------------------------------------\n";
    std::cout << "Множество A: ";
    setA.forEach([](int x) { std::cout << x << " "; });
    std::cout << "\nМножество B: ";
    setB.forEach([](int x) { std::cout << x << " "; });

    Set<int> un = setA.unionWith(setB);
    std::cout << "\n\nОбъединение (A ∪ B): ";
    un.forEach([](int x) { std::cout << x << " "; });

    Set<int> inter = setA.intersectWith(setB);
    std::cout << "\nПересечение (A ∩ B): ";
    inter.forEach([](int x) { std::cout << x << " "; });

    Set<int> diff = setA.differenceWith(setB);
    std::cout << "\nРазность (A \\ B):   ";
    diff.forEach([](int x) { std::cout << x << " "; });
    std::cout << "\n";
}

void runInteractiveMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "             ГЛАВНОЕ МЕНЮ               \n";
        std::cout << "========================================\n";
        std::cout << "1. Работа с АВЛ-деревом (числа)\n";
        std::cout << "2. Работа с АВЛ-деревом (студенты)\n";
        std::cout << "3. Операции над Множествами (Set)\n";
        std::cout << "0. Выход из программы\n";
        std::cout << "Выберите пункт: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            treeMenuInt();
        } else if (choice == 2) {
            treeMenuStudent();
        } else if (choice == 3) {
            setMenuDemo();
        }
    }
}