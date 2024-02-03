//
// Created by Jlisowskyy on 2/2/24.
//

#ifndef AVLTREE_H
#define AVLTREE_H

#include <functional>
#include <bit>

#include "_AVLcore.h"
#include "../bTreeHelpers.h"

template<
    class KeyT,
    class ItemT,
    class PredT = std::greater<KeyT>
>class AVLTree {
    // ------------------------------
    // Inner Types
    // ------------------------------

    struct node {
        KeyT _key;
        ItemT _item;
        int8_t bl{};

        node* left{};
        node* right{};

        node() = default;
        node(const node& other): _key{other._key}, _item{other._item} {}
        node(KeyT&& key, ItemT&& item): _key{std::move(key)}, _item{std::move(item)} {}
        node(const KeyT& key, const ItemT& item): _key{key}, _item{item} {}

        friend bool operator>(const node& a, const KeyT& b) {
            return pred(a._key, b);
        }

        friend bool operator>(const KeyT& a, const node& b) {
            return pred(a, b._key);
        }

        friend std::ostream& operator<<(std::ostream& out, const node& n) {
            return out << n._key;
        }

        ~node() = default;

    private:
        inline static PredT pred{};
    };

    enum tiltStates {
        rightTilt = -1,
        noTilt = 0,
        leftTilt = 1
    };

    enum directions {
        left = 0,
        right = 1,
    };

    using AVLCore = _AVLCore<node, true>;

    // ------------------------------
    // Class creation
    // ------------------------------
public:

    AVLTree() = default;

    AVLTree(const AVLTree& other):
        _elemCount{other._elemCount}, _root{CloneTree(other._root)} {}

    AVLTree(AVLTree&& other) noexcept: _elemCount{other._elemCount}, _root{other._root} {
        other._root = nullptr;
        other._elemCount = 0;
    }

    AVLTree& operator=(const AVLTree& other) {
        if (this == &other) return *this;

        CleanTree(_root);

        _root = CloneTree(other._root);
        _elemCount = other._elemCount;

        return *this;
    }

    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this == &other) return *this;

        CleanTree(_root);

        _root = other._root;
        _elemCount = other._elemCount;

        other._root = nullptr;
        other._elemCount = 0;

        return *this;
    }

    // ------------------------------
    // Class interaction
    // ------------------------------

    void clean() {
        CleanTree(_root);
        _root = nullptr;
        _elemCount = 0;
    }

    bool insertRecursive(const KeyT& key, const ItemT& item) {
        return _inserterRecu::insert(key, item, _root);
    }

    bool insert(const KeyT& key, const ItemT& item) {
        return _insertNotRecursiveDoublePointers(key, item, _root);
    }

    bool insert(const std::pair<KeyT, ItemT>& pair) {
        const auto& [key, item] = pair;
        return _insertNotRecursiveDoublePointers(key, item, _root);;
    }

    bool insert(KeyT&& key, ItemT&& item) {
        return _insertNotRecursiveDoublePointers(key, item, _root);;
    }

    bool insert(node* n) {
        return false; // TODO
    }

    bool remove(const KeyT& key) {
        // node** n = _searchDP(key);
        // if (!*n) return false;
        //
        // _delete(n);

        return true; // TODO
    }

    [[nodiscard]] node* removeAndGetNode(const KeyT& key) {
        // node** n = _searchDP(key);
        // if (!*n) return nullptr;
        //
        // _delete(n);
        return nullptr; // TODO
    }

    [[nodiscard]] bool removeAndGet(const KeyT& key, ItemT& out) {
        // node** n = _searchDP(key);
        // if (!*n) return false;
        //
        // out = std::move((*n)->content.second);
        // _delete(n);

        return true; // TODO
    }

    [[nodiscard]] bool contains(const KeyT& key) const {
        node* n = _search(key);
        if (!n) return false;
        return true;
    }

    // Note: due to high cost of insertion dummy is used
    [[nodiscard]] ItemT& safeGet(const KeyT& key) {
        static ItemT dummy{};

        node* n = _search(key);
        if (!n) return dummy;

        return n->_item;
    }

    // Note: key must exist inside the tree
    [[nodiscard]] ItemT& get(const KeyT& key) const {
        return  _search(key)->_item;
    }

    [[nodiscard]] ItemT& operator[](const KeyT& key) {
        return safeGet(key);
    }

    [[nodiscard]] const ItemT& operator[](const KeyT& key) const {
        return get(key);
    }

    friend std::ostream& operator<<(std::ostream& out, const AVLTree& tree) {
        return PrettyBTreePrinter<node>::PrintWithQueue(out, tree._root);
    }

private:

    node* _search(const KeyT& key) {
        node* n = _root;

        while(n) {
            if (key > *n) n = n->right;
            else if (*n> key) n = n->left;
            else return n;
        }

        return n;
    }

    template<class StackItemT>
    struct _simpleStack {
        explicit _simpleStack(const size_t size): _tab(size) {}
        ~_simpleStack() = default;

        void push(StackItemT n){
            _tab[pos++] = n;
        }

        StackItemT pop() {
            return _tab[--pos];
        }

        [[nodiscard]] size_t size() const {
            return pos;
        }

    private:
        size_t pos{};
        std::vector<StackItemT> _tab;
    };

    bool _insertNotRecursiveDoublePointers(const KeyT& key, const ItemT& item, node*& root) {
        // structures preparing
        const size_t maxSize = 3 * std::countl_zero(_elemCount) / 2;
        _simpleStack<node**> stackNode(maxSize);
        _simpleStack<bool> stackDir(maxSize);
        node** nd = &root;

        stackNode.push(nd);

        // tree traversal
        while(*nd) {
            if (key > **nd) {
                nd = &(*nd)->right;
                stackDir.push(right);
            }
            else if (**nd > key) {
                nd = &(*nd)->left;
                stackDir.push(left);
            }
            else return false;

            stackNode.push(nd);
        }

        // attaching new node
        nd = stackNode.pop();
        *nd = new node(key, item);

        // Correcting tree structure
        while(stackNode.size()) {
            nd = stackNode.pop();

            if (stackDir.pop() == left)
                // Node wass added to the left subtree
            {
                switch ((*nd)->bl) {
                    case rightTilt:
                        (*nd)->bl = noTilt;
                        return true;
                    case noTilt:
                        (*nd)->bl = leftTilt;
                        break;
                    case leftTilt:
                        if ((*nd)->left->bl == rightTilt) AVLCore::LR(*nd);
                        else AVLCore::LL(*nd);
                        return true;
                }
            }
            else
                // Node was added to the right subtree
            {
                switch ((*nd)->bl) {
                    case leftTilt:
                        (*nd)->bl = noTilt;
                        return true;
                    case noTilt:
                        (*nd)->bl = rightTilt;
                        break;
                    case rightTilt:
                        if ((*nd)->right->bl == leftTilt) AVLCore::RL(*nd);
                        else AVLCore::RR(*nd);
                        return true;
                }
            }
        }

        return true;
    }

    struct _inserterRecu {
        _inserterRecu() = delete;
        ~_inserterRecu() = delete;

        template<class KeyWrapperT, class ItemWrapperT>
        static bool insert(KeyWrapperT key, ItemWrapperT item, node*& root) {
            _insert(key, item, root);
            return _result;
        }

    private:

        template<class KeyWrapperT, class ItemWrapperT>
        static void _insert(KeyWrapperT key, ItemWrapperT item, node*& root) {
            // we traversed whole tree, possible to add the element
            if (!root) {
                root = new node(key, item);
                _result = true;
                _wasChanged = true;
                return;
            }

            if (key > *root)
                // right subtree next target
            {
                _insert(key, item, root->right);

                // balanced of the subtree did not change
                if (!_wasChanged) return;

                switch (root->bl) {
                    case leftTilt:
                        root->bl = noTilt;
                        _wasChanged = false;
                        break;
                    case noTilt:
                        root->bl = rightTilt;
                        break;
                    case rightTilt:
                        if (root->right->bl == leftTilt) AVLCore::RL(root);
                        else AVLCore::RR(root);
                        _wasChanged = false;
                        break;
                }
            }
            else if(*root > key)
                // left subtree next target
            {
                _insert(key, item, root->left);

                // balanced of the subtree did not change
                if (!_wasChanged) return;

                switch (root->bl) {
                    case rightTilt:
                        root->bl = noTilt;
                        _wasChanged = false;
                        break;
                    case noTilt:
                        root->bl = leftTilt;
                        break;
                    case leftTilt:
                        if (root->left->bl == rightTilt) AVLCore::LR(root);
                        else AVLCore::LL(root);
                        _wasChanged = false;
                        break;
                }
            }
            else {
                _wasChanged = false;
                _result = false;
            }
        }

        inline static bool _result{};
        inline static bool _wasChanged{};
    };



    // ------------------------------
    // Private class methods
    // ------------------------------

    // ------------------------------
    // Class fields
    // ------------------------------

    size_t _elemCount{};
    node* _root{};
};

#endif //AVLTREE_H
