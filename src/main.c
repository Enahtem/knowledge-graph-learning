#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Screen dimensions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define SELECTED_COLOR_RED 255
#define SELECTED_COLOR_GREEN 0
#define SELECTED_COLOR_BLUE 0

#define ENTITY_RADIUS 40

typedef struct Entity {
    char* label;
    char* description;
    int x;
    int y;
    struct Relation** relations;
    int num_relations;
} Entity;

typedef struct Relation {
    char* label;
    char* desription;
    struct Entity* entity;
} Relation;

typedef struct KnowledgeGraph {
    Entity** entities;
    int num_entities;
} KnowledgeGraph;

void initialize_graph(KnowledgeGraph* knowledgegraph) {
    knowledgegraph->entities = NULL;
    knowledgegraph->num_entities = 0;
}

void add_knowledge(KnowledgeGraph* knowledgegraph, char* e_label1, char* e_description1, char* r_label, char* r_description, char* e_label2, char* e_description2) {
    int has_entity1 = 0;
    int has_entity2 = 0;
    int entity1_index = 0;
    int entity2_index = 0;
    for (int i = 0; i < knowledgegraph->num_entities; i++) {
        if (strcmp((knowledgegraph->entities)[i]->label, e_label1) == 0) {
            has_entity1 = 1;
            entity1_index = i;
        }

        if (strcmp((knowledgegraph->entities)[i]->label, e_label2) == 0) {
            has_entity2 = 1;
            entity2_index = i;
        }
    }

    if (has_entity1 == 0) {
        knowledgegraph->num_entities++;
        knowledgegraph->entities = (Entity**)realloc(knowledgegraph->entities, sizeof(Entity*) * (knowledgegraph->num_entities));
        entity1_index = knowledgegraph->num_entities - 1;
        knowledgegraph->entities[knowledgegraph->num_entities - 1] = (Entity*)malloc(sizeof(Entity));
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->label = (char*)malloc(sizeof(e_label1) + 1);
        strcpy(knowledgegraph->entities[knowledgegraph->num_entities - 1]->label, e_label1);
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->description = (char*)malloc(sizeof(e_description1) + 1);
        strcpy(knowledgegraph->entities[knowledgegraph->num_entities - 1]->description, e_description1);
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->num_relations = 0;
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->relations = NULL;
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->x = rand() % SCREEN_WIDTH;
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->y = rand() % SCREEN_HEIGHT;
    }
    if (has_entity2 == 0) {
        knowledgegraph->num_entities++;
        knowledgegraph->entities = (Entity**)realloc(knowledgegraph->entities, sizeof(Entity*) * (knowledgegraph->num_entities));
        entity2_index = knowledgegraph->num_entities - 1;
        knowledgegraph->entities[knowledgegraph->num_entities - 1] = (Entity*)malloc(sizeof(Entity));
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->label = (char*)malloc(sizeof(e_label2) + 1);
        strcpy(knowledgegraph->entities[knowledgegraph->num_entities - 1]->label, e_label2);
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->description = (char*)malloc(sizeof(e_description2) + 1);
        strcpy(knowledgegraph->entities[knowledgegraph->num_entities - 1]->description, e_description2);
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->num_relations = 0;
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->relations = NULL;
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->x = rand() % SCREEN_WIDTH;
        knowledgegraph->entities[knowledgegraph->num_entities - 1]->y = rand() % SCREEN_HEIGHT;
    }

    for (int i = 0; i < knowledgegraph->entities[entity1_index]->num_relations; i++) {
        if (strcmp(knowledgegraph->entities[entity1_index]->relations[i]->entity->label, e_label2) == 0) {
            return;
        }
    }
    knowledgegraph->entities[entity1_index]->num_relations++;
    knowledgegraph->entities[entity1_index]->relations = (Relation**)realloc(knowledgegraph->entities[entity1_index]->relations, sizeof(Relation*) * knowledgegraph->entities[entity1_index]->num_relations);
    knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations - 1] = (Relation*)malloc(sizeof(Relation));
    knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations - 1]->label = (char*)malloc(sizeof(r_label) + 1);
    strcpy(knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations - 1]->label, r_label);
    knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations - 1]->desription = (char*)malloc(sizeof(r_description) + 1);
    strcpy(knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations - 1]->desription, r_description);
    knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations - 1]->entity = knowledgegraph->entities[entity2_index];
}

void print_graph(KnowledgeGraph* knowledgegraph) {
    for (int i = 0; i < knowledgegraph->num_entities; i++) {
        printf("%s: ", knowledgegraph->entities[i]->label);
        for (int j = 0; j < knowledgegraph->entities[i]->num_relations; j++) {
            printf("%s %s, ", knowledgegraph->entities[i]->relations[j]->label, knowledgegraph->entities[i]->relations[j]->entity->label);
        }
        printf("\n");
    }
}

void destroy_graph(KnowledgeGraph* knowledgegraph) {
    if (knowledgegraph == NULL || knowledgegraph->entities == NULL) {
        return;
    }
    for (int i = 0; i < knowledgegraph->num_entities; i++) {
        Entity* entity = knowledgegraph->entities[i];
        if (entity == NULL) {
            continue;
        }
        free(entity->label);
        free(entity->description);

        for (int j = 0; j < entity->num_relations; j++) {
            Relation* relation = entity->relations[j];
            if (relation == NULL) {
                continue;
            }
            free(relation->label);
            free(relation);
        }
        free(entity->relations);
        free(entity);
    }
    free(knowledgegraph->entities);
    knowledgegraph->num_entities = 0;
}

void render_entities(SDL_Renderer* renderer, KnowledgeGraph* knowledgegraph, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background color to black

    // Clear the screen
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white

    // Render each entity as a white filled circle
    for (int i = 0; i < knowledgegraph->num_entities; i++) {
        // Draw the circle
        for (int y = -ENTITY_RADIUS; y <= ENTITY_RADIUS; y++) {
            for (int x = -ENTITY_RADIUS; x <= ENTITY_RADIUS; x++) {
                if (x * x + y * y <= ENTITY_RADIUS * ENTITY_RADIUS) {
                    SDL_RenderDrawPoint(renderer, knowledgegraph->entities[i]->x + x, knowledgegraph->entities[i]->y + y);
                }
            }
        }

        // Render entity name in the middle of the node
        SDL_Color textColor = { 0, 0, 0, 255 }; // Black color
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, knowledgegraph->entities[i]->label, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { knowledgegraph->entities[i]->x - strlen(knowledgegraph->entities[i]->label) * 3, knowledgegraph->entities[i]->y - 10, strlen(knowledgegraph->entities[i]->label) * 6, 20 };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Render edges
        for (int j = 0; j < knowledgegraph->entities[i]->num_relations; j++) {
            Entity* relatedEntity = knowledgegraph->entities[i]->relations[j]->entity;
            // Draw a line between the centers of the two entities
            SDL_RenderDrawLine(renderer, knowledgegraph->entities[i]->x, knowledgegraph->entities[i]->y, relatedEntity->x, relatedEntity->y);

            // Render relation name along the edge
            textSurface = TTF_RenderText_Solid(font, knowledgegraph->entities[i]->relations[j]->label, textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = { (knowledgegraph->entities[i]->x + relatedEntity->x) / 2, (knowledgegraph->entities[i]->y + relatedEntity->y) / 2, strlen(knowledgegraph->entities[i]->relations[j]->label) * 6, 20 };
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

        // Update forces here.
        // Relations attract get c_1 log (distance/c2)
        // Entities repel c3/d^2



    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL initialization failed! Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf initialization failed! Error: %s\n", TTF_GetError());
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Knowledge Graph", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed! Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed! Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("res/arial.ttf", 50);
    if (!font) {
        printf("Failed to load font! Error: %s\n", TTF_GetError());
        return 1;
    }

    // Create knowledge graph
    KnowledgeGraph* knowledgegraph = malloc(sizeof(KnowledgeGraph));
    initialize_graph(knowledgegraph);
    char* knowledge[] = { "socrates", "", "is a", "", "mortal", "" };
    char* knowledge2[] = { "mortal", "", "is a property of", "", "thing", "" };
    add_knowledge(knowledgegraph, knowledge[0], knowledge[1], knowledge[2], knowledge[3], knowledge[4], knowledge[5]);
    add_knowledge(knowledgegraph, knowledge2[0], knowledge2[1], knowledge2[2], knowledge2[3], knowledge2[4], knowledge2[5]);
    print_graph(knowledgegraph);

    // Event loop
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        time_t start_time, end_time;
        time(&start_time);
        while (difftime(start_time, end_time)<1){ //1 Frame per Second.
            time(&end_time);
        }
        render_entities(renderer, knowledgegraph, font);
    }

    // Cleanup
    TTF_CloseFont(font);
    TTF_Quit();
    destroy_graph(knowledgegraph);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(knowledgegraph);

    return 0;
}
