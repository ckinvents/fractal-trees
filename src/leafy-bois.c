#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

/**
 * Attempt #2 at drawing some
 * leafy bois, mostly because
 * #1 got super convoluted and
 * complicated and would
 * probably be a performance/
 * memory nightmare.
 **/

/**
 * Checklist: 
 * - efficient, easy
 *   -to-draw/animate tree 
 *   structs that represent
 *   tree fractals and look
 *   nice
 * - Vector structs for
 *   handling branch math
 * - Point structs for
 *   representing coordinates
 *   in space without angle
 *   information
 **/

const int WIDTH = 1020;
const int HEIGHT = 800;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

Tree forest[255];

void main(int argv, int* argc[])
{
    // Setup screen first
    window = SDL_CreateWindow(
        "Trees (Now Actually Pretty Good)",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    // Setup renderer second
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    // Initialize at first tree
    for (int i = 0; i < 255; i++)
    {
        Tree_initTree(&forest[i]);
    }
    int numActiveTree = 1;
    // Main update loop
    int isRunning = 1;
    // Timers for... timing
    uint32_t startFrame;
    double dt = 0;
    while (isRunning)
    {
        startFrame = SDL_GetTicks();
        // Event handler
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = 0;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_C)
            {
                for (int i = 0; i < numActiveTree; i++)
                {
                    Tree_initTree(&forest[i]);
                    Vector_setCoords(&(forest[i].branch1), 0, 0);
                    Vector_setCoords(&(forest[i].branch2), 0, 0);
                    Vector_setCoords(&(forest[i].branch3), 0, 0);
                }
                numActiveTree = 1;
            }
            else
            {
                for (int i = 0; i < numActiveTree; i++)
                {
                    //printf("Updating tree %d...\n", i);
                    Tree_triggerTree(&forest[i], &event);
                    //printf("Number of branches: %lu", sizeof(forest[i].branches) / sizeof(Branch));
                    if (i == numActiveTree - 1 && numActiveTree < 255 && (forest[i].state & stateMask) == complete)
                    {
                        numActiveTree++;
                    }
                }
            }
        }
        // Update loop
        for (int i = 0; i < numActiveTree; i++)
        {
            Tree_updateTree(&forest[i], dt);
            //puts("update");
        }
        // Draw loop
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(renderer);
        for (int i = 0; i < numActiveTree; i++)
        {
            Tree_drawTree(&forest[i], renderer);
        }
        SDL_RenderPresent(renderer);
        dt = (double)(SDL_GetTicks() - startFrame) / 1000;
    }
    // Cleanup SDL
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}