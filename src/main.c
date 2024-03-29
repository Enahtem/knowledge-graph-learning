#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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


// aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

typedef struct Entity{
    char *label;
    char *description;
    int x;
    int y;
    struct Relation **relations;
    int num_relations;
} Entity;

typedef struct Relation{
    char *label;
    char *desription;
    struct Entity *entity;
}Relation;

typedef struct KnowledgeGraph{
    Entity **entities;
    int num_entities;
}KnowledgeGraph;



void initialize_graph(KnowledgeGraph *knowledgegraph){
    knowledgegraph->entities=NULL;
    knowledgegraph->num_entities=0;
}

void add_knowledge(KnowledgeGraph *knowledgegraph, char *e_label1, char *e_description1, char *r_label, char *r_description, char *e_label2, char *e_description2){
    int has_entity1 = 0;
    int has_entity2 = 0;
    int entity1_index = 0;
    int entity2_index = 0;
    for (int i =0;i<knowledgegraph->num_entities;i++){
        if (strcmp((knowledgegraph->entities)[i]->label, e_label1)==0){
            has_entity1=1;
            entity1_index=i;
        }
        
        if (strcmp((knowledgegraph->entities)[i]->label, e_label2)==0){
            has_entity2=1;
            entity2_index=i;
        }
    }

    if (has_entity1 == 0){
        knowledgegraph->num_entities++;
        knowledgegraph->entities = (Entity **)realloc(knowledgegraph->entities, sizeof(Entity *)*(knowledgegraph->num_entities));
        entity1_index=knowledgegraph->num_entities-1;
        knowledgegraph->entities[knowledgegraph->num_entities-1]=(Entity *)malloc(sizeof(Entity));
        knowledgegraph->entities[knowledgegraph->num_entities-1]->label=(char *)malloc(sizeof(e_label1)+1);
        strcpy(knowledgegraph->entities[knowledgegraph->num_entities-1]->label, e_label1);
        knowledgegraph->entities[knowledgegraph->num_entities-1]->description=(char *)malloc(sizeof(e_description1)+1);
        strcpy(knowledgegraph->entities[knowledgegraph->num_entities-1]->description, e_description1);
        knowledgegraph->entities[knowledgegraph->num_entities-1]->num_relations=0;
        knowledgegraph->entities[knowledgegraph->num_entities-1]->relations=NULL;
        knowledgegraph->entities[knowledgegraph->num_entities-1]->x=0;
        knowledgegraph->entities[knowledgegraph->num_entities-1]->y=0;
    }
    if (has_entity2==0){
        knowledgegraph->num_entities++;
        knowledgegraph->entities = (Entity **)realloc(knowledgegraph->entities, sizeof(Entity *)*(knowledgegraph->num_entities));
        entity2_index=knowledgegraph->num_entities-1;
        knowledgegraph->entities[knowledgegraph->num_entities-1]=(Entity *)malloc(sizeof(Entity));
        knowledgegraph->entities[knowledgegraph->num_entities-1]->label=(char *)malloc(sizeof(e_label2)+1);
        strcpy(knowledgegraph->entities[knowledgegraph->num_entities-1]->label, e_label2);
        knowledgegraph->entities[knowledgegraph->num_entities-1]->description=(char *)malloc(sizeof(e_description2)+1);
        strcpy(knowledgegraph->entities[knowledgegraph->num_entities-1]->description, e_description2);
        knowledgegraph->entities[knowledgegraph->num_entities-1]->num_relations=0;
        knowledgegraph->entities[knowledgegraph->num_entities-1]->relations=NULL;
        knowledgegraph->entities[knowledgegraph->num_entities-1]->x=0;
        knowledgegraph->entities[knowledgegraph->num_entities-1]->y=0;
    }


    for (int i =0;i<knowledgegraph->entities[entity1_index]->num_relations;i++){
        if(strcmp(knowledgegraph->entities[entity1_index]->relations[i]->entity->label, e_label2)==0){
            return;
        }
    }
    knowledgegraph->entities[entity1_index]->num_relations++;
    knowledgegraph->entities[entity1_index]->relations = (Relation **)realloc(knowledgegraph->entities[entity1_index]->relations,sizeof(Relation *)*knowledgegraph->entities[entity1_index]->num_relations);
    knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations-1]=(Relation *)malloc(sizeof(Relation));
    knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations-1]->label=(char *)malloc(sizeof(r_label)+1);
    strcpy(knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations-1]->label, r_label);
    knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations-1]->desription=(char *)malloc(sizeof(r_description)+1);
    strcpy(knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations-1]->desription, r_description);
    knowledgegraph->entities[entity1_index]->relations[knowledgegraph->entities[entity1_index]->num_relations-1]->entity=knowledgegraph->entities[entity2_index];
}

void print_graph(KnowledgeGraph *knowledgegraph){
    for (int i =0;i<knowledgegraph->num_entities;i++){
        printf("%s: ", knowledgegraph->entities[i]->label);
        for (int j =0;j<knowledgegraph->entities[i]->num_relations;j++){
            printf("%s %s, ", knowledgegraph->entities[i]->relations[j]->label, knowledgegraph->entities[i]->relations[j]->entity->label);
        }
        printf("\n");
    }
}


// Use this coding style to improve self.
void destroy_graph(KnowledgeGraph *knowledgegraph) {
    if (knowledgegraph == NULL || knowledgegraph->entities == NULL) {
        return;
    }
    for (int i = 0; i < knowledgegraph->num_entities; i++) {
        Entity *entity = knowledgegraph->entities[i];
        if (entity == NULL) {
            continue;
        }
        free(entity->label);
        free(entity->description);

        for (int j = 0; j < entity->num_relations; j++) {
            Relation *relation = entity->relations[j];
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



void display_graph(KnowledgeGraph *knowledgegraph){
    // SDL Stuff Here
}

/*
Display Knowledge Graph
Delete/Edit Knowledge
*/

int main(int argc, char *argv[]) {
    // Personal Testing
    KnowledgeGraph *knowledgegraph=(KnowledgeGraph *)malloc(sizeof(KnowledgeGraph));
    initialize_graph(knowledgegraph);

    char *knowledge[] = {"socrates","","is a","","mortal",""};
    char *knowledge2[] = {"mortal","","is a property of","","socrates",""};

    add_knowledge(knowledgegraph, knowledge[0], knowledge[1], knowledge[2], knowledge[3], knowledge[4], knowledge[5]);
    add_knowledge(knowledgegraph, knowledge2[0], knowledge2[1], knowledge2[2], knowledge2[3], knowledge2[4], knowledge2[5]);
    print_graph(knowledgegraph);
    destroy_graph(knowledgegraph);






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
