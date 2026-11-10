#ifndef SET_HPP
#define SET_HPP

#include "AVLTree.hpp"

#include <utility>


template <typename T>
class Set {
	private:
		AVLTree<T> tree;

	public:
		Set() noexcept : tree() {}

		Set(const Set& other) : tree(other.tree) {}

		Set(Set&& other) noexcept : tree(std::move(other.tree)) {}

		Set& operator=(const Set& other) {
			if (this != &other) {
				tree = other.tree;
			}
			return *this;
		}

		Set& operator=(Set&& other) noexcept {
			if (this != &other) {
				tree = std::move(other.tree);
			}
			return *this;
		}

		~Set() noexcept = default;

		bool insert(const T& val) {
			return tree.insert(val);
		}

		bool insert(T&& val) {
			return tree.insert(std::move(val));
		}

		bool remove(const T& val) {
			return tree.remove(val);
		}

		bool contains(const T& val) const noexcept {
			return tree.contains(val);
		}

		size_t size() const noexcept {
			return tree.size();
		}

		bool empty() const noexcept {
			return tree.empty();
		}

		void clear() noexcept {
			tree.clear();
		}

		Set<T> unionWith(const Set<T>& other) const {
			Set<T> result = *this;
			other.tree.forEachLKP([&](const T& item) {
				result.insert(item);
			});
			return result;
		}

		Set<T> intersectWith(const Set<T>& other) const {
			Set<T> result;
			tree.forEachLKP([&](const T& item) {
				if (other.contains(item)) {
					result.insert(item);
				}
			});
			return result;
		}

		Set<T> differenceWith(const Set<T>& other) const {
			Set<T> result;
			tree.forEachLKP([&](const T& item) {
				if (!other.contains(item)) {
					result.insert(item);
				}
			});
			return result;
		}

		bool isSubsetOf(const Set<T>& other) const noexcept {
			bool isSub = true;
			tree.forEachLKP([&](const T& item) {
				if (!other.contains(item)) {
					isSub = false;
				}
			});
			return isSub;
		}

		template <typename Func>
		void forEach(Func callback) const {
			tree.forEachLKP(callback);
		}
};


#endif