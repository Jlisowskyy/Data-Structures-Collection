//
// Created by Jlisowskyy on 12/2/23.
//

#ifndef _BASEBINOMIALQUEUET_H
#define _BASEBINOMIALQUEUET_H

#define DEBUG_ // TODO: TEMPRORARY

#include <iostream>

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

        return nullptr;
    }

    static node* _mergeList(node* l1, node* l2) {
        return _mergeListRecu(l1, l2);
    }

    static node* _copyList(node* list) {

        return nullptr;
    }

    static void _cleanList(node* list) {

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

        friend bool operator>(const node& a, const node& b) {
            static PriorityFunction _pred;
            return _pred(a.content.first, b.content.first);
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