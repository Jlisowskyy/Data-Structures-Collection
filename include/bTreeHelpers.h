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
    explicit basicNode(std::pair<keyT, itemT>&& pair): content(std::move(pair)) {}
    basicNode(const itemT& nItem, const keyT& nkey): content{nItem, nkey} {}
    ~basicNode() = default;

    // All operators below simplifies expressions used literally in all structures

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
std::ostream& SimplestRecursivePrint(const nodeT* const n, const size_t depth, std::ostream& out = std::cout) {
    if (!n) return out;

    SimplestRecursivePrint(n->left, depth + 4, out);
    out << std::string(depth, ' ') << *n << std::endl;
    SimplestRecursivePrint(n->right, depth + 4, out);

    return out;
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
void CleanTree(const nodeT* n)
    // Simplest recursive deleteng function
{
    if (!n) return;

    CleanTree(n->left);
    CleanTree(n->right);

    delete n;
}

template<class nodeT>
nodeT* CloneTree(const nodeT* n)
    // clones tree and returns its root with all branches
{
    if (!n) return nullptr;

    auto* retNode = new nodeT(*n);

    // Get both subtrees
    retNode->left = CloneTree(n->left);
    retNode->right = CloneTree(n->right);

    return retNode;
}

template<class nodeT>
class PrettyBTreePrinter {
    // ------------------------------
    // class innerTypes
    // ------------------------------

    // Used to get logical representation of tree in heap-like cordinates
    struct printInfo {
        printInfo() = default;
        ~printInfo() = default;

        size_t depth{};
        size_t rowInd{};
        const nodeT* node{};

        // Orders all elements in heap like cordinates sequence
        friend bool operator>(const printInfo& a, const printInfo& b) {
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

    using queT = std::priority_queue<printInfo, std::vector<printInfo>, std::greater<>>;

    static void SetSpacing(const std::pair<size_t, size_t>& spacings) {
        verticalSpacing = spacings.first;
        horizontalSpacing = spacings.second;
    }

    // Performs whole printing
    static std::ostream& PrintWithQueue(std::ostream& out, const nodeT* const root) {
        if (!root) {
            out << "Empty tree\n";
            return out;
        }

        const size_t largestPrint = FindlargestPrintSize(root);
        // is used in whole printing as length of single element print

        const size_t maxDepth = FindTreeDepth(root, -1);
        // used to calculate whole tree size

        const size_t myhorizontalSpacing = std::max(horizontalSpacing, largestPrint);
        // checks wheter there is need to extend standard horizontal spacing to not be smaller than single print unit

        const size_t lowestRowSize = (1 << maxDepth) * largestPrint + ((1 << maxDepth) - 1) * myhorizontalSpacing;
        // amount of characters needed in last row

        const std::string emptySpaceStr(largestPrint, ' ');
        // string used to substitute lacking node

        const std::string verSpaceStr(verticalSpacing, '\n');
        // used to display vertical spacings

        queT que{};
        // orders all nodes by its print sequence
        _gatherPrintInfo(root, que);

        // Loops throught all depths avoiding last one
        for (size_t i = 0; i < maxDepth; ++i) {
            const size_t layerElemCount = 1 << i; // 2^i
            const double layerDivider = 1 << (i + 1); // 2^(i+1) - determines how large should be left offset
            const size_t leftOffset = ((double)(lowestRowSize - layerElemCount*largestPrint)) / layerDivider;

            // setting up left offset
            out << std::string(leftOffset, ' ');

            // All below calculates how big space between following elements should be
            const size_t layerSpace = lowestRowSize - 2 * leftOffset;
            const size_t interSpace = layerElemCount == 1 ? 0 : (layerSpace - layerElemCount*largestPrint) / (layerElemCount - 1);
            const std::string interSpaceStr(interSpace, ' ');

            size_t expectedRowIndex = 0;

            // get all elements from our floor
            while(!que.empty() && que.top().depth == i) {
                auto elem = que.top();
                que.pop();

                // replace lacking elements with blank strings
                while(expectedRowIndex != elem.rowInd) {
                    out << emptySpaceStr << interSpaceStr;
                    ++expectedRowIndex;
                }

                // print actual element
                out << std::setw(largestPrint) << *elem.node << interSpaceStr;
                ++expectedRowIndex;
            }

            // display vertical spacing
            out << std::endl << verSpaceStr;
        }

        size_t expectedRowIndex = 0;
        const size_t LastRowSpaces = lowestRowSize - (1 << maxDepth) * largestPrint;
        const double singleSpace = maxDepth == 0? 0 : LastRowSpaces / ((1 << maxDepth) - 1);

        while(!que.empty()) {
            auto elem = que.top();
            que.pop();

            const size_t dist = elem.rowInd - expectedRowIndex;
            expectedRowIndex = elem.rowInd + 1;

            out << std::string(dist * singleSpace + dist * largestPrint , ' ') << std::setw(largestPrint) << *elem.node << std::string(myhorizontalSpacing, ' ');
        }

        return out << std::endl;
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

    static size_t FindTreeDepth(const nodeT* const n, const int depth) {
        if (!n) return depth;

        const size_t lDepth = FindTreeDepth(n->left, depth + 1);
        const size_t rDepth = FindTreeDepth(n->right, depth + 1);

        return std::max(lDepth, rDepth);
    }

    // ------------------------------
    // Class private methods
    // ------------------------------
private:

    // boots up recursive procedure, wchich collects information about tree
    static void _gatherPrintInfo(const nodeT* const n, queT& que) {
        if (!n) return;

        printInfo info{};
        info.node = n;
        que.push(info);

        _gatherPrintInfoRec(n->left, que, info);
        _gatherPrintInfoRec(n->right, que, info);
    }

    // recursive tree info gathering
    static void _gatherPrintInfoRec(const nodeT* const n, queT& que, const printInfo& pInfo) {
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

    inline static size_t verticalSpacing = 2;
    inline static size_t horizontalSpacing = 2;
};

#endif //BTREEHELPERS_H
