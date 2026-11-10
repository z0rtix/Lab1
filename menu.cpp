#include "menu.hpp"
#include "AVLTree.hpp"
#include "Set.hpp"
#include "Person.hpp"
#include "tests.hpp"
#include "benchmark.hpp"

#include <iostream>


static void clearInput() {
	std::cin.clear();
	while (std::cin.get() != '\n' && !std::cin.eof()) {}
}

static int readInt(const char* prompt) {
	int value;
	while (true) {
		std::cout << prompt;
		if (std::cin >> value) {
			clearInput();
			return value;
		}
		std::cout << "\033[31mОшибка ввода! Введите целое число.\033[0m\n";
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

	std::cout << "\nСтруктура дерева студентов:\n";
	students.print();

	std::cout << "\nСтуденты с баллом >= 4.5 (метод where):\n";
	AVLTree<Student> honors = students.where([](const Student& s) {
		return s.getGPA() >= 4.5;
	});
	honors.forEachLKP([](const Student& s) {
		std::cout << "  " << s << "\n";
	});
}

static void demoSets() {
	std::cout << "\n--- Демонстрация операций над множествами (Set) ---\n";
	Set<int> a;
	Set<int> b;

	a.insert(1); a.insert(2); a.insert(3); a.insert(4); a.insert(5);
	b.insert(4); b.insert(5); b.insert(6); b.insert(7); b.insert(8);

	std::cout << "Множество A: { ";
	a.forEach([](int x) { std::cout << x << " "; });
	std::cout << "}\nМножество B: { ";
	b.forEach([](int x) { std::cout << x << " "; });
	std::cout << "}\n";

	Set<int> un = a.unionWith(b);
	std::cout << "Объединение (A U B): { ";
	un.forEach([](int x) { std::cout << x << " "; });

	Set<int> inter = a.intersectWith(b);
	std::cout << "}\nПересечение (A n B): { ";
	inter.forEach([](int x) { std::cout << x << " "; });

	Set<int> diff = a.differenceWith(b);
	std::cout << "}\nРазность    (A \\ B): { ";
	diff.forEach([](int x) { std::cout << x << " "; });
	std::cout << "}\n";
}


void runMenu() {
	AVLTree<int> tree;

	while (true) {
		std::cout << "\n================ МЕНЮ (АВЛ-ДЕРЕВО) ================\n";
		std::cout << " 1. Вставить число в дерево\n";
		std::cout << " 2. Заполнить дерево готовым набором (10 чисел)\n";
		std::cout << " 3. Удалить число из дерева\n";
		std::cout << " 4. Проверить наличие числа (contains)\n";
		std::cout << " 5. Показать дерево (графика) и все 6 обходов\n";
		std::cout << " 6. Проверить map (x * 2), where (четные) и reduce (сумма)\n";
		std::cout << " 7. Извлечь поддерево по узлу и проверить вхождение\n";
		std::cout << " 8. Очистить дерево\n";
		std::cout << " 9. Демонстрация работы со студентами (Student)\n";
		std::cout << "10. Демонстрация работы с множествами (Set)\n";
		std::cout << "11. Запустить модульные тесты\n";
		std::cout << "12. Запустить бенчмарк (замеры времени)\n";
		std::cout << " 0. Выход\n";
		std::cout << "---------------------------------------------------\n";

		int choice = readInt("Выберите пункт: ");

		if (choice == 0) {
			std::cout << "Выход из программы.\n";
			break;
		} else if (choice == 1) {
			int x = readInt("Введите число для вставки: ");
			if (tree.insert(x)) std::cout << "\033[32mЭлемент добавлен.\033[0m\n";
			else std::cout << "\033[33mТакой элемент уже есть в дереве.\033[0m\n";
			tree.print();
		} else if (choice == 2) {
			int preset[] = {50, 25, 75, 10, 30, 60, 90, 5, 15, 65};
			for (int i = 0; i < 10; i++) tree.insert(preset[i]);
			std::cout << "\033[32mДобавлен набор из 10 элементов:\033[0m\n";
			tree.print();
		} else if (choice == 3) {
			int x = readInt("Введите число для удаления: ");
			if (tree.remove(x)) std::cout << "\033[32mЭлемент удален.\033[0m\n";
			else std::cout << "\033[31mЭлемент не найден.\033[0m\n";
			tree.print();
		} else if (choice == 4) {
			int x = readInt("Введите число для поиска: ");
			if (tree.contains(x)) std::cout << "\033[32mЭлемент НАЙДЕН в дереве.\033[0m\n";
			else std::cout << "\033[31mЭлемент НЕ найден.\033[0m\n";
		} else if (choice == 5) {
			std::cout << "\nГрафическая структура дерева (корень слева):\n";
			tree.print();
			if (!tree.empty()) {
				std::cout << "\n1. ЛКП (по возрастанию): ";
				tree.forEachLKP([](int x) { std::cout << x << " "; });
				std::cout << "\n2. ПКЛ (по убыванию):    ";
				tree.forEachPKL([](int x) { std::cout << x << " "; });
				std::cout << "\n3. КЛП (корень-лево-пр): ";
				tree.forEachKLP([](int x) { std::cout << x << " "; });
				std::cout << "\n4. КПЛ (корень-пр-лево): ";
				tree.forEachKPL([](int x) { std::cout << x << " "; });
				std::cout << "\n5. ЛПК (лево-пр-корень): ";
				tree.forEachLPK([](int x) { std::cout << x << " "; });
				std::cout << "\n6. ПЛК (пр-лево-корень): ";
				tree.forEachPLK([](int x) { std::cout << x << " "; });
				std::cout << "\nВсего элементов: " << tree.size() << "\n";
			}
		} else if (choice == 6) {
			if (tree.empty()) {
				std::cout << "\033[31mДерево пусто! Сначала добавьте элементы.\033[0m\n";
			} else {
				AVLTree<int> mapped = tree.map([](int x) { return x * 2; });
				std::cout << "После map (умножение на 2): ";
				mapped.forEachLKP([](int x) { std::cout << x << " "; });

				AVLTree<int> evens = tree.where([](int x) { return x % 2 == 0; });
				std::cout << "\nПосле where (только четные): ";
				evens.forEachLKP([](int x) { std::cout << x << " "; });

				int sum = tree.reduce([](int val, int acc) { return val + acc; }, 0);
				std::cout << "\nСумма элементов (reduce): " << sum << "\n";
			}
		} else if (choice == 7) {
			if (tree.empty()) {
				std::cout << "\033[31mДерево пусто!\033[0m\n";
			} else {
				int val = readInt("Введите значение узла для выделения поддерева: ");
				try {
					AVLTree<int> sub = tree.extractSubtree(val);
					std::cout << "\033[32mВыделено поддерево (размер " << sub.size() << "):\033[0m\n";
					sub.print();
					std::cout << "Проверка containsSubtree: "
					          << (tree.containsSubtree(sub) ? "ВХОДИТ (true)\n" : "НЕ ВХОДИТ (false)\n");
				} catch (const std::exception& e) {
					std::cout << "\033[31mОшибка: " << e.what() << "\033[0m\n";
				}
			}
		} else if (choice == 8) {
			tree.clear();
			std::cout << "\033[32mДерево очищено.\033[0m\n";
		} else if (choice == 9) {
			demoStudents();
		} else if (choice == 10) {
			demoSets();
		} else if (choice == 11) {
			runAllTests();
		} else if (choice == 12) {
			runBenchmarks();
		} else {
			std::cout << "\033[31mНеверный пункт меню.\033[0m\n";
		}
	}
}