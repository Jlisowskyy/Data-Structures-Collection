//
// Created by Jlisowskyy on 1/13/24.
//

#ifndef INDEXEDSORTING_H
#define INDEXEDSORTING_H

template<class itemT, class indexedT>
class IndexedSorter {
    // ------------------------------
    // Class creation
    // ------------------------------
public:

    IndexedSorter() = delete;
    IndexedSorter(indexedT& indexedType, const int size): A{indexedType}, size{size} {}
    ~IndexedSorter() = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    void QuickSort() {
        _quickSort(0, size-1);
    }

    std::ostream& Print(std::ostream& out = std::cout) const {
        for(auto& item : A) {
            out << item << " -> ";
        }
        return out << "NULL";
    }

    // ------------------------------
    // Private methods
    // ------------------------------

    void _quickSort(const int l, const int r) {
        const int m = _partition2(l, r);
        if (m-1 > l) _quickSort(l, m-1);
        if (m+1 < r) _quickSort(m+1, r);
    }


    [[nodiscard]] int _partition1(const int l , const int r) {
        int i = l;
        itemT v = A[r];

        for (int j = l; j < r; ++j)
            if (A[j] <= v)
                std::swap(A[j], A[i++]);
        std::swap(A[i], A[r]);

        return i;
    }

    [[nodiscard]] int _partition2(const int l, const int r) {
        itemT v = A[l];
        int i = l;
        int j = r+1;

        while(i < j) {
            while(A[++i] <= v){}
            while(A[--j] > v) {}

            if (i < j)
                std::swap(A[i], A[j]);
        }

        A[l] = A[j];
        A[j] = v;

        return j;
    }



    // ------------------------------
    // class fields
    // ------------------------------

    indexedT& A;
    int size;
};

#endif //INDEXEDSORTING_H
