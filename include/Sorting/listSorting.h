//
// Created by Jlisowskyy on 1/13/24.
//

#ifndef LISTSORTING_H
#define LISTSORTING_H

#include <queue>

namespace ListSortingAlgorithms {

template<class itemT>
struct listNode {
    listNode() = default;
    explicit listNode(const itemT& item): _item{item} {}

    constexpr listNode*& getNext() { return _next; }
    constexpr itemT& getItem() { return _item; }

    static void printList(listNode* node, std::ostream& out = std::cout) {
        while (node) {
            out << node->getItem() << " -> ";
            node = node->getNext();
        }
        out << "NULL\n";
    }

    static void pushFront(listNode*& node, const itemT& item) {
        auto* nNode = new listNode(item);
        nNode->getNext() = node;
        node = nNode;
    }

    static void pushFront(listNode*& node, listNode* nNode) {
        nNode->getNext() = node;
        node = nNode;
    }

    static listNode* extractFront(listNode*& node) {
        auto* retNode = node;
        node = node->getNext();
        return retNode;
    }

    static void cleanList(listNode*& node) {
        while(node) {
            listNode* n = extractFront(node);
            delete n;
        }

        node = nullptr;
    }

private:
    itemT _item{};
    listNode* _next{};
};

template<class nodeT, class predT = std::greater<>>
struct nodeComparer {
    nodeComparer() = default;
    explicit nodeComparer(nodeT* n): node{n} {}

    friend bool operator<(const nodeComparer& a, const nodeComparer& b) {
        return pred(a.node->getItem(), b.node->getItem());
    }

    nodeT* node{};
    static constexpr predT pred{};
};

template<class nodeT, class predT = std::greater<>>
void HeapSort(nodeT*& root) {
    static constexpr predT pred{};
    std::priority_queue<nodeComparer<nodeT, predT>> que{};

    while(root) {
        auto next = nodeT::extractFront(root);
        que.emplace(next);
    }

    while(!que.empty()) {
        nodeT::pushFront(root, que.top().node);
        que.pop();
    }
}

template<class nodeT, class predT = std::greater<>>
void QuickSort(nodeT*& root) {
    static const predT pred{};
    static const auto Push = [](nodeT*& head, nodeT*& tail, nodeT* node) {
        if (!head) {
            head = node;
            tail = node;
            return;
        }

        node->getNext() = head;
        head = node;
    };

    if (!root) return;

    nodeT *L{}, *M{}, *R{};
    nodeT *LTail{}, *MTail{}, *RTail{};
    auto v = root->getItem();

    while(root) {
        nodeT* n = root;
        root = root->getNext();
        n->getNext() = nullptr;

        if (pred(n->getItem(), v))
            Push(L, LTail, n);
        else if (pred(v, n->getItem()))
            Push(R, RTail, n);
        else
            Push(M, MTail, n);
    }

    QuickSort(L);
    QuickSort(R);

    if (L) {
        root = L;
        LTail->getNext() = M;
    }
    else
        root = M;

    MTail->getNext() = R;
}







};
#endif //LISTSORTING_H
