# JDLV
Jeu de la Vie en C (commencé le 22/01/2024) :

Inspiré par cette vidéo : https://youtu.be/S-W0NX97DB0

Fonctionne sur Windows avec mingw d'installé

Testé en utilisant VSCode : à partir du dossier principal :

>>> mingw32-make.exe main

>>> bin/main.exe

Il est possible de changer certains paramètres dans le code source (src/main.c):

WIN_W       : Longueur de la fenêtre
WIN_H       : Largeur de la fenêtre

SQUARE_SIZE : Taille en px d'un carré (correspond à une case)

À NOTER     : La taille de la fenêtre sera légèrement rétrécie pour devenir un multiple de SQUARE_SIZE

Couleur (entre 0 et 255) :
BG_R        : Couleur d'arrière-plan (rouge)
BG_G        : Couleur d'arrière-plan (vert)
BG_B        : Couleur d'arrière-plan (bleu)

Il est également possible d'ajouter des structures prédéfinies (ces structures sont en commentaire) ou même d'en créer par soi-même :

const int structure_<nom_de_la_structure>[TAB_HEIGHT][TAB_WIDTH] = {
  {1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
};

add_structure(map_tab, squares_to_add, &last_square_to_add, structure_<nom_de_la_structure>, <position (en px) par rapport au haut de la fenêtre>, <position (en px) par rapport au bord gauche de la fenêtre>);

"0" correspond à une case vide et "1" correspond à une case active.
Ce code est à ajouter après la ligne `SDL_Init(SDL_INIT_VIDEO);`

![Image du projet](https://github.com/MAMGA62/JDLV/blob/main/JDLV_image.png?raw=true)
