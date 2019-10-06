#ifndef TREE_H
#define TREE_H

#include <SDL2/SDL.h>
#include "vector.h"

// Tree struct for
// computational/drawing
// info

typedef struct _Branch {
    Vector point1;
    Vector point2;
    SDL_Color color;
} Branch;

typedef struct _Leaf {
    Vector point;
    SDL_Color color;
} Leaf;

typedef struct _Tree {
    Branch branches[255];
    Leaf leaves[255];
    Vector originCoord;
    Vector branch1;
    Vector branch2;
    Vector branch3;
    double duration;
    uint8_t state;
    uint8_t branchNum;
    uint8_t leafNum;
} Tree;

enum _TreeStateFlags {
    stateMask   = 0b0111,
    clickMask   = 0b1000,
    noBranch    = 0b0000,
    oneBranch   = 0b0001,
    twoBranch   = 0b0010,
    threeBranch = 0b0011,
    complete    = 0b0100
};

// Initializes a tree struct
void Tree_initTree(Tree* tree);
// Fills a branch struct
void Tree_fillBranch(Branch* branch, Vector* p1, Vector* p2, SDL_Color* color);
// Fills a leaf struct
void Tree_fillLeaf(Leaf* leaf, Vector* point, SDL_Color* color);

// Updates a tree
void Tree_updateTree(Tree* tree, SDL_Event* event, double dt);

// Computes branches & leaves in tree
void Tree_computeTree(Tree* tree);

// Computes actual fractal from
// initial branches and fills
// branch/point array
void Tree_computeBranches(Tree* tree, Vector originCoord, Vector branch1, double angle2, double length2, double angle3, double length3);

// Pops branch to tree
void Tree_popBranch(Tree* tree, Vector* origin, Vector* offset);

// Pops leaf to tree
void Tree_popLeaf(Tree* tree, Vector* point);

// Draws a tree
void Tree_drawTree(Tree* tree, SDL_Renderer* renderer);

#endif //TREE_H