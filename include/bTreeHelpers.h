//
// Created by Jlisowskyy on 1/11/24.
//

#ifndef BTREEHELPERS_H
#define BTREEHELPERS_H

#include <iostream>
#include <sstream>
#include <queue>
#include <iomanip>

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

template<class nodeT>
void SimplestRecursivePrint(const nodeT* const n, const size_t depth) {
    if (!n) return;

    SimplestRecursivePrint(n->left, depth + 4);
    std::cout << std::string(depth, ' ') << *n << std::endl;
    SimplestRecursivePrint(n->right, depth + 4);
}

template<class nodeT>
void TestQueuePrintRec(const nodeT* n, std::queue<std::pair<const nodeT*, int>>& que, int depth) {
    if (!n) return;

    que.push(std::make_pair(n, depth));
    TestQueuePrintRec(n->left, que, depth+4);
    TestQueuePrintRec(n->right, que, depth+4);
}

template<class nodeT>
void TestQueuePrint(const nodeT* n) {
    std::queue<std::pair<const nodeT* , int>> que{};
    TestQueuePrintRec(n,que, 0);

    while(!que.empty()) {
        auto el = que.front();
        que.pop();

        std::cout << std::string(el.second, ' ') << *el.first << std::endl;
    }
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

template<class nodeT>
class PrettyBTreePrinter {
    // ------------------------------
    // class innerTypes
    // ------------------------------

    struct printInfo {
        printInfo() = default;
        ~printInfo() = default;

        size_t depth{};
        size_t rowInd{};
        const nodeT* node{};

        friend bool operator<(const printInfo& a, const printInfo& b) {
            return (a.depth > b.depth) || (a.depth == b.depth && a.rowInd > b.rowInd);
        }
    };

    // ------------------------------
    // Class creation
    // ------------------------------

public:
    PrettyBTreePrinter() = delete;
    PrettyBTreePrinter(const PrettyBTreePrinter& other) = delete;
    PrettyBTreePrinter& operator=(const PrettyBTreePrinter& other) = delete;

    // ------------------------------
    // Class interaction
    // ------------------------------

    static void SetSpacing(const std::pair<size_t, size_t>& spacings) {
        verticalSpacing = spacings.first;
        horizontalSpacing = spacings.second;
    }

    static std::ostream& PrintWithQueue(std::ostream& out, const nodeT* const root) {
        if (!root) {
            out << "Empty tree\n";
            return out;
        }

        const size_t largestPrint = FindlargestPrintSize(root);
        const size_t maxDepth = FindTreeDepth(root, 0);
        const size_t myhorizontalSpacing = std::max(horizontalSpacing, largestPrint);
        const double lowestRowSize = (1 << maxDepth) * largestPrint + ((1 << maxDepth) - 1) * myhorizontalSpacing;
        const std::string emptySpaceStr(largestPrint, ' ');
        const std::string verSpaceStr(verticalSpacing, '\n');

        std::priority_queue<printInfo> que{};
        _gatherPrintInfo(root, que);

        for (size_t i = 0; i < maxDepth; ++i) {
            const size_t layerElemCount = 1 << i;
            const double layerDivider = 1 << (i + 1);
            const size_t leftOffset = ((double)(lowestRowSize - layerElemCount*largestPrint)) / layerDivider;
            const std::string horOffsetStr(leftOffset, ' ');

            size_t expectedRowIndex = 0;
            while(!que.empty() && que.top().depth == i) {
                auto elem = que.top();
                que.pop();

                while(expectedRowIndex != elem.rowInd) {
                    out << horOffsetStr << emptySpaceStr;
                    ++expectedRowIndex;
                }

                out << horOffsetStr << std::setw(largestPrint) << *elem.node;
            }

            out << std::endl << verSpaceStr;
        }

        const std::string horizontalSpacingStr(myhorizontalSpacing, ' ');
        size_t expectedRowIndex = 0;
        while(!que.empty()) {
            auto elem = que.top();
            que.pop();

            while(expectedRowIndex != elem.rowInd) {
                out << horizontalSpacingStr << emptySpaceStr;
                ++expectedRowIndex;
            }

            out << horizontalSpacingStr << std::setw(largestPrint) << *elem.node;
        }

        return out;
    }

    // expects node to have overloaded printing operator
    static size_t FindlargestPrintSize(const nodeT* const n) {
        if (!n) return 0;
        const size_t lSize = FindlargestPrintSize(n->left);
        const size_t rSize = FindlargestPrintSize(n->right);

        std::ostringstream str{};
        str << n->content.first;

        // chosing maximal size
        return std::max(str.str().length(), std::max(lSize, rSize));
    }

    static size_t FindTreeDepth(const nodeT* const n, const size_t depth) {
        if (!n) return depth;

        const size_t lDepth = FindTreeDepth(n->left, depth + 1);
        const size_t rDepth = FindTreeDepth(n->right, depth + 1);

        return std::max(lDepth, rDepth);
    }

    // ------------------------------
    // Class private methods
    // ------------------------------
private:

    static void _gatherPrintInfo(const nodeT* const n, std::priority_queue<printInfo>& que) {
        if (!n) return;

        printInfo info{};
        info.node = n;
        que.push(info);

        _gatherPrintInfoRec(n->left, que, info);
        _gatherPrintInfoRec(n->right, que, info);
    }

    static void _gatherPrintInfoRec(const nodeT* const n, std::priority_queue<printInfo>& que, const printInfo& pInfo) {
        if (!n) return;

        printInfo info{};

        const size_t offset = n == pInfo.node->left ? 0 : 1;

        info.depth = pInfo.depth + 1;
        info.node = n;
        info.rowInd = 2*pInfo.rowInd + offset;
        que.push(info);

        _gatherPrintInfoRec(n->left, que, info);
        _gatherPrintInfoRec(n->right, que, info);
    }

    // ------------------------------
    // Class private fields
    // ------------------------------

    inline static size_t verticalSpacing = 4;
    inline static size_t horizontalSpacing = 1;
};

#endif //BTREEHELPERS_H
