#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

template <typename T>
class AVLTree {
	private:
		struct Node {
			T value;
			Node* left;
			Node* right;
			int height;

			explicit Node(const T& val) : value(val), left(nullptr), right(nullptr), height(1) {}
			explicit Node(T&& val) noexcept : value(std::move(val)), left(nullptr), right(nullptr), height(1) {}
		};

		Node* root;
		size_t treeSize;

		int getHeight(Node* node) const noexcept {
			return node ? node->height : 0;
		}

		int getBalanceFactor(Node* node) const noexcept {
			return node ? (getHeight(node->right) - getHeight(node->left)) : 0;
		}

		void updateHeight(Node* node) noexcept {
			if (node) {
				int leftH = getHeight(node->left);
				int rightH = getHeight(node->right);
				node->height = 1 + (leftH > rightH ? leftH : rightH);
			}
		}

		Node* rotateRight(Node* y) noexcept {
			Node* x = y->left;
			Node* t2 = x->right;

			x->right = y;
			y->left = t2;

			updateHeight(y);
			updateHeight(x);

			return x;
		}

		Node* rotateLeft(Node* x) noexcept {
			Node* y = x->right;
			Node* t2 = y->left;

			y->left = x;
			x->right = t2;

			updateHeight(x);
			updateHeight(y);

			return y;
		}

		Node* balance(Node* node) noexcept {
			if (!node) return nullptr;

			updateHeight(node);
			int bf = getBalanceFactor(node);

			if (bf < -1) {
				if (getBalanceFactor(node->left) > 0) {
					node->left = rotateLeft(node->left);
				}
				return rotateRight(node);
			}

			if (bf > 1) {
				if (getBalanceFactor(node->right) < 0) {
					node->right = rotateRight(node->right);
				}
				return rotateLeft(node);
			}

			return node;
		}

		Node* insertNode(Node* node, const T& val, bool& inserted) {
			if (!node) {
				inserted = true;
				return new Node(val);
			}

			if (val < node->value) {
				node->left = insertNode(node->left, val, inserted);
			} else if (node->value < val) {
				node->right = insertNode(node->right, val, inserted);
			} else {
				inserted = false;
				return node;
			}

			return balance(node);
		}

		Node* insertNodeMove(Node* node, T&& val, bool& inserted) {
			if (!node) {
				inserted = true;
				return new Node(std::move(val));
			}

			if (val < node->value) {
				node->left = insertNodeMove(node->left, std::move(val), inserted);
			} else if (node->value < val) {
				node->right = insertNodeMove(node->right, std::move(val), inserted);
			} else {
				inserted = false;
				return node;
			}

			return balance(node);
		}

		Node* findMin(Node* node) const noexcept {
			while (node && node->left) {
				node = node->left;
			}
			return node;
		}

		Node* removeMin(Node* node) noexcept {
			if (!node->left) {
				return node->right;
			}
			node->left = removeMin(node->left);
			return balance(node);
		}

		Node* removeNode(Node* node, const T& val, bool& removed) {
			if (!node) {
				removed = false;
				return nullptr;
			}

			if (val < node->value) {
				node->left = removeNode(node->left, val, removed);
			} else if (node->value < val) {
				node->right = removeNode(node->right, val, removed);
			} else {
				removed = true;
				Node* leftChild = node->left;
				Node* rightChild = node->right;
				delete node;

				if (!rightChild) return leftChild;

				Node* minNode = findMin(rightChild);
				minNode->right = removeMin(rightChild);
				minNode->left = leftChild;
				return balance(minNode);
			}

			return balance(node);
		}

        void destroyTree(Node* node) const noexcept {
            if (node) {
                destroyTree(node->left);
                destroyTree(node->right);
                delete node;
            }
        }

		Node* copyTree(Node* node) const {
			if (!node) return nullptr;
			Node* newNode = new Node(node->value);
			newNode->height = node->height;
			try {
				newNode->left = copyTree(node->left);
				newNode->right = copyTree(node->right);
			} catch (...) {
				destroyTree(newNode);
				throw;
			}
			return newNode;
		}

		Node* findNode(Node* node, const T& val) const noexcept {
			Node* current = node;
			while (current) {
				if (val < current->value) {
					current = current->left;
				} else if (current->value < val) {
					current = current->right;
				} else {
					return current;
				}
			}
			return nullptr;
		}

		bool areEqualTrees(Node* first, Node* second) const noexcept {
			if (!first && !second) return true;
			if (!first || !second) return false;
			bool equalVal = !(first->value < second->value) && !(second->value < first->value);
			return equalVal &&
			       areEqualTrees(first->left, second->left) &&
			       areEqualTrees(first->right, second->right);
		}

		bool isSubtree(Node* mainNode, Node* subNode) const noexcept {
			if (!subNode) return true;
			if (!mainNode) return false;
			if (areEqualTrees(mainNode, subNode)) return true;
			return isSubtree(mainNode->left, subNode) || isSubtree(mainNode->right, subNode);
		}

		template <typename Func>
		void traverseKLP(Node* node, Func callback) const {
			if (!node) return;
			callback(node->value);
			traverseKLP(node->left, callback);
			traverseKLP(node->right, callback);
		}

		template <typename Func>
		void traverseKPL(Node* node, Func callback) const {
			if (!node) return;
			callback(node->value);
			traverseKPL(node->right, callback);
			traverseKPL(node->left, callback);
		}

		template <typename Func>
		void traverseLKP(Node* node, Func callback) const {
			if (!node) return;
			traverseLKP(node->left, callback);
			callback(node->value);
			traverseLKP(node->right, callback);
		}

		template <typename Func>
		void traverseLPK(Node* node, Func callback) const {
			if (!node) return;
			traverseLPK(node->left, callback);
			traverseLPK(node->right, callback);
			callback(node->value);
		}

		template <typename Func>
		void traversePLK(Node* node, Func callback) const {
			if (!node) return;
			traversePLK(node->right, callback);
			traversePLK(node->left, callback);
			callback(node->value);
		}

		template <typename Func>
		void traversePKL(Node* node, Func callback) const {
			if (!node) return;
			traversePKL(node->right, callback);
			callback(node->value);
			traversePKL(node->left, callback);
		}

		void printHelper(Node* node, const std::string& prefix, bool isLeft) const {
			if (!node) return;
			printHelper(node->right, prefix + (isLeft ? "│   " : "    "), false);
			std::cout << prefix << (isLeft ? "└── " : "┌── ") << node->value << " (h=" << node->height << ")\n";
			printHelper(node->left, prefix + (isLeft ? "│   " : "    "), true);
		}

	public:
		AVLTree() noexcept : root(nullptr), treeSize(0) {}

		AVLTree(const AVLTree& other) : root(copyTree(other.root)), treeSize(other.treeSize) {}

		AVLTree(AVLTree&& other) noexcept : root(other.root), treeSize(other.treeSize) {
			other.root = nullptr;
			other.treeSize = 0;
		}

		AVLTree& operator=(const AVLTree& other) {
			if (this != &other) {
				Node* newRoot = copyTree(other.root);
				clear();
				root = newRoot;
				treeSize = other.treeSize;
			}
			return *this;
		}

		AVLTree& operator=(AVLTree&& other) noexcept {
			if (this != &other) {
				clear();
				root = other.root;
				treeSize = other.treeSize;
				other.root = nullptr;
				other.treeSize = 0;
			}
			return *this;
		}

		~AVLTree() noexcept {
			clear();
		}

		bool insert(const T& val) {
			bool inserted = false;
			root = insertNode(root, val, inserted);
			if (inserted) treeSize++;
			return inserted;
		}

		bool insert(T&& val) {
			bool inserted = false;
			root = insertNodeMove(root, std::move(val), inserted);
			if (inserted) treeSize++;
			return inserted;
		}

		bool remove(const T& val) {
			bool removed = false;
			root = removeNode(root, val, removed);
			if (removed) treeSize--;
			return removed;
		}

		bool contains(const T& val) const noexcept {
			return findNode(root, val) != nullptr;
		}

		size_t size() const noexcept {
			return treeSize;
		}

		bool empty() const noexcept {
			return treeSize == 0;
		}

		void clear() noexcept {
			destroyTree(root);
			root = nullptr;
			treeSize = 0;
		}

		template <typename Func>
		void forEachKLP(Func callback) const { traverseKLP(root, callback); }

		template <typename Func>
		void forEachKPL(Func callback) const { traverseKPL(root, callback); }

		template <typename Func>
		void forEachLKP(Func callback) const { traverseLKP(root, callback); }

		template <typename Func>
		void forEachLPK(Func callback) const { traverseLPK(root, callback); }

		template <typename Func>
		void forEachPLK(Func callback) const { traversePLK(root, callback); }

		template <typename Func>
		void forEachPKL(Func callback) const { traversePKL(root, callback); }

		template <typename Func>
		AVLTree<T> map(Func func) const {
			if (!root) throw std::runtime_error("Empty tree");
			AVLTree<T> result;
			traverseLKP(root, [&](const T& val) {
				result.insert(func(val));
			});
			return result;
		}

		template <typename Func>
		AVLTree<T> where(Func pred) const {
			AVLTree<T> result;
			traverseLKP(root, [&](const T& val) {
				if (pred(val)) result.insert(val);
			});
			return result;
		}

		template <typename Func>
		T reduce(Func func, T startVal) const {
			if (!root) throw std::runtime_error("Empty tree");
			T acc = startVal;
			traverseLKP(root, [&](const T& val) {
				acc = func(val, acc);
			});
			return acc;
		}

		AVLTree<T> extractSubtree(const T& val) const {
			Node* sub = findNode(root, val);
			if (!sub) throw std::invalid_argument("Element not found");
			AVLTree<T> result;
			result.root = copyTree(sub);
			result.traverseLKP(result.root, [&](const T&) {
				result.treeSize++;
			});
			return result;
		}

		bool containsSubtree(const AVLTree<T>& sub) const noexcept {
			return isSubtree(root, sub.root);
		}

		void print() const {
			if (!root) {
				std::cout << "[Дерево пусто]\n";
				return;
			}
			printHelper(root->right, "", false);
			std::cout << root->value << " (h=" << root->height << ")\n";
			printHelper(root->left, "", true);
		}
};

#endif