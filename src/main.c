#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_NODES 100
#define NODE_RADIUS 10
#define MOVE_SPEED 5
#define ZOOM_SPEED 0.1
#define SELECTED_COLOR_RED 255
#define SELECTED_COLOR_GREEN 0
#define SELECTED_COLOR_BLUE 0

// Global variables to store screen width and height
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

typedef struct {
    float x;
    float y;
} Vector2D;

typedef struct {
    Vector2D position;
    int color_r;
    int color_g;
    int color_b;
} Node;

Node nodes[NUM_NODES];
Vector2D camera = {0, 0}; // Camera position
float zoom = 1.0; // Zoom factor

void generate_random_graph() {
    srand(time(NULL));

    for (int i = 0; i < NUM_NODES; ++i) {
        nodes[i].position.x = rand() % SCREEN_WIDTH;
        nodes[i].position.y = rand() % SCREEN_HEIGHT;
        nodes[i].color_r = 0;
        nodes[i].color_g = 0;
        nodes[i].color_b = 0;
    }
}

void render(SDL_Renderer *renderer, int selected_node_index) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw black border around the edges
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, 0, SCREEN_WIDTH, 0);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, 0, SCREEN_HEIGHT);
    SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT, 0, 0);

    // Draw edges (lines between nodes)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < NUM_NODES; ++i) {
        for (int j = i + 1; j < NUM_NODES; ++j) {
            if (i == selected_node_index || j == selected_node_index) {
                SDL_SetRenderDrawColor(renderer, SELECTED_COLOR_RED, SELECTED_COLOR_GREEN, SELECTED_COLOR_BLUE, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_RenderDrawLine(renderer, (int)((nodes[i].position.x - camera.x) * zoom), (int)((nodes[i].position.y - camera.y) * zoom),
                               (int)((nodes[j].position.x - camera.x) * zoom), (int)((nodes[j].position.y - camera.y) * zoom));
        }
    }

    // Draw nodes (circles)
    for (int i = 0; i < NUM_NODES; ++i) {
        if (i == selected_node_index) {
            SDL_SetRenderDrawColor(renderer, SELECTED_COLOR_RED, SELECTED_COLOR_GREEN, SELECTED_COLOR_BLUE, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, nodes[i].color_r, nodes[i].color_g, nodes[i].color_b, 255);
        }
        SDL_RenderDrawPoint(renderer, (int)((nodes[i].position.x - camera.x) * zoom), (int)((nodes[i].position.y - camera.y) * zoom));
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        printf("SDL_GetDesktopDisplayMode Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SCREEN_WIDTH = dm.w;
    SCREEN_HEIGHT = dm.h;

    SDL_Window *window = SDL_CreateWindow("Random Graph Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_HIDDEN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_ShowWindow(window);

    generate_random_graph();

    int selected_node_index = 0;
    SDL_Event event;
    int quit = 0;
    Uint32 last_tick_time = SDL_GetTicks();
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_tick_time >= 1000) {
            last_tick_time = current_time;
            // Select a different node every second
            selected_node_index = (selected_node_index + 1) % NUM_NODES;
            // Change color of selected node
            nodes[selected_node_index].color_r = SELECTED_COLOR_RED;
            nodes[selected_node_index].color_g = SELECTED_COLOR_GREEN;
            nodes[selected_node_index].color_b = SELECTED_COLOR_BLUE;
        }

        render(renderer, selected_node_index);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
