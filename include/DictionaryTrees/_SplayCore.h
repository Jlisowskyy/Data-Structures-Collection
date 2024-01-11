//
// Created by Jlisowskyy on 1/11/24.
//

#ifndef _SPLAYCORE_H
#define _SPLAYCORE_H

#include "_AVLcore.h"

template <
    class keyT,
    class nodeT
> class _SplayCore {
    using AVLCore = _AVLCore<nodeT>;
public:

    // ------------------------------
    // Class creation
    // ------------------------------

    _SplayCore() = delete;
    _SplayCore(const _SplayCore& other) = delete;
    _SplayCore& operator=(const _SplayCore& other) = delete;

    // ------------------------------
    // Class static methods
    // ------------------------------

    static void lZig(nodeT*& n) {
        AVLCore::LL(n);
    }

    static void lZigZig(nodeT*& n) {
        AVLCore::LL(n);
        AVLCore::LL(n);
    }

    static void lZigZag(nodeT*& n) {
        AVLCore::LR(n);
    }

    static void rZig(nodeT*& n) {
        AVLCore::RR(n);
    }

    static void rZigZig(nodeT*& n) {
        AVLCore::RR(n);
        AVLCore::RR(n);
    }

    static void rZigZag(nodeT*& n) {
        AVLCore::RL(n);
    }

    static void splay(const keyT key, nodeT*& root) {

        // elements of interest should be on the left side
        if (*root > key) {

            // empty branch
            if (!root->left) return;

            if (*root->left > key)
                // deeper left side
            {
                if (root->left->left)
                    // if exists process depth two rotation
                {
                    splay(key, root->left->left);
                    lZigZig(root);
                }
                else
                    // if not process depth one rotation
                    lZig(root);
            }
            else if (key > *root->left)
                // deeper right side
            {
                if (root->left->right)
                    // if exists process depth two rotation
                {
                    splay(key, root->left->right);
                    lZigZag(root);
                }
                else
                    // if not process depth one rotation
                    lZig(root);
            }
            else
                // we found our element roll back
                lZig(root);

        }
        // elements of interest should be on the right side
        else if (key > *root) {

            // empty branch abandon work
            if (!root->right) return;

            if (*root->right > key)
                // deeper left side
            {
                if (root->right->left)
                    // if exists process depth two rotation
                {
                    splay(key, root->right->left);
                    rZigZag(root);
                }
                else
                    // if not process depth one rotation
                    rZig(root);
            }
            else if (key > *root->right)
                // deeper right side
            {
                if (root->right->right)
                    // if exists process depth two rotation
                {
                    splay(key, root->right->right);
                    rZigZig(root);
                }
                else
                    // if not process depth one rotation
                    rZig(root);
            }
            else
                // we found our element roll back
                rZig(root);
        }
    }
};
#endif //_SPLAYCORE_H
