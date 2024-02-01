//
// Created by Jlisowskyy on 1/31/24.
//

#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include <random>
#include <chrono>

static constexpr size_t SIZE_ONE = 1;
static constexpr size_t SIZE_ZERO = 0;

constexpr size_t POW2FAST(const int degree) {
    return SIZE_ONE << degree;
}

static constexpr size_t primePer2Power[] = {
    POW2FAST(16)-17, // 0-2^8
    POW2FAST(24)-17, // 2^8-2^16
    POW2FAST(32)-17, // 2^16-2^24
    POW2FAST(40)-87, // 2^24-2^32
    POW2FAST(48)-59, // 2^32-2^40
    POW2FAST(56)-5, // 2^40-2^48
    SIZE_ZERO-59, // 2^48-2^56
};

inline int extractMSB(const size_t x)
    // IMPORTANT: x cannot be 0, it means undefined behavior
{
    static constexpr int maxB = sizeof(size_t) * 8 - 1;
    static constexpr size_t start = SIZE_ONE << maxB;

    int bit = maxB;
    size_t iter = start;

    while((x & iter) == 0) {
        --bit;
        iter >>= 1;
    }

    return bit;
}

template<
    class KeyT,
    bool OptimizeSecondModulo = false,
    size_t (*HashableAccessor)(const KeyT& item) = [](const KeyT& item) { return static_cast<size_t>(item); }
>class BaseHashFunction {

    /*               Description
     *
     *  Works accordinly to formula:
     *  h(v) = ((a*b + b) mod prime) mod size
     *
     *  Note: formula enforces usage of 2 modulo operations
     *
     *  When optimze flag is enabled, size is expected to be power of 2,
     *  otherwise behavior is undefined.
     *
     *  Updated fomrula:
     *
     *  h(v) = ((a*b + b) mod prime) & sizeMask
     */

    // ------------------------------
    // Class creation
    // ------------------------------
public:
    explicit BaseHashFunction(const size_t size):
        _sizeMod{_initSizeMod(size)}
    {
        RollParameteres();

        const int msbBit = extractMSB(size);
        _prime = primePer2Power[msbBit/8];
    }

    explicit BaseHashFunction(const size_t size, const size_t prime):
        _prime{prime}, _sizeMod{_initSizeMod(size)}
    {
        RollParameteres();
    }

    BaseHashFunction(const BaseHashFunction&) = default;
    BaseHashFunction(BaseHashFunction&&) = default;
    BaseHashFunction& operator=(const BaseHashFunction&) = default;
    BaseHashFunction& operator=(BaseHashFunction&&) = default;

    ~BaseHashFunction() = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    void RollParameteres() {
        if constexpr (sizeof(size_t) == 8) {
            static std::mt19937_64 randEngine_64{
                static_cast<size_t>(std::chrono::steady_clock::now().time_since_epoch().count())
            };

            _a = randEngine_64();
            _b = randEngine_64();
        }

        if constexpr (sizeof(size_t) == 4) {
            static std::mt19937 randEngine_32{
                static_cast<size_t>(std::chrono::steady_clock::now().time_since_epoch().count())
            };

            _a = randEngine_32();
            _b = randEngine_32();
        }
    }

    size_t GetMaxVal() const {
        return _prime-1;
    }

    size_t operator()(const KeyT& val) const {
        const size_t hashable = HashableAccessor(val);

        if constexpr (OptimizeSecondModulo)
            return ((_a*hashable + _b) % _prime) & _sizeMod;
        else
            return ((_a*hashable + _b) % _prime) % _sizeMod;
    }

    // ------------------------------
    // Class private methods
    // ------------------------------
private:

    static size_t _initSizeMod(const size_t size){
        if constexpr (OptimizeSecondModulo)
            return size - 1;
        else
            return size;
    }


    // ------------------------------
    // Class fields
    // ------------------------------
    size_t _a;
    size_t _b;
    size_t _prime;
    size_t _sizeMod;
};

struct IdentityHash {
    IdentityHash() = default;
    explicit IdentityHash([[maybe_unused]] size_t) {}
    ~IdentityHash() = default;

    size_t operator()(const size_t x) const { return x; }
};

#endif //HASHFUNCTIONS_H
