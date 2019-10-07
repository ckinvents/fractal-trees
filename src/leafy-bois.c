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

const int WIDTH = 640;
const int HEIGHT = 480;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

Tree forest[255];

void main(int argv, int* argc[])
{
    // Setup screen first
    window = SDL_CreateWindow(
        "Trees (Slightly Broken)",
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
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = 0;
            }
            else
            {
                for (int i = 0; i < numActiveTree; i++)
                {
                    //printf("Updating tree %d...\n", i);
                    Tree_updateTree(&forest[i], &event, dt);
                    if (i == numActiveTree - 1 && numActiveTree < 255 && (forest[i].state & stateMask) == complete)
                    {
                        numActiveTree++;
                    }
                }
            }
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