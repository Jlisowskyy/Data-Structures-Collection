//
// Created by Jlisowskyy on 12/2/23.
//

#ifndef _BASEBINOMIALQUEUET_H
#define _BASEBINOMIALQUEUET_H

#define DEBUG_ // TODO: TEMPRORARY

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/*          TODO:
 *  - Efficient tree building
 *
 */

template<typename PrioT, typename ItemT, class PriorityFunction>
class _baseBinominialQueue{
    struct node; // defined below
    using mPair = std::pair<PrioT, ItemT>;
public:
    // -------------------------------
    // type creation and copying
    // -------------------------------

    _baseBinominialQueue() = default;

    _baseBinominialQueue(const mPair* const pairs, const size_t size): _elemCounter{ size }{
        for (int i = 0; i < size; ++i) {
            auto n = new node{pairs[i]};
            _insert(n);
        }
    }

    _baseBinominialQueue(const PrioT* const prios, const ItemT* const items, const size_t size): _elemCounter{ size }{
        for (int i = 0; i < size; ++i) {
            auto n = new node{prios[i], items[i]};
            _insert(n);
        }
    }

    _baseBinominialQueue(const _baseBinominialQueue& other): _elemCounter{ other._elemCounter }{
        _root = other._copyList();
    }

    _baseBinominialQueue(_baseBinominialQueue&& other) noexcept: _root{ other._root }, _elemCounter{ other._elemCounter } {
        other._root = nullptr;
    }

    _baseBinominialQueue& operator=(const _baseBinominialQueue& other) {
        if (&other == this) return *this;

        _cleanList(_root);
        _root = other._copyList();
        _elemCounter = other._elemCounter;
        return *this;
    }

    _baseBinominialQueue& operator=(_baseBinominialQueue&& other) noexcept {
        if (&other == this) return *this;

        _cleanList(_root);
        _root = other._root;
        _elemCounter = other._elemCounter;
        other._root = nullptr;
        return *this;
    }

    ~_baseBinominialQueue() {
        _cleanList(_root);
    }

    // ------------------------------
    // Type interaction
    // ------------------------------

    static void ChangePrintRecuOffsetStep(const unsigned int newStep) {
        PrintRecuOffsetStep = newStep;
        SpacingStep = std::string(newStep, '\n');
    }

    static void ChangePrintLevelSpacing(const unsigned int newSize) {
        PrintSpaceDist = newSize;
        SpacingString = std::string(newSize, '\n');
    }

    _baseBinominialQueue& Insert(const PrioT& prio, const ItemT& item)
    {
        auto n = new node{prio, item};
        _insert(n);
        ++_elemCounter;
        return *this;
    }

    _baseBinominialQueue& Insert(const mPair& pair)
    {
        auto n = new node{pair};
        _insert(n);
        ++_elemCounter;
        return *this;
    }

    _baseBinominialQueue& Max(mPair& out) const
        // when tree is empty behaviour is undefined
    {
        out = _getMax();
        return *this;
    }

    [[nodiscard]] mPair Max() const
        // when tree is empty behaviour is undefined
    {
        return _getMax();
    }

    _baseBinominialQueue& DeleteMax(mPair& out)
        // when tree is empty behaviour is undefined
    {
        node* temp = _getMaxNode();
        out = temp->content;
        _deleteMax(temp);
        --_elemCounter;
        return *this;
    }

    mPair DeleteMax()
    // when tree is empty behaviour is undefined
    {
        node* temp = _getMaxNode();
        mPair ret = temp->content;
        _deleteMax(temp);
        --_elemCounter;
        return ret;
    }

    [[nodiscard]] bool Search(PrioT prio, mPair& out) const {
        node* result = _searchTreeList(_root, prio);

        if (result == nullptr) return false;

        out = result->content;
        return true;
    }

    [[nodiscard]] bool IsEmpty() const {
        return _elemCounter == 0;
    }

    _baseBinominialQueue& Merge(_baseBinominialQueue& other)
        // other object becomes unusable
    {
        _root = _mergeList(_root, other._root);
        _elemCounter += other._elemCounter;

        other._root = nullptr;
        other._elemCounter = 0;

        return *this;
    }

    _baseBinominialQueue& CopyAndMerge(const _baseBinominialQueue& other)
        // other object is untouched
    {
        _root = _mergeList(_root, _copyList(other._root));
        _elemCounter += other._elemCounter;

        return *this;
    }

    static _baseBinominialQueue Merge(_baseBinominialQueue& a, _baseBinominialQueue& b)
        // both a and b becomes unusable
    {
        _baseBinominialQueue ret{};

        ret._root = _merge(a._root, b._root);
        ret._elemCounter = a._elemCounter + b._elemCounter;

        a._root = nullptr;
        a._elemCounter = 0;

        b._root = nullptr;
        b._elemCounter = 0;

        return ret;
    }

    static _baseBinominialQueue CopyAndMerge(const _baseBinominialQueue& a, const _baseBinominialQueue& b)
        // both a and b are untouched after this operation
    {
        _baseBinominialQueue ret{};

        ret._root = _mergeList(_copyList(a._root), _copyList(b._root));
        ret._elemCounter = a._elemCounter + b._elemCounter;

        return ret;
    }

    bool Delete(const PrioT prio, mPair& out) {
        node** res = _search(&_root, prio);
        if (res == nullptr) return false;

        out = (*res)->content;
        _delete(res);

        return true;
    }

    // TODO: implement
    // bool Delete(const PrioT prio)
    // {
    //     node** res = _search(&_root, prio);
    //     if (res == nullptr) return false;
    //
    //     _delete(res);
    //
    //     return true;
    // }
    //
    // bool Replace(const PrioT prio, const mPair& newItem, mPair& oItem) {
    //     node** res = _search(&_root, prio);
    //     if (res == nullptr) return false;
    //
    //     oItem = (*res)->content;
    //     auto n = new node{newItem};
    //     _replace(res, n);
    //
    //     return true;
    // }
    //
    // bool Replace(const PrioT prio, const mPair& newItem) {
    //     node** res = _search(&_root, prio);
    //     if (res == nullptr) return false;
    //
    //     auto n = new node{newItem};
    //     _replace(res, n);
    //
    //     return true;
    // }

    // TODO: UPDATE KEY
    // template<class PriorityUpdateFunction>

    // friend std::ostream& operator<<(std::ostream& out, const _baseLeftistHeapT& tree) {
    //     auto printParams = _findPrintParams(tree._root);
    //
    //     _printRecu(out, tree._root, 0, printParams.second);
    //     return out;
    // }

    // -------------------------------
    // implementation components
    // -------------------------------

    mPair& _getMax() const
        // When _root is nullptr behaciour is undefined.
    {
        return _getMaxNode()->content;
    }

    static size_t _getMaxStringSize(node* tree)
    {
        if (tree == nullptr) return 0;

        size_t max = 0;
        do {
            std::ostringstream str{};
            str << *tree;

            if (const size_t nMax = str.str().length(); nMax > max) max = nMax;
            if (const size_t chldMax = _getMaxStringSize(tree->child); chldMax > max) max = chldMax;
        }while((tree = tree->next));

        return max;
    }

    struct _printInfo {
        std::string emptyFieldString;
        std::string horizontalConnectionString;
        std::string verticalConnectionString;
        size_t maxStringSize;
        size_t maxDepth;
    };

    using sVect = std::vector<std::string>;
    using pVect = std::vector<sVect>;
    static void _print(std::ostream& out, node* list)
    {
        if (list == nullptr) {
            out << "[ Empty tree ]";
            return;
        }

        _printInfo info {
            .maxDepth = 2*(list->prev->height + 1),
            .maxStringSize = _getMaxStringSize(list)
        };

        // to make printing prettier ~~ perfectly aligned
        if (info.maxStringSize % 2 != 0) ++info.maxStringSize;

        info.emptyFieldString = std::string(info.maxStringSize, ' ');
        info.horizontalConnectionString = std::string(info.maxStringSize, '-');
        info.verticalConnectionString = std::string(info.maxStringSize, ' ');
        info.verticalConnectionString[info.maxStringSize / 2] = '|';

        const pVect pages = _genTreePrintRecu(list, info);

        for(size_t j = 0; j < info.maxDepth; ++j) {
            for (size_t i = 0; i < pages.size(); ++i) {
                out << pages[i][j];
            }
        }
    }

    static pVect _genTreePrintVect(node* tree, const _printInfo& info) {

        return {};
    }

    static pVect _genTreePrintRecu(node* tree, const _printInfo& info) {
        if (tree == nullptr) {
            pVect v;
            v.push_back(sVect{});

            for(size_t i = 0; i < info.maxDepth; ++i) v[0].push_back(info.emptyFieldString);
            return v;
        };

        _printInfo mInfo = info;
        mInfo.maxDepth -= 2;

        // Generating separator;
        sVect separator{};
        separator.push_back(info.horizontalConnectionString);
        for(size_t i = 1; i < info.maxDepth; ++i) {
            separator.push_back(info.emptyFieldString);
        }

        pVect subMatrix{};
        do {
            pVect subResult = _genTreePrintRecu(tree->child,  mInfo);

            // preparing first line
            sVect firstLine{};

            firstLine.push_back(tree->ToString(info.maxStringSize, ' '));

            if (!tree->child)
                firstLine.push_back(info.emptyFieldString);
            else
                firstLine.push_back(info.verticalConnectionString);
            firstLine.insert(firstLine.end(), subResult.begin(), subResult.end());
            subMatrix.push_back(firstLine);

            // adding other lines
            for(size_t i = 1; i < subResult.size(); ++i) {
                sVect line{};

                // correcting last element on list
                if (tree->next) {
                    line.push_back(info.horizontalConnectionString);
                    line.push_back(info.emptyFieldString);
                }
                else {
                    line.push_back(info.emptyFieldString);
                    line.push_back(info.emptyFieldString);
                }

                line.insert(line.end(), subResult[i].begin(), subResult[i].end());
                subMatrix.push_back(line);
            }

            if (tree->next) subMatrix.push_back(separator);
        }while((tree = tree->next));

        return subMatrix;
    }

    node* _getMaxNode() const
        // When _root is nullptr behaviour is undefined.
        // Returns pointer to Maximal node on the queue,
        // which should not be deleted until _deleteMax is not called on this node.
    {
        node* Max = _root;
        node* p = _root;

        while((p = p->next)) {
            if (*p > *Max) p = Max;
        }

        return Max;
    }

    void _deleteMax(node* maxNode) {
        _removeMaxFromList(maxNode);
        delete maxNode;
    }

    void _removeMaxFromList(node* maxNode)
        // Max node have to point to some node on main queue, otherwise behaciour is undefined.
        // Just disconnects maxNode from structures and performs all necessary repairs.
    {
        node* t = maxNode->child;

        if (maxNode->next)
            // node is not last one
        {
            maxNode->next->prev = maxNode->prev;
        }
        else
            // node is tail
        {
            _root->prev = maxNode->prev;
        }

        if (maxNode == _root) {
            _root = _root->next;
        }
        else {
            maxNode->prev->next = maxNode->next;
        }

        _root = _mergeList(_root, t);
    }

    static node* _searchTreeList(node* list, PrioT prio) {
        if (list == nullptr) return nullptr;

        do {
            if (*list > prio)
                if (node* res = _searchTreeList(list->child, prio); res != nullptr) {
                    return res;
                }

            if (!(prio > *list)) return list;

        }while ((list = list->next));

        return nullptr;
    }

    static node* _mergeTree(node* t1, node* t2)
        // t1->h == t2->h
    {
        if (*t2 > *t1) std::swap(t1, t2);

        if (t1->height == 0) {
            t1->child = t2;
        }
        else {
            t2->prev = t1->child->prev;
            t2->prev->next = t2;
            t1->child->prev = t2;
        }

        ++t1->height;
        return t1;
    }

    static node* _extract(node** head) {
        if (!*head) return nullptr;
        node* t = *head;

        if (t->next) {
            t->next->prev = t->prev;
            *head = t->next;
        }

        t->prev = t;
        t->next = nullptr;

        return t;
    }

    static node* _mergeListRecu(node* l1, node* l2) {
        static auto _mergeFirstHigher = [](node* lHigher, node* lLower) {
            node* n1 = _extract(&lLower);
            node* l3 = _mergeListRecu(lHigher, lLower);

            // repairing connections
            n1->next = l3;
            n1->prev = l3->prev;
            l3.prev = n1;

            return l3;
        };

        // ------------------------------
        // Function code
        // ------------------------------

        if (!l1) return l2;
        if (!l2) return l1;


        if (l1->height > l2->height)
            // if head of l2 has smaller height we need to extract it first to preserve growing heights in queue
        {
            return _mergeFirstHigher(l1, l2);
        }

        if (l2->height > l1->height)
            // if head of l1 has smaller height we need to extract it first to preserve growing heights in queue
        {
            return _mergeFirstHigher(l2, l1);
        }

        // neither of them has bigger height so they are equal - no need to check
        node* n1 = _extract(&l1);
        node* n2 = _extract(&l2);
        node* nTree = _mergeTree(n1, n2);
        node* l3 = _mergeListRecu(l1, l2);
        l3 = _mergeListRecu(l3, nTree);

        return l3;
    }

    static node* _mergeListNonRecu(node* l1, node* l2) {
        // TODO: IMPLEMENT

        return nullptr;
    }

    static node* _mergeList(node* l1, node* l2) {
        return _mergeListRecu(l1, l2);
    }

    static node* _copyList(node* list) {
        if (list == nullptr) return nullptr;

        node* tail = list->Clone();
        tail->child = _copyList(list->child);
        node* head = tail;

        while((list = list->next)) {
            tail->next = list->Clone();
            tail->next->prev = tail;
            tail = tail->next;
            tail->child = _copyList(list->child);
        }

        head->prev = tail;
        tail->next = nullptr;

        return head;
    }

    static void _cleanList(node* list)
    {
        if (list == nullptr) return;
        node* readyToRemove = nullptr;

        do {
            delete readyToRemove;
            _cleanList(list->child);
            readyToRemove = list;
        }while ((list = list->next));
    }

    // -----------------------------------
    // debugging and testing methods
    // -----------------------------------

    // ------------------------------
    // internal types
    // ------------------------------

    struct node {
        node(const PrioT& nPrio, const ItemT& nItem):
            content{ nPrio, nItem }, prev { this }, next{ nullptr }, child{ nullptr }, height{ 0 } {}
        explicit node(const mPair& pair):
            node{ pair.first, pair.second } {}
        node(): content{}, prev { this }, next{ nullptr }, child{ nullptr }, height{ 0 } {}

        std::pair<PrioT, ItemT> content;
        node* prev;
        node* next;
        node* child;
        size_t height;

        mPair CloneContent() const {
            return content;
        }

        node* Clone() const
            // returns cloned node without connection to its children
        {
            return new node(content.first, content.second);
        }

        std::string ToString(size_t minSize, char fill) {
            std::ostringstream str{};
            str << std::setw(minSize) << std::setfill(fill) << content.first;
            return str.str();
        }

        friend bool operator>(const node& a, const node& b) {
            static PriorityFunction _pred;
            return _pred(a.content.first, b.content.first);
        }

        friend bool operator>(const node& a, const PrioT& prio) {
            static PriorityFunction _pred;
            return _pred(a.content.first, prio);
        }

        friend bool operator>(const PrioT& prio, const node& b) {
            static PriorityFunction _pred;
            return _pred(prio, b.content.first);
        }

        friend bool operator==(const node& nd, const PrioT& prio){
            static PriorityFunction _pred;
            return !_pred(nd.content.first, prio) && !_pred(prio, nd.content.first);
        }

        friend std::ostream& operator<<(std::ostream& out, const node& a) {
            out << a.content.first;
            return out;
        }
    };

    // ------------------------------
    // private fields
    // ------------------------------

    node* _root = nullptr;
    size_t _elemCounter = 0;
    PriorityFunction _pred{};

    inline static unsigned int PrintRecuOffsetStep = 2;
    inline static std::string SpacingStep = std::string(PrintRecuOffsetStep, ' ');
    inline static unsigned int PrintSpaceDist = 0;
    inline static std::string SpacingString = std::string(PrintSpaceDist, '\n');
};

#endif //_BASEBINOMIALQUEUET_H