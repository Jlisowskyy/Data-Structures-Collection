//
// Created by Jlisowskyy on 1/11/24.
//

#ifndef _AVLCORE_H
#define _AVLCORE_H

template<
    class nodeT,
    bool refactorBL = false
> struct _AVLCore {
    _AVLCore() = delete;
    _AVLCore(const _AVLCore& other) = delete;
    _AVLCore& operator=(const _AVLCore& other) = delete;

    enum blStates {
        balanced = 0,
        leftSlightTilt = 1,
        rightSlightTilt = -1,
        leftTilt = 2,
        rightTilt = -2,
    };

    static void RR(nodeT*& root) {
        // saving right subtree
        nodeT* rightSubtree = root->right;

        // balancing subtrees
        root->right = rightSubtree->left;
        rightSubtree->left = root;

        // if necessary changes bl
        if constexpr (refactorBL) {
            if (rightSubtree->bl == rightSlightTilt) {
                rightSubtree->bl = balanced;
                root->bl = balanced;
            }
            else {
                rightSubtree->bl = leftSlightTilt;
                root->bl = rightSlightTilt;
            }
        }

        //saving result
        root = rightSubtree;
    }

    static void LL(nodeT*& root) {
        // saving left subtree
        nodeT* leftSubtree = root->left;

        // balacing subtrees
        root->left = leftSubtree->right;
        leftSubtree->right = root;

        if constexpr (refactorBL) {
            if (leftSubtree->bl == leftSlightTilt) {
                root->bl = balanced;
                leftSubtree->bl = balanced;
            }
            else {
                leftSubtree->bl = rightSlightTilt;
                root->bl = leftSlightTilt;
            }
        }

        //saving result
        root = leftSubtree;
    }

    static void LR(nodeT*& root) {
        //saving subtrees
        nodeT* leftSubtree = root->left;
        nodeT* leftRightSubtree = leftSubtree->right;

        // balancing tree
        leftSubtree->right = leftRightSubtree->left;
        leftRightSubtree->left = leftSubtree;

        root->left = leftRightSubtree->right;
        leftRightSubtree->right = root;

        if constexpr (refactorBL) {
            if (leftRightSubtree->bl == balanced) {
                root->bl = leftSubtree->bl = balanced;
            }
            else if (leftRightSubtree->bl == leftSlightTilt) {
                leftSubtree->bl = balanced;
                root->bl = rightSlightTilt;
            }
            else {
                leftSubtree->bl = leftSlightTilt;
                root->bl = balanced;
            }

            leftRightSubtree->bl = balanced;
        }

        // saving result
        root = leftRightSubtree;
    }

    static void RL(nodeT*& root) {
        //saving subtress
        nodeT* rightSubtree = root->right;
        nodeT* rightLeftSubtree = rightSubtree->left;

        //balacning trees
        rightSubtree->left = rightLeftSubtree->right;
        rightLeftSubtree->right = rightSubtree;

        root->right = rightLeftSubtree->left;
        rightLeftSubtree->left = root;

        if constexpr (refactorBL) {
            if (rightLeftSubtree->bl == balanced) {
                root->bl = rightSubtree->bl = balanced;
            }
            else if (rightLeftSubtree->bl == rightSlightTilt) {
                root->bl = leftSlightTilt;
                rightSubtree->bl = balanced;
            }
            else{
                root->bl = balanced;
                rightSubtree->bl = rightSlightTilt;
            }

            rightLeftSubtree->bl = balanced;
        }

        //saving results
        root = rightLeftSubtree;
    }
};

#endif //_AVLCORE_H
