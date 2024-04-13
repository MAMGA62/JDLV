
// Ajouter un texte qui affiche le numéro du tour

#include <stdio.h>
#include <SDL.h>

// Pour le random
#include <time.h>
#include <stdlib.h>


#define BG_R 0
#define BG_G 0
#define BG_B 0

#define NB_BOUCLES 10000

#define SQUARE_SIZE 9

// #define WIN_W 1720
// #define WIN_H 1000

#define WIN_W 1400
#define WIN_H 750

#define WINDOW_WIDTH (WIN_W - (WIN_W % SQUARE_SIZE))
#define WINDOW_HEIGHT (WIN_H - (WIN_H % SQUARE_SIZE))


// Définit les paramètres d'un rectangle
#define SETRECT(rect, vx, vy, vw, vh) rect.x = vx; rect.y = vy; rect.w = vw; rect.h = vh;

// "Ajoute" un rectangle : L'ajoute à la liste des rectangles actifs
#define ADDRECT(map, to_add, last_to_add, i, j) last_to_add++; to_add[last_to_add] = &map[i][j];

// "Retire" un rectangle : Affiche un rectangle de la même couleur que le fond, lui retire sa longueur, "l'enlève" à la liste des rectangles actifs et fait son rendu
#define REMOVERECT(map , to_remove, last_to_remove, i, j) last_to_remove++; to_remove[last_to_remove] = &map[i][j];

#define ISACTIVE(map, i, j) map[i][j].w != 0


#define TAB_WIDTH ((WINDOW_WIDTH) / SQUARE_SIZE)
#define TAB_HEIGHT ((WINDOW_HEIGHT) / SQUARE_SIZE)

// Permet d'ajouter une structure à partir d'une position initiale et d'un tableau qui le représente
void add_structure(SDL_Rect map[TAB_HEIGHT][TAB_WIDTH], SDL_Rect *to_add[TAB_HEIGHT * TAB_WIDTH], int *last_to_add, const int tab[TAB_HEIGHT][TAB_WIDTH], int i, int j);

// Permet d'ajouter des rectangles de façon aléatoire sur la map
void random_map(SDL_Rect map[TAB_HEIGHT][TAB_WIDTH], SDL_Rect *to_add[TAB_HEIGHT * TAB_WIDTH], int *last_to_add);

// Renvoie le nombre de voisins d'un rectangle
int nb_neighbors(SDL_Rect map[TAB_HEIGHT][TAB_WIDTH], int i, int j);

int main(int argc, char *argv[]){

    srand(time(NULL));   // Pour le random

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Tableau représentant la map
    SDL_Rect map_tab[TAB_HEIGHT][TAB_WIDTH];

    // Initialisation de la map
    for (int i = 0; i < TAB_HEIGHT; i++){

        for (int j = 0; j < TAB_WIDTH; j++){
            
            // On renseigne les coordonnées du rectangle
            // On considère qu'il n'y a pas de rectangle si sa longueur est nulle : donc on ne lui met pas de longueur par défaut
            // On lui donne quand même une hauteur pour qu'on ait plus qu'à modifier sa longueur pour l'afficher
            //          map             vx                  vy      vw      vh  
            SETRECT(map_tab[i][j], j * SQUARE_SIZE, i * SQUARE_SIZE, 0, SQUARE_SIZE);
        }
    }

    // Tableau qui contient l'addresse des cases à activer à la fin du tour (pointe vers un SDL_Rect de map_tab)
    SDL_Rect *squares_to_add[TAB_HEIGHT * TAB_WIDTH] = {};

    // Tableau qui contient l'addresse des cases à désactiver à la fin du tour (pointe vers un SDL_Rect de map_tab)
    SDL_Rect *squares_to_remove[TAB_HEIGHT * TAB_WIDTH] = {};

    // Variable qui stocke l'indice du dernier élément de squares_to_add
    int last_square_to_add = -1;

    // Variable qui stocke l'indice du dernier élément de squares_to_remove
    int last_square_to_remove = -1;

    // Variable qui stocke le nombre de voisins d'une case
    int neighbors = 0;

    printf("        JEUX DE LA VIE      \n");
    printf("----------------------------\n\n");
    printf("Taille de la map : %dX%d\n", TAB_WIDTH, TAB_HEIGHT);
    printf("Nombre de cases : %d\n", TAB_WIDTH * TAB_HEIGHT);
    printf("Taille d'une case : %dX%dpx\n\n", SQUARE_SIZE, SQUARE_SIZE);

    // Création de la fenêtre
    window = SDL_CreateWindow("Jeu de la vie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL){
        printf("Erreur pendant la création de la fenêtre...\n");
        exit(EXIT_FAILURE);
    }

    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL){
        printf("Erreur pendant la création du renderer...\n");
        exit(EXIT_FAILURE);
    }

    SDL_Init(SDL_INIT_VIDEO);

    // INITIALISATION -----------------------------------------------
    // Tableau initial

    // const int structure_grenouille[TAB_HEIGHT][TAB_WIDTH] = {
    //     {0, 0, 1, 0},
    //     {1, 0, 0, 1},
    //     {1, 0, 0, 1},
    //     {0, 1, 0, 0}
    // };

    // const int structure_planeur[TAB_HEIGHT][TAB_WIDTH] = {
    //     {1, 0, 1},
    //     {0, 1, 1},
    //     {0, 1, 0}
    // };

    // const int structure_floraison[TAB_HEIGHT][TAB_WIDTH] = {
    //     {0, 1, 1, 1, 0},
    //     {1, 0, 1, 0, 1},
    //     {0, 1, 1, 1, 0}
    // };

    // const int structure_square[TAB_HEIGHT][TAB_WIDTH] = {
    //     {1, 1},
    //     {1, 1}
    // };

    const int structure_cannon[TAB_HEIGHT][TAB_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // add_structure(map_tab, squares_to_add, &last_square_to_add, structure_grenouille, 10, 70);

    // add_structure(map_tab, squares_to_add, &last_square_to_add, structure_planeur, 0, 0);

    // add_structure(map_tab, squares_to_add, &last_square_to_add, structure_floraison, 10, 5);
    // add_structure(map_tab, squares_to_add, &last_square_to_add, structure_square, 10, 5);

    // random_map(map_tab, squares_to_add, &last_square_to_add);

    add_structure(map_tab, squares_to_add, &last_square_to_add, structure_cannon, TAB_HEIGHT - 10, 2);


    // On ajoute tous les rectangles à ajouter (Les 1ers)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (int i = 0; i <= last_square_to_add; i++){
        squares_to_add[i]->w = SQUARE_SIZE;
        SDL_RenderFillRect(renderer, squares_to_add[i]);
    }

    last_square_to_add = -1;

    // Update de l'écran
    SDL_RenderPresent(renderer);

    // FIN INITIALISATION -------------------------------------------

    // On part dans la boucle
    for (int run = 0; run < NB_BOUCLES; run++){

        // SDL_Delay(50);

        // Checks des cases
        for (int i = 0; i < TAB_HEIGHT; i++){

            for (int j = 0; j < TAB_WIDTH; j++){

                neighbors = nb_neighbors(map_tab, i, j);

                // if (neighbors > 0){
                //     printf("i : %d, j : %d  --> %d voisins\n", i, j, neighbors);
                // }

                if (ISACTIVE(map_tab, i, j)){
                    if (neighbors < 2 || neighbors > 3){
                        REMOVERECT(map_tab, squares_to_remove, last_square_to_remove, i, j);
                    }

                } else if (neighbors == 3){
                    ADDRECT(map_tab, squares_to_add, last_square_to_add, i, j);
                    //printf("+1\n"); 
                }
            }
        }

        // On supprime tous les rectangles à supprimer
        SDL_SetRenderDrawColor(renderer, BG_R, BG_G, BG_B, 255);

        for (int i = 0; i <= last_square_to_remove; i++){
            SDL_RenderFillRect(renderer, squares_to_remove[i]);
            squares_to_remove[i]->w = 0; 
        }

        // On ajoute tous les rectangles à ajouter
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int i = 0; i <= last_square_to_add; i++){
            squares_to_add[i]->w = SQUARE_SIZE;
            SDL_RenderFillRect(renderer, squares_to_add[i]);
        }

        last_square_to_add = -1;
        last_square_to_remove = -1;

        // Update de l'écran
        SDL_RenderPresent(renderer);

        SDL_Delay(50);

    }

    SDL_Delay(2000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

int nb_neighbors(SDL_Rect map[TAB_HEIGHT][TAB_WIDTH], const int i, const int j){

    int result = 0;

    for (int a = -1; a <= 1; a++){

        for (int b = -1; b <= 1; b++){

            // printf("a : %d, b : %d\n", a, b);

            if ((a || b) && ISACTIVE(map, (i + a) >= 0 ? (i + a) % TAB_HEIGHT : TAB_HEIGHT + i + a, (j + b) >= 0 ? (j + b) % TAB_WIDTH : TAB_WIDTH + j + b)){
                result++;
                // printf("+1\n");
            }

        }

    }

    return result;

}

void add_structure(SDL_Rect map[TAB_HEIGHT][TAB_WIDTH], SDL_Rect *to_add[TAB_HEIGHT * TAB_WIDTH], int *last_to_add, const int tab[TAB_HEIGHT][TAB_WIDTH], const int i, const int j){

    for (int a = 0; a < TAB_HEIGHT; a++){
        for (int b = 0; b < TAB_WIDTH; b++){

            if (tab[a][b] && !(ISACTIVE(map, (a + i) % TAB_HEIGHT, (b + j) % TAB_WIDTH))){
                ADDRECT(map, to_add, (*last_to_add), (a + i) % TAB_HEIGHT, (b + j) % TAB_WIDTH);
            }

        }
    }

}

void random_map(SDL_Rect map[TAB_HEIGHT][TAB_WIDTH], SDL_Rect *to_add[TAB_HEIGHT * TAB_WIDTH], int *last_to_add){
    int r;
    for (int a = 0; a < TAB_HEIGHT; a++){
        for (int b = 0; b < TAB_WIDTH; b++){
            
            r = rand() % 2;

            if (r){
                ADDRECT(map, to_add, (*last_to_add), a, b);
            }

        }
    }

}