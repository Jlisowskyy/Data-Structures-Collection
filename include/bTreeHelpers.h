//
// Created by Jlisowskyy on 1/11/24.
//

#ifndef BTREEHELPERS_H
#define BTREEHELPERS_H

#include <iostream>
#include <sstream>

template<class keyT, class itemT, class predT>
struct basicNode {
    std::pair<keyT, itemT> content;

    basicNode* left{};
    basicNode* right{};

    basicNode() = default;
    basicNode(const basicNode& other): content{other.content} {}
    explicit basicNode(const std::pair<keyT, itemT>& pair): content{pair} {}
    basicNode(const itemT& nItem, const keyT& nkey): content{nItem, nkey} {}
    ~basicNode() = default;

    friend bool operator>(const basicNode& a, const basicNode& b) {
        return pred(a.content.first, b.content.first);
    }

    friend bool operator>(const basicNode& a, const keyT& b) {
        return pred(a.content.first, b);
    }

    friend bool operator>(const keyT& a, const basicNode& b) {
        return pred(a, b.content.first);
    }

    friend bool operator==(const basicNode& a, const basicNode& b){
        return !(a > b || b > a);
    }

    friend bool operator==(const keyT& a, const basicNode& b) {
        return !(a > b || b > a);
    }

    friend bool operator==(const basicNode& a, const keyT& b) {
        return !(a > b || b > a);
    }

    friend bool operator!=(const basicNode& a, const basicNode& b){
        return !(a==b);
    }

    friend bool operator!=(const keyT& a, const basicNode& b) {
        return !(a==b);
    }

    friend bool operator!=(const basicNode& a, const keyT& b) {
        return !(a==b);
    }

    friend std::ostream& operator<<(std::ostream& out, const basicNode& n) {
        return out << n.content.first;
    }

private:
    static constexpr predT pred{};
};

// expects node to have overloaded printing operator
template<class nodeT>
size_t FindlargestPrintSize(const nodeT* const n) {
    if (!n) return 0;
    const size_t lSize = FindlargestPrintSize(n->left);
    const size_t rSize = FindlargestPrintSize(n->right);

    std::ostringstream str{};
    str << n->content.first;

    // chosing maximal size
    return std::max(str.str().length(), std::max(lSize, rSize));
}

template<class nodeT>
void SimplestRecursivePrint(const nodeT* const n, const size_t depth) {
    if (!n) return;

    SimplestRecursivePrint(n->left, depth + 4);
    std::cout << std::string(depth, ' ') << *n << std::endl;
    SimplestRecursivePrint(n->right, depth + 4);
}

template<class nodeT>
void CleanTree(const nodeT* n) {
    if (!n) return;

    CleanTree(n->left);
    CleanTree(n->right);

    delete n;
}

template<class nodeT>
nodeT* CloneTree(const nodeT* n) {
    if (!n) return nullptr;

    auto* retNode = new nodeT(*n);

    retNode->left = CloneTree(n->left);
    retNode->right = CloneTree(n->right);

    return retNode;
}


#endif //BTREEHELPERS_H
