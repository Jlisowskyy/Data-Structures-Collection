//
// Created by Jlisowskyy on 12/1/23.
//

#ifndef _BASELEFTISTHEAPT_H
#define _BASELEFTISTHEAPT_H

#define DEBUG_ // TODO: TEMPRORARY

#include <iostream>

template<typename PrioT, typename ItemT, class PriorityFunction>
class _baseLeftistHeapT{
    struct node; // defined below
    using mPair = std::pair<PrioT, ItemT>;
public:
    // -------------------------------
    // type creation and copying
    // -------------------------------

    _baseLeftistHeapT() = default;

    _baseLeftistHeapT(const mPair* const pairs, const size_t size): _elemCounter{ size }{
        for (int i = 0; i < size; ++i) {
            _insert(pairs[i]);
        }
    }

    _baseLeftistHeapT(const PrioT* const prios, const ItemT* const items, const size_t size): _elemCounter{ size }{
        for (int i = 0; i < size; ++i) {
            _insert(prios[i], items[i]);
        }
    }

    _baseLeftistHeapT(const _baseLeftistHeapT& other): _elemCounter{ other._elemCounter }{
        _root = other._copyTree();
    }

    _baseLeftistHeapT(_baseLeftistHeapT&& other) noexcept: _root{ other._root }, _elemCounter{ other._elemCounter } {
        other._root = nullptr;
    }

    _baseLeftistHeapT& operator=(const _baseLeftistHeapT& other) {
        if (&other == this) return *this;

        _cleanTree(_root);
        _root = other._copyTree();
        _elemCounter = other._elemCounter;
        return *this;
    }

    _baseLeftistHeapT& operator=(_baseLeftistHeapT&& other) noexcept {
        if (&other == this) return *this;

        _cleanTree(_root);
        _root = other._root;
        _elemCounter = other._elemCounter;
        other._root = nullptr;
        return *this;
    }

    ~_baseLeftistHeapT() {
        _cleanTree(_root);
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

    _baseLeftistHeapT& Insert(const PrioT& prio, const ItemT& item)
    {
        auto n = new node{prio, item};
        _insert(n);
        ++_elemCounter;
        return *this;
    }

    // TODO
    // _baseLeftistHeapT& Insert(mPair)
    // {
    //     auto n = new node{prio, item};
    //     _insert(n);
    //     ++_elemCounter;
    //     return *this;
    // }

    _baseLeftistHeapT& Max(mPair& out) const
        // when tree is empty behaviour is undefined
    {
        out = _root->content;
        return *this;
    }

    [[nodiscard]] mPair Max() const
        // when tree is empty behaviour is undefined
    {
        return _root->content;
    }

    _baseLeftistHeapT& DeleteMax(mPair& out)
        // when tree is empty behaviour is undefined
    {
        const node* temp = _root;
        out = _root->content;
        _root = _merge(_root->left, _root->right);

        delete temp;
        return *this;
    }

    [[nodiscard]] bool Search(PrioT prio, mPair& out) const {
        node* result = _search(_root, prio);

        if (result == nullptr) return false;

        out = result->content;
        return true;
    }

    mPair DeleteMax()
        // when tree is empty behaviour is undefined
    {
        const node* temp = _root;
        mPair ret = _root->content;

        _root = _merge(_root->left, _root->right);
        delete temp;

        return ret;
    }

    [[nodiscard]] bool IsEmpty() const {
        return _elemCounter == 0;
    }

    _baseLeftistHeapT& Merge(_baseLeftistHeapT& other)
        // other object becomes unusable
    {
        _root = _merge(_root, other._root);
        _elemCounter += other._elemCounter;

        return *this;
    }

    _baseLeftistHeapT& CopyAndMerge(const _baseLeftistHeapT& other)
        // other object is untouched
    {
        _root = _merge(_root, _copyTree(other._root));
        _elemCounter += other._elemCounter;

        return *this;
    }

    static _baseLeftistHeapT Merge(_baseLeftistHeapT& a, _baseLeftistHeapT& b)
        // both a and b becomes unusable
    {
        _baseLeftistHeapT ret{};

        ret._root = _merge(a._root, b._root);
        ret._elemCounter = a._elemCounter + b._elemCounter;

        return ret;
    }

    static _baseLeftistHeapT CopyAndMerge(const _baseLeftistHeapT& a, const _baseLeftistHeapT& b)
        // both a and b are untouched after this operation
    {
        _baseLeftistHeapT ret{};

        ret._root = _merge(_copyTree(a._root), _copyTree(b._root));
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

    bool Delete(const PrioT prio)
    {
        node** res = _search(&_root, prio);
        if (res == nullptr) return false;

        _delete(res);

        return true;
    }

    bool Replace(const PrioT prio, const mPair& newItem, mPair& oItem) {
        node** res = _search(&_root, prio);
        if (res == nullptr) return false;

        oItem = (*res)->content;
        auto n = new node{newItem};
        _replace(res, n);

        return true;
    }

    bool Replace(const PrioT prio, const mPair& newItem) {
        node** res = _search(&_root, prio);
        if (res == nullptr) return false;

        auto n = new node{newItem};
        _replace(res, n);

        return true;
    }

    // TODO: UPDATE KEY
    // template<class PriorityUpdateFunction>

    std::ostream& operator<<(std::ostream& out, const _baseLeftistHeapT& tree) {
        auto printParams = tree._findPrintParams();
        out << std::setw(printParams.second);

        _printRecu(out, _root, 0);
        return out;
    }

    // -------------------------------
    // implementation components
    // -------------------------------
private:

    void _delete(node** tree) {
        node* nSubTree = _merge((*tree)->left, (*tree)->right);
        delete *tree;
        *tree = nSubTree;
    }

    void _replace(node** tree, node** n) {
        node* nSubTree = _merge((*tree)->left, n);
        nSubTree = _merge((*tree)->right, nSubTree);
        delete *tree;
        *tree = nSubTree;
    }

    static void _printRecu(std::ostream& out, const node* tree, const size_t offset) {
        static auto PrintContent = [&] {
            for(int i = 0; i < offset; ++i)
                out << SpacingStep;
            out << *tree;
        };

        if (!tree) return;

        _printRecu(out, tree->left, offset+1);
        PrintContent();
        _printRecu(out, tree->right, offset+1);
    }

    void _print(std::ostream& out) const{
        // TODO: Make nice looking printing procedure
        (void)_root;
        out << "Not implemented\n";
    }

    // Returns height of the tree as first and maximal length of printed priority as second
    static std::pair<size_t, size_t> _findPrintParams(node* nd)
    {
        if (nd == nullptr) return {0, 0};

        std::ostringstream str{};
        str << *nd;
        size_t max = str.str().length();
        const auto [lHeight, lMax] = _findMaxPrint(nd->left);
        const auto [rHeight, rMax] = _findMaxPrint(nd->right);

        if (lMax > max) max = lMax;
        if (rMax > max) max = rMax;

        return {lHeight > rHeight ? lHeight + 1 : rHeight + 1, max};
    }

    static node** _search(const node** n, PrioT prio)
        // when n is a nullptr behaviour is undefined
    {
        if (!*n) return nullptr;

        if (_pred(prio, (*n)->content.first)) return nullptr;
        if (_pred((*n)->content.first, prio)) {
            node* res = _search(&(*n)->left, prio);

            if (res == nullptr) res = _search(&(*n)->right, prio);
            return res;
        }
        return n; // found
    }

    // TODO: CAN BE A LOT FASTER
    static node* _search(const node* n, PrioT prio) {
        if (!n) return nullptr;

        if (_pred(prio, n->content.first)) return nullptr;
        if (_pred(n->content.first, prio)) {
            node* res = _search(n->left, prio);

            if (res == nullptr) res = _search(n->right, prio);
            return res;
        }
        return n; // found
    }

    void _insert(node* n) {
        _root = _merge(_root, n);
    }

    static node* _merge(node* n1, node* n2) {
        node* nRoot;
        if (!n1) return n2;
        if (!n2) return n1;

        if (*n2 > *n1)
            // bigger root has to be higher
        {
            nRoot = n2;
            n2->right = _merge(n2->right, n1);
        }
        else {
            nRoot = n1;
            n1->right = _merge(n1->right, n2);
        }

        // Flipping tree
        if (!nRoot->left || nRoot->right->npl > nRoot->left->npl) {
            std::swap(nRoot->right, nRoot->left);
        }

        // Upadating NPL parameter
        if (!nRoot->right) {
            nRoot->npl = 0;
        }
        else {
            nRoot->npl = nRoot->right->npl;
        }

        return nRoot;
    }

    static node* _copyTree(const node* tree) {
        if (!tree) return nullptr;

        node* nTree = tree->Clone();

        nTree->left = _copyTree(tree->left);
        nTree->right = _copyTree(tree->right);
        return nTree;
    }

    void _cleanTree(node* n) {
        if (n == nullptr) return;

        _cleanTree(n->left);
        delete n->left;

        _cleanTree(n->right);
        delete n->right;
    }

    // -----------------------------------
    // debugging and testing methods
    // -----------------------------------

#ifdef DEBUG_
public:
    static void TestNode(){
        node n1(PrioT{}+2, ItemT{});
        node n2(PrioT{}+1, ItemT{});

        std::cout << n1 << std::endl;
        std::cout << n2 << std::endl;
        std::cout << (n1 > n2) << std::endl;
        std::cout << (n2 > n1) << std::endl;
    }
private:
#endif

    // ------------------------------
    // internal types
    // ------------------------------

    struct node {
        node() = default;
        node(const PrioT& nPrio, const ItemT& nItem, const size_t nNpl = 0, node* nLeft = nullptr, node* nRight = nullptr):
            content{ nPrio, nItem }, left{ nLeft }, right{ nRight }, npl{ nNpl }{}
        explicit node(const mPair& pair, const size_t nNpl = 0, node* nLeft = nullptr, node* nRight = nullptr):
            content{ pair }, left{ nLeft }, right{ nRight }, npl{ nNpl }{}

        std::pair<PrioT, ItemT> content;
        node* left;
        node* right;

        size_t npl; // practiacally there could be int32

        mPair CloneContent() const {
            return content;
        }

        node* Clone() const
            // returns cloned node without connection to its children
        {
            return new node(content.first, content.second, npl);
        }

        friend bool operator>(const node& a, const node& b) {
            static PriorityFunction _pred;
            return _pred(a.content.first, b.content.first);
        }

        friend bool operator==(const node& nd, const PrioT& prio){
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
    static PriorityFunction _pred;

    inline static unsigned int PrintRecuOffsetStep = 5;
    inline static std::string SpacingStep = std::string(PrintRecuOffsetStep, ' ');
    inline static unsigned int PrintSpaceDist = 3;
    inline static std::string SpacingString = std::string(PrintSpaceDist, '\n');
};

#endif //_BASELEFTISTHEAPT_H