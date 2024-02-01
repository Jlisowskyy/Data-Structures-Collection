//
// Created by Jlisowskyy on 2/1/24.
//

#ifndef BSTTREE_H
#define BSTTREE_H

#include "../bTreeHelpers.h"

template<
    class KeyT,
    class ItemT,
    class PredT
>class BTreeT {
    // ------------------------------
    // Class inner types
    // ------------------------------

    using node = basicNode<KeyT, ItemT, PredT>;

    // ------------------------------
    // Class creation
    // ------------------------------
public:

    BTreeT() = default;

    BTreeT(const BTreeT& other): _elemCount(other._elemCount) {
        _root = CloneTree(other._root);
    }

    BTreeT(BTreeT&& other) noexcept : _root(other._root), _elemCount(other._elemCount) {}

    BTreeT& operator=(const BTreeT& other) {
        if (this == &other) return *this;

        CleanTree(_root);
        _root = CloneTree(other._root);
        _elemCount = other._elemCount;

        return *this;
    }

    BTreeT& operator=(BTreeT&& other) noexcept {
        if (this == &other) return *this;

        CleanTree(_root);
        _root = other._root;
        _elemCount = other._elemCount;

        other._root = nullptr;
        other._elemCount = 0;

        return *this;
    }

    ~BTreeT() {
        CleanTree(_root);
    }

    // ------------------------------
    // Class interaction
    // ------------------------------

    bool insert(const KeyT& key, const ItemT& item) {
        return _insert(key,  item);
    }

    bool insert(const std::pair<KeyT, ItemT>& pair) {
        const auto& [key, item] = pair;
        return _insert(key, item);
    }

    bool insert(std::pair<KeyT, ItemT>&& pair) {
        return _insertMove(pair);
    }

    bool insert(node* n) {
        return _insertNode(n);
    }

    bool remove(const KeyT& key) {
        node** n = _searchDP(key);
        if (!*n) return false;

        _delete(n);

        return true;
    }

    node* removeAndGetNode(const KeyT& key) {
        node** n = _searchDP(key);
        if (!*n) return nullptr;

        _delete(n);
        return *n;
    }

    bool removeAndGet(const KeyT& key, ItemT& out) {
        node** n = _searchDP(key);
        if (!*n) return false;

        out = std::move((*n)->content.second);
        _delete(n);

        return true;
    }

    // ------------------------------
    // Private class methods
    // ------------------------------
private:

    node** _searchDP(const KeyT& key) const {
        node** n = _root;

        while(*n) {
            if (*n < key) n = &(*n)->right;
            else if (*n > key) n = &(*n)->left;
            else return n;
        }

        return n;
    }

    bool _insertMove(std::pair<KeyT, ItemT>&& pair) {
        node** n =_searchDP(pair.first);
        if (*n) return false;

        *n = new node(std::move(pair));
        return true;
    }

    bool _insert(const KeyT& key, const ItemT& item) {
        node** n =_searchDP(key);
        if (*n) return false;

        *n = new node(item, key);
        return true;
    }

    bool _insertNode(node* toAdd) {
        node** n =_searchDP(toAdd->content.first);
        if (*n) return false;

        *n = toAdd;
        toAdd->left = toAdd->right = nullptr;
        return true;
    }

    inline static u_char _treeToRemove = 0;
    void _delete(node** n) {
        _treeToRemove = 1 - _treeToRemove;
        switch (_treeToRemove) {
            case 0:
                _deleteLeftSubtree(n);
            break;
            case 1:
                _deleteRightSubtree(n);
            break;
        }
    }

    // Note: *root cannot be null
    static void _deleteLeftSubtree(node** n) {
        node* toRemove = *n;

        if (toRemove->left) {
            node* biggest = _extractBiggest(&toRemove->left);
            biggest->left = toRemove->left;
            biggest->right = toRemove->right;
            *n = biggest;
        }
        else if (toRemove->right) {
            node* smallest = _extractSmallest(&toRemove->right);
            smallest->left = toRemove->left;
            smallest->right = toRemove->right;
            *n = smallest;
        }
        else *n = nullptr;

        delete toRemove;
    }

    // Note: *root cannot be null
    static void _deleteRightSubtree(node** n) {
        node* toRemove = *n;

        if (toRemove->right) {
            node* smallest = _extractSmallest(&toRemove->right);
            smallest->left = toRemove->left;
            smallest->right = toRemove->right;
            *n = smallest;
        }
        else if (toRemove->left) {
            node* biggest = _extractBiggest(&toRemove->left);
            biggest->left = toRemove->left;
            biggest->right = toRemove->right;
            *n = biggest;
        }
        else *n = nullptr;

        delete toRemove;
    }

    // Note: *root cannot be null
    static node* _extractSmallest(node** root) {
        node** smallestParent = _findSmallest(root);
        node* smallest = *smallestParent;

        *smallestParent = smallest->right;
        return smallest;
    }

    // Note: *root cannot be null
    static node* _extractBiggest(node** root) {
        node** biggestParent = _findBiggest(root);
        node* biggest = *biggestParent;

        *biggestParent = biggest->left;
        return biggest;
    }

    // Note: *root cannot be null
    static node** _findSmallest(node** root) {
        while((*root)->left) root = &(*root)->left;
        return root;
    }

    // Note: *root cannot be null
    static node** _findBiggest(node** root) {
        while((*root)->right) root = &(*root)->right;
        return root;
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    node* _root{};
    size_t _elemCount{};
};

#endif //BSTTREE_H