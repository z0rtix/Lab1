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
		explicit MemoryTracker(int val = 0) noexcept : id(val) {
			activeCount++;
		}

		MemoryTracker(const MemoryTracker& other) noexcept : id(other.id) {
			activeCount++;
		}

		MemoryTracker(MemoryTracker&& other) noexcept : id(other.id) {
			other.id = -1;
			activeCount++;
		}

		MemoryTracker& operator=(const MemoryTracker& other) noexcept {
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

		~MemoryTracker() noexcept {
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

static void testEmptyAndDuplicates() {
	AVLTree<int> tree;
	assert(tree.empty() == true);
	assert(tree.size() == 0);
	assert(tree.contains(10) == false);
	assert(tree.remove(10) == false);

	int visited = 0;
	tree.forEachLKP([&](int) { visited++; });
	assert(visited == 0);

	assert(tree.insert(10) == true);
	assert(tree.empty() == false);
	assert(tree.size() == 1);
	assert(tree.contains(10) == true);

	assert(tree.insert(10) == false);
	assert(tree.size() == 1);

	int rvalDup = 10;
	assert(tree.insert(std::move(rvalDup)) == false);
	assert(tree.size() == 1);

	int rvalNew = 20;
	assert(tree.insert(std::move(rvalNew)) == true);
	assert(tree.size() == 2);

	tree.clear();
	assert(tree.empty() == true);
	assert(tree.size() == 0);

	std::cout << "\033[32m[✓]\033[0m Тест 01: Граничные случаи пустого дерева, rvalue-вставка и дубликаты\n";
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

	std::cout << "\033[32m[✓]\033[0m Тест 02: Все 4 вида балансировки АВЛ (повороты LL, RR, LR, RL)\n";
}

static void testRemoveAllCases() {
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

	std::cout << "\033[32m[✓]\033[0m Тест 03: Удаление листа, узла с 1 и 2 потомками, удаление корня\n";
}

static void testAllSixTraversals() {
	AVLTree<int> tree;
	int items[] = {40, 20, 60, 10, 30, 50, 70};
	for (int i = 0; i < 7; i++) {
		tree.insert(items[i]);
	}

	int buf[7];
	int idx = 0;

	int expKLP[] = {40, 20, 10, 30, 60, 50, 70};
	idx = 0;
	tree.forEachKLP([&](int x) { buf[idx++] = x; });
	for (int i = 0; i < 7; i++) assert(buf[i] == expKLP[i]);

	int expKPL[] = {40, 60, 70, 50, 20, 30, 10};
	idx = 0;
	tree.forEachKPL([&](int x) { buf[idx++] = x; });
	for (int i = 0; i < 7; i++) assert(buf[i] == expKPL[i]);

	int expLKP[] = {10, 20, 30, 40, 50, 60, 70};
	idx = 0;
	tree.forEachLKP([&](int x) { buf[idx++] = x; });
	for (int i = 0; i < 7; i++) assert(buf[i] == expLKP[i]);

	int expLPK[] = {10, 30, 20, 50, 70, 60, 40};
	idx = 0;
	tree.forEachLPK([&](int x) { buf[idx++] = x; });
	for (int i = 0; i < 7; i++) assert(buf[i] == expLPK[i]);

	int expPLK[] = {70, 50, 60, 30, 10, 20, 40};
	idx = 0;
	tree.forEachPLK([&](int x) { buf[idx++] = x; });
	for (int i = 0; i < 7; i++) assert(buf[i] == expPLK[i]);

	int expPKL[] = {70, 60, 50, 40, 30, 20, 10};
	idx = 0;
	tree.forEachPKL([&](int x) { buf[idx++] = x; });
	for (int i = 0; i < 7; i++) assert(buf[i] == expPKL[i]);

	std::cout << "\033[32m[✓]\033[0m Тест 04: Точная проверка всех 6 обходов (КЛП, КПЛ, ЛКП, ЛПК, ПЛК, ПКЛ)\n";
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

	int product = tree.reduce([](int val, int acc) { return val * acc; }, 1);
	assert(product == 120);

	std::cout << "\033[32m[✓]\033[0m Тест 05: Функциональные операции map, where, reduce\n";
}

static void testSubtreesAndEquality() {
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

	AVLTree<int> subCopy = sub;
	assert(sub == subCopy);
	subCopy.insert(99);
	assert(sub != subCopy);
	assert(tree.containsSubtree(subCopy) == false);

	AVLTree<int> emptySub;
	assert(tree.containsSubtree(emptySub) == true);
	assert(emptySub.containsSubtree(sub) == false);

	std::cout << "\033[32m[✓]\033[0m Тест 06: Извлечение поддерева, поиск поддерева и операторы == / !=\n";
}

static void testRuleOfFiveAndSelfAssignment() {
	AVLTree<int> t1;
	t1.insert(10);
	t1.insert(20);
	t1.insert(30);

	AVLTree<int> t2(t1);
	assert(t2 == t1);
	t2.insert(40);
	assert(t2 != t1);
	assert(t1.size() == 3);

	AVLTree<int> t3;
	t3.insert(99);
	t3 = t1;
	assert(t3 == t1);
	assert(t3.contains(99) == false);

	AVLTree<int>& selfRef = t3;
	t3 = selfRef;
	assert(t3.size() == 3);
	assert(t3.contains(20) == true);

	AVLTree<int> t4(std::move(t1));
	assert(t4.size() == 3);
	assert(t1.empty() == true);
	assert(t1.size() == 0);

	AVLTree<int> t5;
	t5.insert(1);
	t5 = std::move(t4);
	assert(t5.size() == 3);
	assert(t4.empty() == true);

	AVLTree<int>& selfMoveRef = t5;
	t5 = std::move(selfMoveRef);
	assert(t5.size() == 3);
	assert(t5.contains(10) == true);

	std::cout << "\033[32m[✓]\033[0m Тест 07: Правило пяти (копирование, перемещение, самоприсваивание)\n";
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

	bool extractEmptyThrown = false;
	try {
		emptyTree.extractSubtree(42);
	} catch (const std::invalid_argument&) {
		extractEmptyThrown = true;
	}
	assert(extractEmptyThrown == true);

	emptyTree.insert(10);
	bool extractMissingThrown = false;
	try {
		emptyTree.extractSubtree(999);
	} catch (const std::invalid_argument&) {
		extractMissingThrown = true;
	}
	assert(extractMissingThrown == true);

	std::cout << "\033[32m[✓]\033[0m Тест 08: Перехват исключений (runtime_error, invalid_argument)\n";
}

static void testSetOperationsAndRuleOfFive() {
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

	Set<int> copyA(a);
	assert(copyA == a);
	copyA.remove(1);
	assert(copyA != a);

	Set<int> movedA(std::move(copyA));
	assert(movedA.size() == 4);
	assert(copyA.empty() == true);

	int rvalSetItem = 100;
	assert(movedA.insert(std::move(rvalSetItem)) == true);
	assert(movedA.contains(100) == true);

	std::cout << "\033[32m[✓]\033[0m Тест 09: Множество Set (объединение, пересечение, разность, Rule of 5)\n";
}

static void testStudentsAndTeachers() {
	AVLTree<Student> students;
	Student s1(PersonID(10, 1), "Artem", "Tokovenko", 4.9);
	Student s2(PersonID(10, 2), "Petr", "Petrov", 3.5);
	Student s3(PersonID(9, 99), "Anna", "Sidorova", 5.0);
	Student s1Dup(PersonID(10, 1), "Clone", "User", 2.0);

	assert(students.insert(s1) == true);
	assert(students.insert(s2) == true);
	assert(students.insert(s3) == true);
	assert(students.insert(s1Dup) == false);
	assert(students.size() == 3);
	assert(students.contains(s1) == true);

	AVLTree<Student> best = students.where([](const Student& s) {
		return s.getGPA() >= 4.5;
	});
	assert(best.size() == 2);

	AVLTree<Teacher> teachers;
	Teacher t1(PersonID(50, 1), "Igor", "Sedykh", "Software Engineering");
	Teacher t2(PersonID(50, 2), "Valery", "Roslovtsev", "Cybernetics");
	assert(teachers.insert(t1) == true);
	assert(teachers.insert(t2) == true);
	assert(teachers.contains(t1) == true);
	assert(teachers.remove(t1) == true);
	assert(teachers.size() == 1);

	std::cout << "\033[32m[✓]\033[0m Тест 10: Работа дерева с типами Student и Teacher по ключу PersonID\n";
}

static void testStressSequentialInsert() {
	AVLTree<int> tree;
	const int count = 1000;
	for (int i = 1; i <= count; i++) {
		assert(tree.insert(i) == true);
	}
	assert(tree.size() == static_cast<size_t>(count));

	int prev = 0;
	tree.forEachLKP([&](int val) {
		assert(val > prev);
		prev = val;
	});

	for (int i = 1; i <= count; i += 2) {
		assert(tree.remove(i) == true);
	}
	assert(tree.size() == static_cast<size_t>(count / 2));

	std::cout << "\033[32m[✓]\033[0m Тест 11: Стресс-тест на 1000 упорядоченных вставок и удалений\n";
}

static void testMemoryLeaksWithTracker() {
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

		AVLTree<MemoryTracker> moved = std::move(tree);
		assert(MemoryTracker::getActiveCount() == 1000);
		assert(tree.empty() == true);

		for (int i = 0; i < 250; i++) {
			copy.remove(MemoryTracker(i));
		}
		assert(copy.size() == 250);
		assert(MemoryTracker::getActiveCount() == 750);

		copy.clear();
		assert(MemoryTracker::getActiveCount() == 500);
	}
	assert(MemoryTracker::getActiveCount() == 0);

	std::cout << "\033[32m[✓]\033[0m Тест 12: Контроль памяти MemoryTracker (0 утечек, активных объектов: 0)\n";
}


void runAllTests() {
	std::cout << "\n\033[36m======================================================================\033[0m\n";
	std::cout << "\033[36m                  ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ (12 БЛОКОВ)                 \033[0m\n";
	std::cout << "\033[36m======================================================================\033[0m\n";

	testEmptyAndDuplicates();
	testAllRotations();
	testRemoveAllCases();
	testAllSixTraversals();
	testFunctionalMethods();
	testSubtreesAndEquality();
	testRuleOfFiveAndSelfAssignment();
	testExceptions();
	testSetOperationsAndRuleOfFive();
	testStudentsAndTeachers();
	testStressSequentialInsert();
	testMemoryLeaksWithTracker();

	std::cout << "\033[36m----------------------------------------------------------------------\033[0m\n";
	std::cout << "\033[32m[УСПЕХ] Все 12 блоков тестирования пройдены без единой ошибки!\033[0m\n\n";
}