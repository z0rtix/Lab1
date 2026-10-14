#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <cstddef>
#include <utility>
#include <string>
#include <sstream>
#include <stdexcept>


template <typename T>
class AVLTree {
    private:
        struct Node {
            T value;
            Node* left;
            Node* right;
            int height;

            Node(const T& val) : value(val), left(nullptr), right(nullptr), height(1) {}
            Node(T&& val) noexcept : value(std::move(val)), left(nullptr), right(nullptr), height(1) {}
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
            if (!node) {
                return nullptr;
            }

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

        Node* findMin(Node* node) noexcept {
            while (node && node->left) {
                node = node->left;
            }
            return node;
        }

        Node* removeMin(Node* node) {
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

                if (!rightChild) {
                    return leftChild;
                }

                Node* min = findMin(rightChild);
                min->right = removeMin(rightChild);
                min->left = leftChild;
                return balance(min);
            }

            return balance(node);
        }

        Node* copyTree(Node* node) {
            if (!node) {
                return nullptr;
            }
            Node* newNode = new Node(node->value);
            newNode->height = node->height;
            newNode->left = copyTree(node->left);
            newNode->right = copyTree(node->right);
            return newNode;
        }

        void destroyTree(Node* node) noexcept {
            if (node) {
                destroyTree(node->left);
                destroyTree(node->right);
                delete node;
            }
        }

        Node* findNode(Node* node, const T& val) const noexcept {
            Node* curr = node;
            while (curr) {
                if (val < curr->value) {
                    curr = curr->left;
                } else if (curr->value < val) {
                    curr = curr->right;
                } else {
                    return curr;
                }
            }
            return nullptr;
        }

        bool areEqualTrees(Node* t1, Node* t2) const noexcept {
            if (!t1 && !t2) {
                return true;
            }
            if (!t1 || !t2) {
                return false;
            }
            return !(t1->value < t2->value) && !(t2->value < t1->value) &&
                areEqualTrees(t1->left, t2->left) &&
                areEqualTrees(t1->right, t2->right);
        }

        bool isSubtree(Node* treeNode, Node* subRoot) const noexcept {
            if (!subRoot) {
                return true;
            }
            if (!treeNode) {
                return false;
            }
            if (areEqualTrees(treeNode, subRoot)) {
                return true;
            }
            return isSubtree(treeNode->left, subRoot) || isSubtree(treeNode->right, subRoot);
        }

        template <typename F>
        void traverseKLP(Node* node, F&& callback) const {
            if (!node) {
                return;
            }
            callback(node->value);
            traverseKLP(node->left, callback);
            traverseKLP(node->right, callback);
        }

        template <typename F>
        void traverseKPL(Node* node, F&& callback) const {
            if (!node) {
                return;
            }
            callback(node->value);
            traverseKPL(node->right, callback);
            traverseKPL(node->left, callback);
        }

        template <typename F>
        void traverseLKP(Node* node, F&& callback) const {
            if (!node) {
                return;
            }
            traverseLKP(node->left, callback);
            callback(node->value);
            traverseLKP(node->right, callback);
        }

        template <typename F>
        void traverseLPK(Node* node, F&& callback) const {
            if (!node) {
                return;
            }
            traverseLPK(node->left, callback);
            traverseLPK(node->right, callback);
            callback(node->value);
        }

        template <typename F>
        void traversePLK(Node* node, F&& callback) const {
            if (!node) {
                return;
            }
            traversePLK(node->right, callback);
            traversePLK(node->left, callback);
            callback(node->value);
        }

        template <typename F>
        void traversePKL(Node* node, F&& callback) const {
            if (!node) {
                return;
            }
            traversePKL(node->right, callback);
            callback(node->value);
            traversePKL(node->left, callback);
        }

        void serializeHelper(Node* node, std::ostringstream& oss) const {
            if (!node) {
                oss << "# ";
                return;
            }
            oss << node->value << " ";
            serializeHelper(node->left, oss);
            serializeHelper(node->right, oss);
        }

        Node* deserializeHelper(std::istringstream& iss) {
            std::string token;
            if (!(iss >> token) || token == "#") {
                return nullptr;
            }

            std::istringstream valStream(token);
            T val;
            valStream >> val;

            Node* node = new Node(val);
            node->left = deserializeHelper(iss);
            node->right = deserializeHelper(iss);
            updateHeight(node);
            return node;
        }

        template <typename Stream>
        void printTreeHelper(Node* node, Stream& out, const std::string& prefix, bool isLeft) const {
            if (!node) {
                return;
            }

            printTreeHelper(node->right, out, prefix + (isLeft ? "│   " : "    "), false);

            out << prefix;
            out << (isLeft ? "└── " : "┌── ");
            out << node->value << " (h:" << node->height << ")\n";

            printTreeHelper(node->left, out, prefix + (isLeft ? "│   " : "    "), true);
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
                clear();
                root = copyTree(other.root);
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
            if (inserted) {
                treeSize++;
            }
            return inserted;
        }

        bool remove(const T& val) {
            bool removed = false;
            root = removeNode(root, val, removed);
            if (removed) {
                treeSize--;
            }
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

        template <typename F>
        void forEachKLP(F&& callback) const {
            traverseKLP(root, callback);
        }

        template <typename F>
        void forEachKPL(F&& callback) const {
            traverseKPL(root, callback);
        }

        template <typename F>
        void forEachLKP(F&& callback) const {
            traverseLKP(root, callback);
        }

        template <typename F>
        void forEachLPK(F&& callback) const {
            traverseLPK(root, callback);
        }

        template <typename F>
        void forEachPLK(F&& callback) const {
            traversePLK(root, callback);
        }

        template <typename F>
        void forEachPKL(F&& callback) const {
            traversePKL(root, callback);
        }

        template <typename F>
        auto map(F&& func) const {
            if (!root) {
                throw std::runtime_error("Cannot call map on an empty tree");
            }
            using ReturnType = decltype(func(root->value));
            AVLTree<ReturnType> result;
            traverseLKP(root, [&](const T& val) {
                result.insert(func(val));
            });
            return result;
        }

        template <typename Predicate>
        AVLTree<T> where(Predicate&& pred) const {
            AVLTree<T> result;
            traverseLKP(root, [&](const T& val) {
                if (pred(val)) {
                    result.insert(val);
                }
            });
            return result;
        }

        template <typename F, typename Accum>
        Accum reduce(F&& func, Accum startVal) const {
            Accum acc = startVal;
            traverseLKP(root, [&](const T& val) {
                acc = func(val, acc);
            });
            return acc;
        }

        AVLTree<T> extractSubtree(const T& val) const {
            Node* sub = findNode(root, val);
            if (!sub) {
                throw std::invalid_argument("Element not found in tree");
            }

            AVLTree<T> result;
            result.root = result.copyTree(sub);
            result.traverseLKP(result.root, [&](const T&) {
                result.treeSize++;
            });
            return result;
        }

        bool containsSubtree(const AVLTree<T>& sub) const noexcept {
            return isSubtree(root, sub.root);
        }

        std::string serialize() const {
            std::ostringstream oss;
            serializeHelper(root, oss);
            return oss.str();
        }

        void deserialize(const std::string& data) {
            clear();
            std::istringstream iss(data);
            root = deserializeHelper(iss);
            traverseLKP(root, [&](const T&) {
                treeSize++;
            });
        }

        template <typename Stream>
        void print(Stream& out) const {
            if (!root) {
                out << "[Empty tree]\n";
                return;
            }
            printTreeHelper(root->right, out, "", false);
            out << root->value << " (h:" << root->height << ")\n";
            printTreeHelper(root->left, out, "", true);
        }
};

#endif