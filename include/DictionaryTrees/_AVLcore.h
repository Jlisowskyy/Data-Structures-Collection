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
        noTilt = 0,
        leftTilt = 1,
        rightTilt = -1,
    };

    static void RR(nodeT*& root) {
        // saving right subtree
        nodeT* rightSubtree = root->right;

        // balancing subtrees
        root->right = rightSubtree->left;
        rightSubtree->left = root;

        // if necessary changes bl
        if constexpr (refactorBL) {
            if (rightSubtree->bl == rightTilt) {
                rightSubtree->bl = noTilt;
                root->bl = noTilt;
            }
            else {
                rightSubtree->bl = leftTilt;
                root->bl = rightTilt;
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
            if (leftSubtree->bl == leftTilt) {
                root->bl = noTilt;
                leftSubtree->bl = noTilt;
            }
            else {
                leftSubtree->bl = rightTilt;
                root->bl = leftTilt;
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
            if (leftRightSubtree->bl == noTilt) {
                root->bl = leftSubtree->bl = noTilt;
            }
            else if (leftRightSubtree->bl == leftTilt) {
                leftSubtree->bl = noTilt;
                root->bl = rightTilt;
            }
            else {
                leftSubtree->bl = leftTilt;
                root->bl = noTilt;
            }

            leftRightSubtree->bl = noTilt;
            // root->bl = (leftRightSubtree->bl==1) ? -1 : 0;
            // leftSubtree->bl = (leftRightSubtree->bl==-1) ? 1 : 0;
            // leftRightSubtree->bl = 0;
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
            if (rightLeftSubtree->bl == noTilt) {
                root->bl = rightSubtree->bl = noTilt;
            }
            else if (rightLeftSubtree->bl == rightTilt) {
                root->bl = leftTilt;
                rightSubtree->bl = noTilt;
            }
            else{
                root->bl = noTilt;
                rightSubtree->bl = rightTilt;
            }

            rightLeftSubtree->bl = noTilt;

            // root->bl = (rightLeftSubtree->bl==-1) ? 1 : 0;
            // rightSubtree->bl = (rightLeftSubtree->bl==1) ? -1 : 0;
            // rightLeftSubtree->bl = 0;
        }

        //saving results
        root = rightLeftSubtree;
    }
};

#endif //_AVLCORE_H
