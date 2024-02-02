//
// Created by Jlisowskyy on 1/10/24.
//

#ifndef SPLAY_H
#define SPLAY_H

#include <cstdlib>
#include <utility>

#include "_SplayCore.h"
#include "../bTreeHelpers.h"

template<
    class KeyT,
    class ItemT,
    class PredT = std::greater<KeyT>
> class SplayTreeT {
    // ------------------------------
    // Class inner types
    // ------------------------------

    using mPair = std::pair<KeyT, ItemT>;
    using node = basicNode<KeyT, ItemT, PredT>;
    using SplayCore = _SplayCore<KeyT, node>;

    // ------------------------------
    // Class creation
    // ------------------------------
public:

    SplayTreeT() = default;

    template<class indexableT>
    SplayTreeT(const indexableT& indexableContainer, const size_t size) {
        for (size_t i = 0; i < size; ++i)
            insert(indexableContainer[i]);
    }

    SplayTreeT(std::initializer_list<mPair> init) {
        for (auto pair: init) {
            insert(pair);
        }
    }

    SplayTreeT(const SplayTreeT& other): _root{CloneTree(other._root)} {}
    SplayTreeT(SplayTreeT&& other) noexcept: _root{other._root} {
        other._root = nullptr;
    }

    SplayTreeT& operator=(const SplayTreeT& other) {
        if (this == &other) return *this;

        CleanTree(_root);
        _root = CloneTree(other._root);

        return *this;
    }

    SplayTreeT& operator=(SplayTreeT&& other)  noexcept {
        if (this == &other) return *this;

        CleanTree(_root);
        _root = other._root;
        other._root = nullptr;

        return *this;
    }

    ~SplayTreeT() {
        CleanTree(_root);
    }

    // ------------------------------
    // Class public methods
    // ------------------------------

    void insert(const KeyT& key, const ItemT& item) {
        if (!_root) _root = new node(std::make_pair(key, item));
        else _insert(std::make_pair(key, item));
    }

    void insert(const mPair&  pair) {
        if (!_root) _root = new node(pair);
        else _insert(pair);
    }

    [[nodiscard]] bool contains(const KeyT& key) {
        if (!_root) return false;
        SplayCore::splay(key, _root);

        return key == *_root ? true : false;
    }

    // Note: tree cannot be empty and should containt desired element
    [[nodiscard]] ItemT& get(const KeyT& key) {
        SplayCore::splay(key, _root);
        return _root->content.second;
    }

    [[nodiscard]] ItemT& safeGet(const KeyT& key) {
        if (!_root) _root = new node(key, ItemT{});
        else SplayCore::splay(key, _root);

        return _root->content.second;
    }

    [[nodiscard]] ItemT& operator[](const KeyT& key) {
        return safeGet(key);
    }

    [[nodiscard]] const ItemT& operator[](const KeyT& key) const {
        return safeGet(key);
    }

    [[nodiscard]] bool isEmpty() const { return _root != nullptr; }

    // Should only be invoked after Contains returned true or we checked that structure is not empty
    [[nodiscard]] ItemT& getLastSearched() {
        return _root->content.second;
    }

    // Should only be invoked after Contains returned true or we checked that structure is not empty
    [[nodiscard]] const ItemT& getLastSearched() const {
        return _root->content.second;
    }

    bool remove(const KeyT& key) {
        return _root == nullptr ? false : _delete(key);
    }

    friend std::ostream& operator<<(std::ostream& out, const SplayTreeT& tree) {
        return PrettyBTreePrinter<node>::PrintWithQueue(out, tree._root);
    }

    // ------------------------------
    // Private class methods
    // ------------------------------
private:

    // TODO: comments
    bool _delete(const KeyT& key) {
        SplayCore::splay(key, _root);

        if (key != *_root) return false;
        const node* n = _root;

        if (!_root->left) {
            _root = _root->right;
        }
        else {
            SplayCore::splay(key, _root->left);
            _root->left->right = _root->right;
            _root = _root->left;
        }

        delete n;
        return true;
    }

    void _insert(const mPair& pair) {
        // Structure reconstruction
        SplayCore::splay(pair.first, _root);

        if (pair.first > *_root)
            // old root pushed into left subtree
        {
            node* n = new node(pair);

            // moving root to desired place
            n->left = _root;
            n->right = _root->right;
            _root->right = nullptr;

            // assigning new root
            _root = n;
        }
        else if (*_root > pair.first)
            // old root pushed into right subtree
        {
            node* n = new node(pair);

            // moving root to desired place
            n->right = _root;
            n->left = _root->left;
            _root->left = nullptr;

            // assigning new root
            _root = n;
        }
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    node* _root{};
};

#endif //SPLAY_H