//
// Created by Jlisowskyy on 2/3/24.
//

#ifndef HELPINGSTRUCTURES_H
#define HELPINGSTRUCTURES_H

#include <vector>

template<class ItemT>
struct simpleStack {
    simpleStack() = default;
    explicit simpleStack(const size_t size): _tab(size) {}
    ~simpleStack() = default;

    void push(ItemT n){
        _tab[pos++] = n;
    }

    ItemT pop() {
        return _tab[--pos];
    }

    [[nodiscard]] size_t size() const {
        return pos;
    }

    [[nodiscard]] size_t maxSize() const {
        return _tab.size();
    }

    void resize(const size_t size) {
        _tab.resize(size);
    }

    void clean() {
        pos = 0;
    }

private:
    size_t pos{};
    std::vector<ItemT> _tab{};
};
#endif //HELPINGSTRUCTURES_H
