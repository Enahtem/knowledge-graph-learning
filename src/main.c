// FIX FORCE BASED DISPLAY


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>   

#define FPS 30
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define ENTITY_RADIUS 30


// Force-Based Display Constants (https://cs.brown.edu/people/rtamassi/gdhandbook/chapters/force-directed.pdf)
#define C1 2
#define C2 1
#define C3 1
#define C4 0.1


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
    char* description;
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
    Entity *entity1 = NULL;
    Entity *entity2 = NULL;

    // Entity 1 Exists?
    for (int i = 0; i < knowledgegraph->num_entities; i++) {
        if (strcmp((knowledgegraph->entities)[i]->label, e_label1) == 0) {
            has_entity1 = 1;
            entity1=knowledgegraph->entities[i];
            break;
        }
        
    }

    // Entity 2 Exists?
    for (int i = 0; i < knowledgegraph->num_entities; i++) {
        if (strcmp((knowledgegraph->entities)[i]->label, e_label2) == 0) {
                has_entity2 = 1;
                entity2 = knowledgegraph->entities[i];
                break;
        }
    }

    // Entity 1 Initialization
    if (has_entity1 == 0) {
        entity1 = (Entity*)malloc(sizeof(Entity));
        entity1->label = strdup(e_label1);
        entity1->description = strdup(e_description1);
        entity1->num_relations = 0;
        entity1->relations = NULL;
        entity1->x = rand() % SCREEN_WIDTH;
        entity1->y = rand() % SCREEN_HEIGHT;

        knowledgegraph->num_entities++;
        knowledgegraph->entities = (Entity**)realloc(knowledgegraph->entities, sizeof(Entity*) * (knowledgegraph->num_entities));
        knowledgegraph->entities[knowledgegraph->num_entities - 1] = entity1;
    }


    // Entity 2 Initialization
    if (has_entity2 == 0) {
        entity2 = (Entity*)malloc(sizeof(Entity));
        entity2->label = strdup(e_label2);
        entity2->description = strdup(e_description2);
        entity2->num_relations=0;
        entity2->relations=NULL;
        entity2->x=rand()%SCREEN_WIDTH;
        entity2->y=rand()%SCREEN_HEIGHT;

        knowledgegraph->num_entities++;
        knowledgegraph->entities = (Entity**)realloc(knowledgegraph->entities, sizeof(Entity*) * (knowledgegraph->num_entities));
        knowledgegraph->entities[knowledgegraph->num_entities - 1]=entity2;
    }

    for (int i = 0; i < entity1->num_relations; i++) {
        if (strcmp(entity1->relations[i]->entity->label, e_label2) == 0) {
            return;
        }
    }
    Relation *relation12 = (Relation*)malloc(sizeof(Relation));
    relation12->label=strdup(r_label);
    relation12->description=strdup(r_description);
    relation12->entity=entity2;

    entity1->num_relations++;
    entity1->relations = (Relation**)realloc(entity1->relations, sizeof(Relation*) * entity1->num_relations);
    entity1->relations[entity1->num_relations - 1] = relation12;
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

void move_entity(Entity *entity, double force_x, double force_y){
    entity->x += (int)(C4 * force_x);
    entity->y += (int)(C4 * force_y);
    if (entity->x<0){
        entity->x=0;
    } else if (entity->x > SCREEN_WIDTH){
        entity->x=SCREEN_WIDTH;
    }
    if (entity->y<0){
        entity->y=0;
    } else if (entity->y > SCREEN_HEIGHT){
        entity->y=SCREEN_HEIGHT;
    }
}

void update_forces(KnowledgeGraph* knowledgegraph) {
    for (int i = 0; i < knowledgegraph->num_entities; i++) {
        double force_x = 0;
        double force_y = 0;
        Entity* entity1 = knowledgegraph->entities[i];

        // Relation Attractive Force
        for (int j = 0; j < entity1->num_relations; j++) {
            Entity* relatedEntity = entity1->relations[j]->entity;
            double dx = relatedEntity->x-entity1->x;
            double dy = relatedEntity->y-entity1->y;
            double distance = sqrt(dx * dx + dy * dy);
            double spring_force = C1 * log(distance / C2);
            force_x = spring_force*(dx/distance);
            force_y = spring_force*(dy/distance);
            move_entity(entity1, force_x, force_y);
            move_entity(relatedEntity, -force_x, -force_y);
        }

        //Entity Repulsive Force
        for (int j = 0; j < knowledgegraph->num_entities; j++) {
            if (i != j) {
                Entity* entity2 = knowledgegraph->entities[j];
                double dx = entity2->x-entity1->x;
                double dy = entity2->y-entity1->y;
                double distance = sqrt(dx * dx + dy * dy);
                double repulsive_force = C3 / (distance*distance);
                force_x = repulsive_force*(dx/distance);
                force_y = repulsive_force*(dy/distance);
                move_entity(entity1, force_x, force_y);
                move_entity(entity2, -force_x, -force_y);
            }
        }
    }
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
            SDL_Color textColor = {255, 255, 255, 255 }; // White color
            textSurface = TTF_RenderText_Solid(font, knowledgegraph->entities[i]->relations[j]->label, textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = { (knowledgegraph->entities[i]->x + relatedEntity->x) / 2, (knowledgegraph->entities[i]->y + relatedEntity->y) / 2, strlen(knowledgegraph->entities[i]->relations[j]->label) * 6, 20 };
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

        update_forces(knowledgegraph);

    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

void delay_to_next_frame(clock_t* last_frame_time) {
    clock_t current_time = clock();
    double elapsed_time = ((double)current_time - *last_frame_time) / CLOCKS_PER_SEC;
    double frame_time = 1.0 / FPS;
    double remaining_time = frame_time - elapsed_time;

    if (remaining_time > 0) {
        int delay_ms = (int)(remaining_time * 1000);
        if (delay_ms > 0)
            SDL_Delay(delay_ms);
    }

    *last_frame_time = clock();
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    clock_t last_frame_time = clock(); // Initialize last frame time
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
    char* knowledge[][6] = {
        { "apple", "", "is a", "", "fruit", "" },
        { "banana", "", "is a", "", "fruit", "" },
        { "orange", "", "is a", "", "fruit", "" },
        { "fruit", "", "is a type of", "", "food", "" },
        { "food", "", "can be", "", "eaten", "" },
        { "eaten", "", "is a", "", "verb", "" },
        { "fruit", "", "contains", "", "vitamins", "" },
        { "vitamins", "", "are essential for", "", "health", "" },
        { "health", "", "is", "", "important", "" },
        { "important", "", "is a", "", "concept", "" },
        { "concept", "", "is related to", "", "idea", "" },
        { "idea", "", "can lead to", "", "innovation", "" },
        { "innovation", "", "is important for", "", "progress", "" },
        { "progress", "", "leads to", "", "development", "" },
        { "development", "", "is necessary for", "", "growth", "" }
    };

    for (int i = 0; i < sizeof(knowledge) / sizeof(knowledge[0]); i++) {
        add_knowledge(knowledgegraph, knowledge[i][0], knowledge[i][1], knowledge[i][2], knowledge[i][3], knowledge[i][4], knowledge[i][5]);
    }
    // print_graph(knowledgegraph);

    // Event loop
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        delay_to_next_frame(&last_frame_time);
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
