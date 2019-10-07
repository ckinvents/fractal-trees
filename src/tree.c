#include "tree.h"
#include <SDL2/SDL.h>

/**
 * Tree
 * 
 * Handles the computation,
 * drawing, and updating of
 * Tree structs, as well as
 * their substructs
 * 
 * Connor Ennis, 2019
 **/

#define MIN_BRANCH_LENGTH 2
#define BRANCH_SHRINK_FACTOR 1.5

// Initiallizes a tree
void Tree_initTree(Tree* tree)
{
    tree->duration = 0;
    tree->state = noBranch;
    tree->branchNum = 0;
    tree->leafNum = 0;
}

// Fills a branch struct
void Tree_fillBranch(Branch* branch, Vector* p1, Vector* p2, SDL_Color* color)
{
    branch->point1 = *p1;
    branch->point2 = *p2;
    branch->color = *color;
}

//Fills a leaf struct
void Tree_fillLeaf(Leaf* leaf, Vector* point, SDL_Color* color)
{
    leaf->point = *point;
    leaf->color = *color;
}

// Updates tree in main loop
void Tree_updateTree(Tree* tree, SDL_Event* event, double dt)
{
    // See if tree incomplete...
    //printf("Tree mask state: %x\n", tree->state & stateMask);
    if ((tree->state & stateMask) != complete)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        // If click occurs for current tree
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            //puts("SDL_MOUSEDOWN event");
            // Set to dragging state
            tree->state |= clickMask;
            tree->duration = 0;
        }
        // While dragging...
        if ((tree->state & clickMask) == clickMask)
        {
            // Based on branch state...
            switch (tree->state & stateMask)
            {
                case noBranch:
                {
                    Vector_setCoords(&tree->originCoord, mouseX, mouseY);
                    break;
                }
                case oneBranch:
                {
                    Vector_setCoords(&tree->branch1, mouseX - tree->originCoord.x, mouseY - tree->originCoord.y);
                    break;
                }
                case twoBranch:
                {
                    // Set origin to center to make
                    // math more straightforward
                    Vector relOrigin;
                    Vector_addVector(&tree->originCoord, &tree->branch1, &relOrigin);
                    Vector_setCoords(&tree->branch2, mouseX - relOrigin.x, mouseY - relOrigin.y);
                    break;
                }
                case threeBranch:
                {
                    Vector relOrigin;
                    Vector_addVector(&tree->originCoord, &tree->branch1, &relOrigin);
                    Vector_setCoords(&tree->branch3, mouseX - relOrigin.x, mouseY - relOrigin.y);
                    break;
                }
                default:
                    break;
            }
            // See if button released...
            if (event->type == SDL_MOUSEBUTTONUP)
            {
                if ((tree->state & stateMask) == threeBranch)
                {
                    Tree_computeTree(tree);
                }
                tree->state++;
                tree->state &= ~clickMask;
                tree->duration = 0;
            }
            else
            {
                tree->duration += dt;
            }
        }
    }
    // If tree already completed...
    else
    {
        // Placeholder, just increments
        // existance timer for now
        tree->duration += dt;
    }
    
}

// Computes branches and leaves of
// tree fractal
void Tree_computeTree(Tree* tree)
{
    // Call recursive branching function with initial info
    Vector originCoord = tree->originCoord;
    Vector branch1 = tree->branch1;
    double angle2 = Vector_getAngleDif(&tree->branch2, &branch1);
    double length2 = Vector_getLength(&tree->branch2);
    double angle3 = Vector_getAngleDif(&tree->branch3, &branch1);
    double length3 = Vector_getLength(&tree->branch3);
    Tree_computeBranches(tree, originCoord, branch1, angle2, length2, angle3, length3);
}

// Recursive branch computing 
// process
void Tree_computeBranches(Tree* tree, Vector originCoord, Vector branch1, double angle2, double length2, double angle3, double length3)
{
    // Add branch for origin
    Vector newOrigin;
    Vector_addVector(&originCoord, &branch1, &newOrigin);
    SDL_Color newColor = {0xff, 0x00, 0x00, 0xff};
    Vector colorVector;
    Tree_popBranch(tree, &originCoord, &branch1, &newColor);
    // See if branch length less
    // than minimum, create leaf
    // if case
    if (Vector_getLength(&branch1) < MIN_BRANCH_LENGTH)
    {
        Tree_popLeaf(tree, &newOrigin);
    }
    else
    {
        // Create vectors for 2 & 3
        Vector branch2;
        Vector branch3;
        double baseAngle = Vector_getAngle(&branch1);
        Vector_setPolar(&branch2, baseAngle + angle2, length2);
        Vector_setPolar(&branch3, baseAngle + angle3, length3);
        // Create new branches for 2 and 3
        Tree_computeBranches(tree, newOrigin, branch2, angle2, length2 / BRANCH_SHRINK_FACTOR, angle3, length3 / BRANCH_SHRINK_FACTOR);
        Tree_computeBranches(tree, newOrigin, branch3, angle2, length2 / BRANCH_SHRINK_FACTOR, angle3, length3 / BRANCH_SHRINK_FACTOR);
    } 
}

// Adds branch to tree draw array
void Tree_popBranch(Tree* tree, Vector* origin, Vector* offset, SDL_Color* color)
{
    Vector p2;
    if (tree->branchNum < MAX_BRANCHES)
    {
        Vector_addVector(origin, offset, &p2);
        Tree_fillBranch(&tree->branches[tree->branchNum], origin, &p2, color);
        tree->branchNum++;
    }
    else
    {
        printf("FATAL: Max branch count exceded! Halting...\n");
        exit(EXIT_FAILURE);
    }
}

// Adds leaf to tree draw array
void Tree_popLeaf(Tree* tree, Vector* point)
{
    // Placeholder, will add real color management
    SDL_Color leafColor = {0x00, 0xff, 0x00, 0xff};
    Tree_fillLeaf(&tree->leaves[tree->leafNum], point, &leafColor);
    tree->leafNum++;
}

// Draws tree in given state
void Tree_drawTree(Tree* tree, SDL_Renderer* renderer)
{
    // If tree incomplete
    if ((tree->state & stateMask) != complete)
    {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        // If at least one branch
        if ((tree->state & stateMask) >= 1)
        {
            Vector newOrigin;
            Vector_addVector(&tree->originCoord, &tree->branch1, &newOrigin);
            SDL_RenderDrawLine(renderer, tree->originCoord.x, tree->originCoord.y, newOrigin.x, newOrigin.y);
            // If at least two branches
            if ((tree->state & stateMask) >= 2)
            {
                Vector point2;
                Vector_addVector(&newOrigin, &tree->branch2, &point2);
                SDL_RenderDrawLine(renderer, newOrigin.x, newOrigin.y, point2.x, point2.y);
                // If three branches
                if ((tree->state & stateMask) == 3)
                {
                    Vector point3;
                    Vector_addVector(&newOrigin, &tree->branch3, &point3);
                    SDL_RenderDrawLine(renderer, newOrigin.x, newOrigin.y, point3.x, point3.y);
                }
            }
        }
    }
    else // For every branch...
    {
        for (int i = 0; i < tree->branchNum; i++)
        {
            SDL_Color* branchColor = &tree->branches[i].color;
            Branch* currentBranch = &tree->branches[i];
            SDL_SetRenderDrawColor(renderer, branchColor->r, branchColor->g, branchColor->b, branchColor->a);
            SDL_RenderDrawLine(renderer, currentBranch->point1.x, currentBranch->point1.y, currentBranch->point2.x, currentBranch->point2.y);
        }
    }
}