//
// Created by Jlisowskyy on 1/31/24.
//

#ifndef CHAINHASHINGMAP_H
#define CHAINHASHINGMAP_H

#include "plainHashMap.h"
#include "../linkedListHelpers.h"

#include <functional>
#include <climits>
#include <vector>

/*                  IMPORTANT NOTES - BUCKETS:
 *
 *  All acceptable bucket classes should have:
 *  - default constructor,
 *  - "bool insert(const KeyT&, const ItemT&)" method - return values: true - added element, false - already exists,
 *  - "size_t size() const" method returning number of actually stored elements
 *  - "bool search(const KeyT&) const" method - returning true - element does exists inside the bucket,
 *                                              false - element does not exists
 *  - "void remove(const KeyT&)" method - perform removing, allowed without any safety checks, can be same as safe remvoe
 *  - "bool safeRemove(const KeyT&)" method - performs removing only if is sure that key exists inside the bucket
 *  - "ItemT& get(const KeyT&)" method - returns matching element. It can assume that element exists.
 *  - "ItemT& safeGet(const KeyT&)" method - returns matching element if exists if not create empty slot under passed keys and returns reference to this slot
 *  - "static std::vector<BucketT>reorganizeBuckets(std::vector<BucketT>, size_t, HashFuncT)" -
 *      reorganizes buckets passed as parameter to a new set of buckets of size passed as an argument
 *      with manner defined by passed hash function. Returns new organized vector
 *
 */

/*                  GENRAL TODOS:
 *  - implement other bucket types?
 *  - add automatical celling to power2 size in constructor
 *  - AVX support?
 */

template <
    class KeyT,
    class ItemT,
    class ComparerT,
    class HashFuncT = BaseHashFunction<KeyT, true>
>class PlainHashBucketT{
    // ------------------------------
    // Class creation
    // ------------------------------
public:

    PlainHashBucketT(): _map(DefaultBucketSize) {};

    PlainHashBucketT(const PlainHashBucketT&) = default;
    PlainHashBucketT(PlainHashBucketT&&) = default;
    PlainHashBucketT& operator=(const PlainHashBucketT&) = default;
    PlainHashBucketT& operator=(PlainHashBucketT&&) = default;

    ~PlainHashBucketT() = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    bool insert(const KeyT& key, const ItemT& item) {
        if (_map.searchAndSave(key) && _comp(_map.getLastSearchedKey(), key)) return false;

        _insert(key, item);
        return true;
    }

    [[nodiscard]] size_t size() const {
        return _elemCount;
    }

    [[nodiscard]] bool search(const KeyT& key) const {
        return _map.searchAndSave(key) && _comp(_map.getLastSearchedKey(), key);
    }

    // NOTE: key must be contained in the bucket
    void remove(const KeyT& key) {
        _map.remove(key);
        --_elemCount;
    }

    bool safeRemove(const KeyT& key) {
        if (!search(key)) return false;
        _map.remove(key);
        --_elemCount;
        return true;
    }

    [[nodiscard]] ItemT& get(const KeyT& key) {
        return _map[key];
    }

    [[nodiscard]] ItemT& safeGet(const KeyT& key) {
        if (_map.searchAndSave(key)) return _map.getLastSearched();

        _map.insert(key, ItemT{});
        return _map[key];
    }

    // TODO: Possible boost gained with AVX
    template<class OuterHashFuncT>
    static std::vector<PlainHashBucketT> reorganizeBuckets(std::vector<PlainHashBucketT> oldBuckets, size_t nSize, OuterHashFuncT nFunc) {
        std::vector<PlainHashBucketT> nBuckets(nSize);

        for (const auto& oBucket : oldBuckets) {
            const auto& [ oKeys, oItems, occup ] = oBucket._map.getUnderlyingArrays();

            for (size_t i = 0; i < occup.size(); ++i)
                if (occup[i] == true) {
                    const size_t hash = nFunc(oKeys[i]);
                    nBuckets[hash]._insert(oKeys[i], oItems[i]);
                }
        }

        return nBuckets;
    }


    // ------------------------------
    // Private methods
    // ------------------------------
private:

    void _insert(const KeyT& key, const ItemT& item) {
        if (++_elemCount == _nextResize) {
            _nextResize++;
            _map.resizeUnconditionally(_map.getSize() * DefaultResizeCoef);
        }

        while (!_map.insert(key, item)) {
            _nextResize++;
            _map.resizeUnconditionally(_map.getSize() * DefaultResizeCoef);
        } // also performs rehashing
    }


    // ------------------------------
    // class fields
    // ------------------------------
public:
    static constexpr size_t DefaultBucketSize = 4;
    static constexpr size_t DefaultResizeCoef = 2;
    static constexpr size_t StartResizeTrehsold = 3;
private:
    size_t _elemCount = 0;
    size_t _nextResize = StartResizeTrehsold;

    _baseExpandiblePlainMapT<KeyT, ItemT, HashFuncT> _map{};

    inline static ComparerT _comp{};
};

template <
    class KeyT,
    class ItemT,
    class ComparerT
> class LinkedListBucketT {
    // ------------------------------
    // Inner types
    // ------------------------------

    struct node {
        node() = default;
        ~node() = default;
        node(const KeyT& key, const ItemT& item, node* nNext = nullptr): next(nNext), _item(item), _key(key) {}
        node(const node& other): _item(other._item), _key(other._key){}

        node* next{};
        ItemT _item;
        KeyT _key;
    };

    // ------------------------------
    // Class creation
    // ------------------------------
public:

    LinkedListBucketT(): _root( new node{} ) {}

    LinkedListBucketT(const LinkedListBucketT& other):
        _elemCount(other._elemCount)
    {
        _root = cloneList(other._root);
    }

    LinkedListBucketT(LinkedListBucketT&& other) noexcept:
        _elemCount(other._elemCount), _root(other._root)
    {
        other._root = nullptr;
        other._elemCount = 0;
    }

    LinkedListBucketT& operator=(const LinkedListBucketT& other) {
        if (&other == this) return *this;

        cleanList(_root);
        _root = cloneList(other._root);
        _elemCount = other._elemCount;

        return *this;
    }

    LinkedListBucketT& operator=(LinkedListBucketT&& other) noexcept {
        if (&other == this) return *this;

        cleanList(_root);
        _root = other._root;
        _elemCount = other._elemCount;

        other._root = nullptr;
        other._elemCount = 0;

        return *this;
    }

    ~LinkedListBucketT() {
        cleanList(_root);
    }

    // ------------------------------
    // Class interaciton
    // ------------------------------

    bool insert(const KeyT& key, const ItemT& item) {
        node* root = _root;

        // chech for existance of key inside list
        while((root = root->next)) {
            if (_comp(root->_key, key)) return false;
        }

        // if is key is not present in the list, add it
        node* nNode = new node(key, item, _root->next);
        _root->next = nNode;
        ++_elemCount;

        return true;
    }

    [[nodiscard]] size_t size() const {
        return _elemCount;
    }

    [[nodiscard]] bool search(const KeyT& key) const {
        node* root = _root;

        // look through all nodes
        while((root = root->next)) {
            if (_comp(root->key, key)) return true;
        }

        return false;
    }

    bool safeRemove(const KeyT& key) {
        node** root = _root;

        // find key inside the list
        while((root = &(*root)->next)) {

            // remove the key
            if (_comp((*root)->key, key)) {
                node* toRemove = *root;
                *root = toRemove->next;
                delete toRemove;
                --_elemCount;
                return true;
            };
        }

        return false;
    }

    [[nodiscard]] ItemT& safeGet(const KeyT& key) {
        node* root = _root;

        // chech for existance of key inside list
        while((root = root->next)) {
            if (_comp(root->_key, key)) return root->_item;
        }

        // if is key is not present in the list, add it
        node* nNode = new node(key, ItemT(), _root->next);
        _root->next = nNode;
        ++_elemCount;

        return nNode->_item;
    }

    [[nodiscard]] ItemT& get(const KeyT& key) {
        return safeGet(key);
    }

    void remove(const KeyT& key) {
        safeRemove(key);
    }

    template<class OuterHashFuncT>
    [[nodiscard]] static std::vector<LinkedListBucketT> reorganizeBuckets(std::vector<LinkedListBucketT> oldBuckets, size_t nSize, OuterHashFuncT nFunc) {
        std::vector<LinkedListBucketT> nBuckets(nSize);

        for (auto& oBucket : oldBuckets) {
            for (size_t i = 0; i < oBucket.size(); ++i) {
                node* n = oBucket._detachFirst();

                const size_t hash = nFunc(n->_key);
                nBuckets[hash]._attachFirst(n);
            }
        }

        return nBuckets;
    }

    // ------------------------------
    // class private methods
    // ------------------------------
private:

    // does not need elem count incerement since its used only in old bucket, ready to destroy
    [[nodiscard]] node* _detachFirst() {
        node* n = _root->next;
        _root->next = n->next;
        return n;
    }

    // does actually need _elemCount increment because is used to transfer old nodes to new buckets
    void _attachFirst(node* n) {
        n->next = _root->next;
        _root->next = n;
        ++_elemCount;
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    size_t _elemCount {};
    node* _root;
    inline static ComparerT _comp{};
};

template<
    class KeyT,
    class ItemT,
    class ComparerT = std::equal_to<KeyT>,
    class HashFuncT = BaseHashFunction<KeyT, true>,
    class BucketT = PlainHashBucketT<KeyT, ItemT, ComparerT>
> class _chainHashingMapT {
    // ------------------------------
    // class creation
    // ------------------------------
public:

    _chainHashingMapT(): _chainHashingMapT(InitMapSize) {}
    explicit _chainHashingMapT(const size_t size):
        _hFunc{size}, _nextUpScaleResize{static_cast<size_t>(size * _rehashPolicy)},
        _nextDownScaleResize{size / DefaultDowscaleFactor}, _buckets(size) {}

    _chainHashingMapT(const _chainHashingMapT&) = default;
    _chainHashingMapT(_chainHashingMapT&&) = default;
    _chainHashingMapT& operator=(const _chainHashingMapT&) = default;
    _chainHashingMapT& operator=(_chainHashingMapT&&) = default;

    ~_chainHashingMapT() = default;

    // ------------------------------
    // class interaction
    // ------------------------------

    bool insert(std::pair<KeyT, ItemT> pair) {
        const auto& [key, item] = pair;
        return insert(key, item);
    }

    bool insert(const KeyT& key, const ItemT& item) {
        const size_t hash = _hFunc(key);

        if (!_buckets[hash].insert(key, item)) return false;

        if (++_elemCount > _nextUpScaleResize) _resize(_buckets.size() * 2);
        return true;
    }

    [[nodiscard]] bool search(const KeyT& key) const {
        return _buckets[_hFunc(key)].search(key);
    }

    // Note: element MUST be contained, otherwise behavior is undefined
    void remove(const KeyT& key) {
        _buckets[_hFunc(key)].remove(key);

        if (--_elemCount < _nextDownScaleResize)
            _resize(_buckets.size() / 2);
    }

    bool safeRemove(const KeyT& key) {
        const bool result = _buckets[_hFunc(key)].safeRemove(key);

        if (result && --_elemCount < _nextDownScaleResize)
            _resize(_buckets.size() / 2);

        return result;
    }

    [[nodiscard]] size_t size() const {
        return _elemCount;
    }

    [[nodiscard]] float load_factor() const {
        return static_cast<float>(_elemCount) / _buckets.size();
    }

    [[nodiscard]] size_t getMaxBucketSize() const {
        return _buckets.size();
    }

    [[nodiscard]] float max_load_factor() const {
        return _rehashPolicy;
    }

    void max_load_factor(float nFactor) {
        _rehashPolicy = nFactor;
    }

    // Note: should only be used when preparing structure to be used in future without insertion and deletion
    bool rehash(const float desiredAverageBucketLoadRatio = 1.5, const int maxTries = 3) {
        int tries = 0;

        while(getAverageUsedBucketLoad() > desiredAverageBucketLoadRatio && tries++ < maxTries) {
            _rehash(_buckets.size());
        }

        return getAverageUsedBucketLoad() < desiredAverageBucketLoadRatio;
    }

    [[nodiscard]] ItemT& get(const KeyT& key) {
        return _buckets[_hFunc(key)].get(key);
    }

    [[nodiscard]] ItemT& operator[](const KeyT& key) {
        return _buckets[_hFunc(key)].safeGet(key);
    }

    [[nodiscard]] ItemT& operator[](const KeyT& key) const {
        return _buckets[_hFunc(key)].safeGet(key);
    }

    [[nodiscard]] size_t getMaximalBucketLoad() const {
        size_t max = 0;

        for (const auto& bucket : _buckets)
            if (const size_t size = bucket.size(); size > max) max = size;

        return max;
    }

    [[nodiscard]] double getAverageUsedBucketLoad() const {
        double usedBuckets = 0;
        for (const auto& bucket : _buckets)
            if (bucket.size() != 0) ++usedBuckets;

        return _elemCount / usedBuckets;
    }

    // ------------------------------
    // class private methods
    // ------------------------------

private:
    void _resize(const size_t nSize) {
        _nextUpScaleResize = static_cast<size_t>(nSize * _rehashPolicy);
        _rehash(nSize);
    }

    void _rehash(const size_t size) {
        _hFunc = HashFuncT(size);
        _buckets = std::move(BucketT::reorganizeBuckets(_buckets, size, _hFunc));
    }

    // ------------------------------
    // class fields
    // ------------------------------
public:
    static constexpr double DefaultRehashPolicy = 1.0;
    static constexpr size_t DefaultDowscaleFactor = 4;
    static constexpr size_t InitMapSize = 8;
private:
    HashFuncT _hFunc;

    float _rehashPolicy = DefaultRehashPolicy; // number used to deduce _nextRehash barrier

    size_t _nextUpScaleResize; // next barrier, which overloading concludes to full rehash
    size_t _nextDownScaleResize;
    size_t _elemCount{}; // actual existing items in container

    std::vector<BucketT> _buckets{};
};

#endif //CHAINHASHINGMAP_H
