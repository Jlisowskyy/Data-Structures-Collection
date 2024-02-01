//
// Created by Jlisowskyy on 1/31/24.
//

#ifndef CHAINHASHINGMAP_H
#define CHAINHASHINGMAP_H

#include "plainHashMap.h"

#include <functional>
#include <limits.h>
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
 *
 *  - TODO static
 *  - TODO safeget?
 *
 */

template <
    class KeyT,
    class ItemT,
    class ComparerT,
    class HashFuncT = BaseHashFunction<KeyT>
>class PlainHashBucketT{
    // ------------------------------
    // Class creation
    // ------------------------------
public:

    PlainHashBucketT(): _map(DefaultBucketSize) {};

    // ------------------------------
    // Class interaction
    // ------------------------------

    bool insert(const KeyT& key, const ItemT& item) {
        if (_map.searchAndSave(key) && _comp(_map.getLastSearchedKey(), key)) return false;

        _insert(key, item);
        return true;
    }

    size_t size() const {
        return _elemCount;
    }

    bool search(const KeyT& key) const {
        return _map.searchAndSave(key) && _comp(_map.getLastSearchedKey(), key);
    }


    void remove(const KeyT& key) {
        _map.remove(key);
    }

    bool safeRemove(const KeyT& key) {
        if (!search(key)) return false;
        _map.remove(key);
        return true;
    }

    ItemT& get(const KeyT& key) {
        _map[key];
    }


    // ------------------------------
    // Private methods
    // ------------------------------
private:

    void _insert(const KeyT& key, const ItemT& item) {
        if (++_elemCount == _nextResize) {
            _nextResize++;
            _map.Resize(_map.getSize() * DefaultResizeCoef, INT_MAX);
        }

        while (!_map.Insert(key, item)) _map.Resize(_map.getSize()); // also performs rehashing
    }


    // ------------------------------
    // class fields
    // ------------------------------
public:
    static constexpr size_t DefaultBucketSize = 4;
    static constexpr size_t DefaultResizeCoef = 2;
    static constexpr size_t StartResizeTrehsold = 2;
private:
    size_t _elemCount = 0;
    size_t _nextResize = StartResizeTrehsold;

    _baseExpandiblePlainMapT<KeyT, ItemT, HashFuncT> _map{};

    static ComparerT _comp{};
};

template <
    class KeyT,
    class ItemT,
    class ComparerT
> class LinkedListBucketT {

};

template<
    class KeyT,
    class ItemT,
    class ComparerT = std::equal_to<KeyT>,
    class HashFuncT = BaseHashFunction<KeyT>,
    class BucketT = PlainHashBucketT<KeyT, ItemT, ComparerT>
> class _chainHashingMapT {
    // ------------------------------
    // class creation
    // ------------------------------
public:

    _chainHashingMapT(): _chainHashingMapT(InitMapSize) {}
    explicit _chainHashingMapT(const size_t size):
        _hFunc{size}, _nextRehash{static_cast<size_t>(size * _rehashPolicy)}, _buckets(size, BucketT{}) {}

    _chainHashingMapT(const _chainHashingMapT&) = default;
    _chainHashingMapT(_chainHashingMapT&&) = default;
    _chainHashingMapT& operator=(const _chainHashingMapT&) = default;
    _chainHashingMapT& operator=(_chainHashingMapT&&) = default;

    ~_chainHashingMapT() = default;

    // ------------------------------
    // class interaction
    // ------------------------------

    bool insert(const KeyT& key, const ItemT& item) {
        const size_t hash = _hFunc(key);

        if (!_buckets[hash].insert(key, item)) return false;
        _bucketCount += _buckets[hash].size() == 1 ? 1 : 0;

        if (_elemCount > _nextRehash) _resize();
        return true;
    }

    [[nodiscard]] bool search(const KeyT& key) const {
        return _buckets[_hFunc(key)].search(key);
    }

    void remove(const KeyT& key) {
        _buckets[_hFunc(key)].remove(key);
    }

    bool safeRemove(const KeyT& key) {
        return _buckets[_hFunc(key)].safeRemove(key);
    }

    [[nodiscard]] size_t getBucketCount() const {
        return _bucketCount;
    }

    [[nodiscard]] size_t size() const {
        return _elemCount;
    }

    [[nodiscard]] float getElemPerBucket() const {
        return static_cast<float>(_elemCount) / static_cast<float>(_bucketCount);
    }

    [[nodiscard]] size_t getMaxBucketSize() const {
        return _buckets.size();
    }

    float& accessRehashPolicy() {
        return _rehashPolicy;
    }

    bool rehash(const float desiredBucketRatio = 1.5, const int maxTries = 3) {
        int tries = 0;

        while(getElemPerBucket() > desiredBucketRatio && tries++ < maxTries) {
            _rehash(_buckets.size());
        }

        return getElemPerBucket() < desiredBucketRatio;
    }

    [[nodiscard]] ItemT& operator[](const KeyT& key) {
        return _buckets[_hFunc(key)].get(key);
    }

    [[nodiscard]] ItemT& operator[](const KeyT& key) const {
        return _buckets[_hFunc(key)].get(key);
    }

    // ------------------------------
    // class private methods
    // ------------------------------

private:
    void _resize() {
        const size_t nSize = _buckets.size() * 2;
        _nextRehash = static_cast<size_t>(nSize * _rehashPolicy);
        _rehash(size);
    }

    void _rehash(const size_t size) {
        _hFunc = HashFuncT(size);
        auto [buckets, activeBuckets] = BucketT::ReoarganizeBuckets(_buckets, size, _hFunc);

        _buckets = buckets;
        _bucketCount = activeBuckets;
    }

    // ------------------------------
    // class fields
    // ------------------------------
public:
    static constexpr double DefaultRehashPolicy = 1.0;
    static constexpr size_t InitMapSize = 8;
private:
    HashFuncT _hFunc;

    float _rehashPolicy = DefaultRehashPolicy; // number used to deduce _nextRehash barrier

    size_t _nextRehash; // next barrier, which overloading concludes to full rehash
    size_t _elemCount{}; // actual existing items in container
    size_t _bucketCount{}; // number of active buckets

    std::vector<BucketT> _buckets{};
};

#endif //CHAINHASHINGMAP_H
