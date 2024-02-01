//
// Created by Jlisowskyy on 1/31/24.
//

#ifndef PLAINHASHMAP_H
#define PLAINHASHMAP_H

#include "HashFunctions.h"

#include <vector>

template<
    class KeyT,
    class ItemT,
    class HashFuncT = BaseHashFunction<KeyT>
>class _basePlainMapT {
    // ------------------------------
    // Class creation
    // ------------------------------
public:

    explicit _basePlainMapT(const size_t size):
        _items(size, ItemT{}), _occupancyTable(size, false), _size{size}, _hFunc{size}
    {}

    _basePlainMapT(const _basePlainMapT&) = default;
    _basePlainMapT(_basePlainMapT&&) = default;
    _basePlainMapT& operator=(const _basePlainMapT&) = default;
    _basePlainMapT& operator=(_basePlainMapT&&) = default;

    ~_basePlainMapT() = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    [[nodiscard]] bool insert(const KeyT& key, const ItemT& item) {
        const size_t hash = _hFunc(key);

        if (!_occupancyTable[hash]) {
            _items[hash] = item;
            _occupancyTable[hash] = true;

            return true;
        }

        return false;
    }

    [[nodiscard]] bool search(const KeyT& key) const {
        return _occupancyTable[_hFunc(key)];
    }

    [[nodiscard]] bool searchAndSave(const KeyT& key) {
        const size_t hash = _hFunc(key);

        _lastSearch = &_items[hash];
        return _occupancyTable[hash];
    }

    void remove(const KeyT& key) {
        _occupancyTable[_hFunc(key)] = false;
    }

    void deepDelete(const KeyT& key) {
        const size_t hash = _hFunc(key);

        _items[hash] = ItemT{};
        _occupancyTable[_hFunc(key)] = false;
    }

    [[nodiscard]] size_t getSize() const {
        return _size;
    }

    // Should only be used after successful SearchAndSaveOperation
    [[nodiscard]] ItemT& getLastSearched() {
        return *_lastSearch;
    }

    [[nodiscard]] ItemT& operator[](const KeyT& key) {
        return _items[_hFunc(key)];
    }

    [[nodiscard]] const ItemT& operator[](const KeyT& key) const{
        return _items[_hFunc(key)];
    }

    // ------------------------------
    // private fields
    // ------------------------------
private:

    std::vector<ItemT> _items;
    std::vector<bool> _occupancyTable;
    size_t _size;
    HashFuncT _hFunc;

    ItemT* _lastSearch{};
};

template<
    class KeyT,
    class ItemT,
    class HashFuncT = BaseHashFunction<KeyT>
>class _baseExpandiblePlainMapT:
    public _basePlainMapT<KeyT, ItemT, HashFuncT> {
    // ------------------------------
    // Class creation
    // ------------------------------
public:
    _baseExpandiblePlainMapT(): _baseExpandiblePlainMapT(1) {}

    explicit _baseExpandiblePlainMapT(const size_t size):
        _basePlainMapT<KeyT, ItemT, HashFuncT>(size), _keys(size, KeyT{}) {}

    _baseExpandiblePlainMapT(const _baseExpandiblePlainMapT&) = default;
    _baseExpandiblePlainMapT(_baseExpandiblePlainMapT&&) = default;
    _baseExpandiblePlainMapT& operator=(const _baseExpandiblePlainMapT&) = default;
    _baseExpandiblePlainMapT& operator=(_baseExpandiblePlainMapT&&) = default;

    ~_baseExpandiblePlainMapT() = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    using _basePlainMapT<KeyT, ItemT, HashFuncT>::search;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::getItem;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::getLastSearched;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::operator[];
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::remove;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::getSize;

    bool Insert(const KeyT& key, const ItemT& item) {
        const size_t hash = _hFunc(key);

        if (!_occupancyTable[hash]) {
            _items[hash] = item;
            _occupancyTable[hash] = true;
            _keys[hash] = key;

            return true;
        }

        return false;
    }

    bool serachAndSave(const KeyT& key) {
        const size_t hash = _hFunc(key);

        _lastSearch = &_items[hash];
        _lastSearchedKey = &_keys[hash];
        return _occupancyTable[hash];
    }

    void DeepDelete(const KeyT& key) {
        const size_t hash = hashFunc(key);

        _items[hash] = ItemT{};
        _occupancyTable[hashFunc(key)] = false;
        _keys[hash] = KeyT{};
    }

    bool Resize(const size_t nSize, const int maxTries) {
        int tryCount = 0;

        do {
            if (_performHashMapTransfer(nSize)) return true;
        }while(tryCount++ < maxTries);

        return false;
    }

    const KeyT& getLastSearchedKey() const {
        return *_lastSearchedKey;
    }

    // ------------------------------
    // Private methods
    // ------------------------------
private:

    bool _performHashMapTransfer(const size_t nSize) {

        // roll new hashfunc
        auto hashFunc = HashFuncT(nSize);

        // prepare containers
        std::vector<ItemT> nItems(nSize, ItemT{});
        std::vector nOccup(nSize, false);
        std::vector<KeyT> nKeys(nSize, KeyT{});

        // copy all existing elements
        for (size_t i = 0; i < _items.size(); ++i) {
            if (_occupancyTable[i] == false) continue;

            const size_t hash = hashFunc(_keys[i]);

            // collision detected abort
            if (nOccup[hash] == true) {
                return false;
            }

            nItems[hash] = _items[i];
            nOccup[hash] = true;
            nKeys[hash] = _keys[i];
        }

        // setup new parameters
        _size = nSize;
        _hFunc = hashFunc;
        _keys = nKeys;
        _items = nItems;
        _occupancyTable = nOccup;

        return true;
    }

    // ------------------------------
    // private fields
    // ------------------------------

    KeyT* _lastSearchedKey = nullptr;

    std::vector<KeyT> _keys;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::_items;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::_occupancyTable;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::_size;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::_hFunc;
    using _basePlainMapT<KeyT, ItemT, HashFuncT>::_lastSearch;
};

#endif //PLAINHASHMAP_H
