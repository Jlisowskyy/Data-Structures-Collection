//
// Created by Jlisowskyy on 1/12/24.
//

#ifndef DEPTH1MAPT_H
#define DEPTH1MAPT_H

template<
    class keyT,
    class itemT,
    const keyT* alphabet = nullptr, // consider yield like function to place here???
    bool expandTo2Pow = false
>class depth1MapT {
    // ------------------------------
    // Inner types
    // ------------------------------

    template<bool pow2expand = false, bool isApriori = false>
    class HashFunction {
        HashFunction() = default;
        ~HashFunction() = default;

    public:
        size_t operator()(keyT key) const {

        }

        size_t _a{};
        size_t _b{};
        size_t _moduloPrime{};
        size_t _size{};
    };
    // ------------------------------
    // Class creation
    // ------------------------------
public:
    constexpr depth1MapT() {

    }

    // ------------------------------
    // Class fields
    // ------------------------------
private:


    static size_t
};

#endif //DEPTH1MAPT_H
