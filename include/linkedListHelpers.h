//
// Created by Jlisowskyy on 2/1/24.
//

#ifndef LINKEDLISTHELPERS_H
#define LINKEDLISTHELPERS_H


// Note: expects nodeT to have defined copyConstructor and "nodeT* next" field.
template<class nodeT>
nodeT* cloneList(nodeT* root) {
    if (!root) return nullptr;

    nodeT* tail = new nodeT(*root);
    nodeT* head = tail;

    while((root = root->next)) {
        tail->next = new nodeT(*root);
        tail = tail->next;
    }

    return head;
}

// Note: nodeT destructor cleaning next chain is prohibited
template<class nodeT>
void cleanList(nodeT* root) {
    while(root) {
        const nodeT* toRemove = root;
        root = root->next;

        delete toRemove;
    }
}
#endif //LINKEDLISTHELPERS_H
