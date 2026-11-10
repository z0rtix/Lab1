#include "tests.hpp"
#include "AVLTree.hpp"
#include "Set.hpp"
#include "Person.hpp"

#include <iostream>
#include <cassert>
#include <stdexcept>


class MemoryTracker {
	private:
		int id;
		static int activeCount;

	public:
		explicit MemoryTracker(int val = 0) : id(val) {
			activeCount++;
		}

		MemoryTracker(const MemoryTracker& other) : id(other.id) {
			activeCount++;
		}

		MemoryTracker(MemoryTracker&& other) noexcept : id(other.id) {
			other.id = -1;
			activeCount++;
		}

		MemoryTracker& operator=(const MemoryTracker& other) {
			if (this != &other) {
				id = other.id;
			}
			return *this;
		}

		MemoryTracker& operator=(MemoryTracker&& other) noexcept {
			if (this != &other) {
				id = other.id;
				other.id = -1;
			}
			return *this;
		}

		~MemoryTracker() {
			activeCount--;
		}

		bool operator<(const MemoryTracker& other) const noexcept {
			return id < other.id;
		}

		static int getActiveCount() noexcept {
			return activeCount;
		}
};


int MemoryTracker::activeCount = 0;


static void testEmptyAndSingleNode() {
	AVLTree<int> tree;
	assert(tree.empty() == true);
	assert(tree.size() == 0);
	assert(tree.contains(10) == false);
	assert(tree.remove(10) == false);

	assert(tree.insert(10) == true);
	assert(tree.empty() == false);
	assert(tree.size() == 1);
	assert(tree.contains(10) == true);

	assert(tree.insert(10) == false);
	assert(tree.size() == 1);

	assert(tree.remove(10) == true);
	assert(tree.empty() == true);
	assert(tree.size() == 0);

	std::cout << "\033[32m[✓]\033[0m Тест 1: Пустое дерево, одиночный узел и защита от дубликатов\n";
}

static void testAllRotations() {
	AVLTree<int> treeRR;
	treeRR.insert(10);
	treeRR.insert(20);
	treeRR.insert(30);
	int resRR[3];
	int idx = 0;
	treeRR.forEachKLP([&](int x) { resRR[idx++] = x; });
	assert(resRR[0] == 20 && resRR[1] == 10 && resRR[2] == 30);

	AVLTree<int> treeLL;
	treeLL.insert(30);
	treeLL.insert(20);
	treeLL.insert(10);
	int resLL[3];
	idx = 0;
	treeLL.forEachKLP([&](int x) { resLL[idx++] = x; });
	assert(resLL[0] == 20 && resLL[1] == 10 && resLL[2] == 30);

	AVLTree<int> treeLR;
	treeLR.insert(30);
	treeLR.insert(10);
	treeLR.insert(20);
	int resLR[3];
	idx = 0;
	treeLR.forEachKLP([&](int x) { resLR[idx++] = x; });
	assert(resLR[0] == 20 && resLR[1] == 10 && resLR[2] == 30);

	AVLTree<int> treeRL;
	treeRL.insert(10);
	treeRL.insert(30);
	treeRL.insert(20);
	int resRL[3];
	idx = 0;
	treeRL.forEachKLP([&](int x) { resRL[idx++] = x; });
	assert(resRL[0] == 20 && resRL[1] == 10 && resRL[2] == 30);

	std::cout << "\033[32m[✓]\033[0m Тест 2: Все 4 вида балансировки АВЛ (LL, RR, LR, RL)\n";
}

static void testRemoveCases() {
	AVLTree<int> tree;
	int values[] = {50, 25, 75, 10, 30, 60, 90, 5, 65};
	for (int i = 0; i < 9; i++) {
		tree.insert(values[i]);
	}
	assert(tree.size() == 9);

	assert(tree.remove(5) == true);
	assert(tree.contains(5) == false);
	assert(tree.size() == 8);

	assert(tree.remove(60) == true);
	assert(tree.contains(60) == false);
	assert(tree.contains(65) == true);
	assert(tree.size() == 7);

	assert(tree.remove(25) == true);
	assert(tree.contains(25) == false);
	assert(tree.contains(10) == true);
	assert(tree.contains(30) == true);
	assert(tree.size() == 6);

	assert(tree.remove(50) == true);
	assert(tree.contains(50) == false);
	assert(tree.size() == 5);

	assert(tree.remove(999) == false);
	assert(tree.size() == 5);

	std::cout << "\033[32m[✓]\033[0m Тест 3: Удаление листа, узла с 1 и 2 потомками, удаление корня\n";
}

static void testTraversals() {
	AVLTree<int> tree;
	tree.insert(20);
	tree.insert(10);
	tree.insert(30);

	int arr[3];
	int idx = 0;

	idx = 0;
	tree.forEachKLP([&](int x) { arr[idx++] = x; });
	assert(arr[0] == 20 && arr[1] == 10 && arr[2] == 30);

	idx = 0;
	tree.forEachKPL([&](int x) { arr[idx++] = x; });
	assert(arr[0] == 20 && arr[1] == 30 && arr[2] == 10);

	idx = 0;
	tree.forEachLKP([&](int x) { arr[idx++] = x; });
	assert(arr[0] == 10 && arr[1] == 20 && arr[2] == 30);

	idx = 0;
	tree.forEachLPK([&](int x) { arr[idx++] = x; });
	assert(arr[0] == 10 && arr[1] == 30 && arr[2] == 20);

	idx = 0;
	tree.forEachPLK([&](int x) { arr[idx++] = x; });
	assert(arr[0] == 30 && arr[1] == 10 && arr[2] == 20);

	idx = 0;
	tree.forEachPKL([&](int x) { arr[idx++] = x; });
	assert(arr[0] == 30 && arr[1] == 20 && arr[2] == 10);

	std::cout << "\033[32m[✓]\033[0m Тест 4: Проверка всех 6 обходов (КЛП, КПЛ, ЛКП, ЛПК, ПЛК, ПКЛ)\n";
}

static void testFunctionalMethods() {
	AVLTree<int> tree;
	for (int i = 1; i <= 5; i++) {
		tree.insert(i);
	}

	AVLTree<int> mapped = tree.map([](int x) { return x * 10; });
	assert(mapped.size() == 5);
	assert(mapped.contains(10) == true);
	assert(mapped.contains(50) == true);
	assert(mapped.contains(1) == false);

	AVLTree<int> filtered = tree.where([](int x) { return x % 2 == 0; });
	assert(filtered.size() == 2);
	assert(filtered.contains(2) == true);
	assert(filtered.contains(4) == true);
	assert(filtered.contains(3) == false);

	int sum = tree.reduce([](int val, int acc) { return val + acc; }, 0);
	assert(sum == 15);

	std::cout << "\033[32m[✓]\033[0m Тест 5: Функциональные методы map, where, reduce\n";
}

static void testSubtrees() {
	AVLTree<int> tree;
	tree.insert(40);
	tree.insert(20);
	tree.insert(60);
	tree.insert(10);
	tree.insert(30);

	AVLTree<int> sub = tree.extractSubtree(20);
	assert(sub.size() == 3);
	assert(sub.contains(20) == true);
	assert(sub.contains(10) == true);
	assert(sub.contains(30) == true);
	assert(tree.containsSubtree(sub) == true);

	sub.insert(99);
	assert(tree.containsSubtree(sub) == false);

	AVLTree<int> emptySub;
	assert(tree.containsSubtree(emptySub) == true);

	std::cout << "\033[32m[✓]\033[0m Тест 6: Извлечение поддерева и поиск поддерева\n";
}

static void testCopyAndMove() {
	AVLTree<int> t1;
	t1.insert(10);
	t1.insert(20);
	t1.insert(30);

	AVLTree<int> t2(t1);
	assert(t2.size() == 3);
	assert(t2.contains(20) == true);

	t2.insert(40);
	assert(t2.size() == 4);
	assert(t1.size() == 3);

	AVLTree<int> t3;
	t3.insert(99);
	t3 = t1;
	assert(t3.size() == 3);
	assert(t3.contains(99) == false);

	AVLTree<int> t4(std::move(t1));
	assert(t4.size() == 3);
	assert(t1.empty() == true);
	assert(t1.size() == 0);

	AVLTree<int> t5;
	t5 = std::move(t4);
	assert(t5.size() == 3);
	assert(t4.empty() == true);

	std::cout << "\033[32m[✓]\033[0m Тест 7: Глубокое копирование и семантика перемещения (Rule of 5)\n";
}

static void testExceptions() {
	AVLTree<int> emptyTree;

	bool mapThrown = false;
	try {
		emptyTree.map([](int x) { return x * 2; });
	} catch (const std::runtime_error&) {
		mapThrown = true;
	}
	assert(mapThrown == true);

	bool reduceThrown = false;
	try {
		emptyTree.reduce([](int x, int acc) { return x + acc; }, 0);
	} catch (const std::runtime_error&) {
		reduceThrown = true;
	}
	assert(reduceThrown == true);

	bool extractThrown = false;
	try {
		emptyTree.extractSubtree(42);
	} catch (const std::invalid_argument&) {
		extractThrown = true;
	}
	assert(extractThrown == true);

	std::cout << "\033[32m[✓]\033[0m Тест 8: Перехват исключений на пустом дереве и неверном ключе\n";
}

static void testSetOperations() {
	Set<int> a;
	Set<int> b;

	for (int i = 1; i <= 5; i++) a.insert(i);
	for (int i = 4; i <= 8; i++) b.insert(i);

	Set<int> un = a.unionWith(b);
	assert(un.size() == 8);

	Set<int> inter = a.intersectWith(b);
	assert(inter.size() == 2);
	assert(inter.contains(4) == true);
	assert(inter.contains(5) == true);

	Set<int> diff = a.differenceWith(b);
	assert(diff.size() == 3);
	assert(diff.contains(1) == true);
	assert(diff.contains(3) == true);
	assert(diff.contains(4) == false);

	assert(inter.isSubsetOf(a) == true);
	assert(a.isSubsetOf(un) == true);
	assert(un.isSubsetOf(a) == false);

	std::cout << "\033[32m[✓]\033[0m Тест 9: Операции над множествами Set (объединение, пересечение, разность)\n";
}

static void testStudentsAndTeachers() {
	AVLTree<Student> students;
	Student s1({10, 1}, "Ivan", "Ivanov", 4.8);
	Student s2({10, 2}, "Petr", "Petrov", 3.5);
	Student s3({9, 99}, "Anna", "Sidorova", 5.0);

	assert(students.insert(s1) == true);
	assert(students.insert(s2) == true);
	assert(students.insert(s3) == true);
	assert(students.insert(s1) == false);
	assert(students.size() == 3);

	AVLTree<Student> best = students.where([](const Student& s) {
		return s.getGPA() >= 4.5;
	});
	assert(best.size() == 2);

	AVLTree<Teacher> teachers;
	Teacher t1({50, 1}, "Igor", "Sedykh", "Software Engineering");
	assert(teachers.insert(t1) == true);
	assert(teachers.contains(t1) == true);

	std::cout << "\033[32m[✓]\033[0m Тест 10: Работа дерева с типами Student и Teacher по PersonID\n";
}

static void testStressAndMemoryLeaks() {
	assert(MemoryTracker::getActiveCount() == 0);
	{
		AVLTree<MemoryTracker> tree;
		for (int i = 0; i < 500; i++) {
			tree.insert(MemoryTracker(i));
		}
		assert(tree.size() == 500);
		assert(MemoryTracker::getActiveCount() == 500);

		AVLTree<MemoryTracker> copy = tree;
		assert(MemoryTracker::getActiveCount() == 1000);

		for (int i = 0; i < 250; i++) {
			tree.remove(MemoryTracker(i));
		}
		assert(tree.size() == 250);
		assert(MemoryTracker::getActiveCount() == 750);

		tree.clear();
		assert(MemoryTracker::getActiveCount() == 500);
	}
	assert(MemoryTracker::getActiveCount() == 0);

	std::cout << "\033[32m[✓]\033[0m Тест 11: Стресс-тест на 500 элементов и проверка отсутствия утечек (0 leaks)\n";
}


void runAllTests() {
	std::cout << "\n======================================================\n";
	std::cout << "               ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ                \n";
	std::cout << "======================================================\n";

	testEmptyAndSingleNode();
	testAllRotations();
	testRemoveCases();
	testTraversals();
	testFunctionalMethods();
	testSubtrees();
	testCopyAndMove();
	testExceptions();
	testSetOperations();
	testStudentsAndTeachers();
	testStressAndMemoryLeaks();

	std::cout << "------------------------------------------------------\n";
	std::cout << "\033[32mВсе 11 блоков тестирования успешно пройдены!\033[0m\n\n";
}