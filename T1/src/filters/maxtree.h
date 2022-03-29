#pragma once

#include "../imagelib/image.h"
#include "pixel.h"


struct nodo;
typedef struct nodo Nodo;

struct nodo
{
	int id;
    int umbral;
    Pixel* pixel_first;
    Pixel* pixel_last;
    Nodo* parent;
    Nodo* prev;
    Nodo* next;
    Nodo* head;
    Nodo* tail;
};

struct level;
typedef struct level Level;

struct level
{
    Queue* queue;
    int number_nodes;
    int node_at_level;
    Nodo* nodo_first;
    Nodo* nodo_last;
};

struct maxtree;
typedef struct maxtree Maxtree;

struct maxtree
{
    Nodo* raiz;
    Pixel*** pixel_array;
    Level** levels;
    int height;
    int width;
    Pixel* pixel_min;
};

Pixel*** load_pixel_array(Image* img, int U, Maxtree* maxtree);


/* Crea los nodos del Maxtree */
int flood(Maxtree* maxtree, int u);

/* Instancia nodo */
Nodo* nodo_init(int id, int umbral);

Level** init_levels();

Level* level_init(int u);

Nodo* check_node(Maxtree* maxtree, int h, int id);

void assign_pixels(Maxtree* maxtree);

Nodo* search_node(Maxtree* maxtree, int id, int grisaceo);

void assign_vecindarios(Maxtree* maxtree);

void area_filter(Maxtree* maxtree, int G, int A);

void add_pixels_vecindarios(Nodo* current);


int check_1(Nodo* current, int G);