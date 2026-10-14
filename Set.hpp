#ifndef SET_HPP
#define SET_HPP

#include "AVLTree.hpp"


template <typename T>
class Set {
    private:
        AVLTree<T> tree;

    public:
        Set() = default;

        bool insert(const T& val) {
            return tree.insert(val);
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

        template <typename F>
        void forEach(F&& callback) const {
            tree.forEachLKP(callback);
        }

        Set<T> unionWith(const Set<T>& other) const {
            Set<T> result = *this;
            other.forEach([&](const T& val) {
                result.insert(val);
            });
            return result;
        }

        Set<T> intersectWith(const Set<T>& other) const {
            Set<T> result;
            forEach([&](const T& val) {
                if (other.contains(val)) {
                    result.insert(val);
                }
            });
            return result;
        }

        Set<T> differenceWith(const Set<T>& other) const {
            Set<T> result;
            forEach([&](const T& val) {
                if (!other.contains(val)) {
                    result.insert(val);
                }
            });
            return result;
        }

        bool isSubsetOf(const Set<T>& other) const {
            bool subset = true;
            forEach([&](const T& val) {
                if (!other.contains(val)) {
                    subset = false;
                }
            });
            return subset;
        }

        bool operator==(const Set<T>& other) const {
            if (size() != other.size()) {
                return false;
            }
            return isSubsetOf(other);
        }

        template <typename Stream>
        void print(Stream& out) const {
            tree.print(out);
        }
};


#endif