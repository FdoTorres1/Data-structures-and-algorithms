#include <stdlib.h>
#include <stdio.h>
#include "../imagelib/image.h"
#include "maxtree.h"

Pixel*** load_pixel_array(Image* img, int U, Maxtree* maxtree)
{
    Pixel*** pixel_array = malloc(img -> height * sizeof(int*));

    int counter = 0;
    for (int row = 0; row < img -> height; row++)
    {
        pixel_array[row] = malloc(img -> width * sizeof(Pixel));
        
        for (int col = 0; col < img -> width; col++)
        {   
            Pixel* pixel = pixel_init(counter, img -> pixels[counter], row, col, img);
            pixel_array[row][col] = pixel;
            if (pixel -> grisaceo == U)
            {
                maxtree -> pixel_min = pixel;
            }
            counter++;
        }
    }
    return pixel_array;
}

Level* level_init(int u)
{
    Level* level = malloc(sizeof(Level));
    level -> number_nodes = 0;
    level -> queue = queue_init(u);
    level -> node_at_level = 0; // false
    level -> nodo_first = NULL;
    return level;
}

Level** init_levels(Image* img)
{
    Level** levels = malloc(128 * sizeof(Level*));

    for (int u = 0; u < 128; u++)
    {
        Level* level = level_init(u);
        levels[u] = level;
    }
    return levels;
}

Nodo* nodo_init(int id, int umbral)
{
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo -> id = id;
    nodo -> umbral = umbral;
    nodo -> head = NULL;
    nodo -> tail = NULL;
    nodo -> prev = NULL;
    nodo -> next = NULL;
    nodo -> pixel_first = NULL;
    nodo -> pixel_last = NULL;

    return nodo;
}

Nodo* check_node(Maxtree* maxtree, int h, int id)
{
    if (maxtree -> levels[h] -> nodo_first == NULL)
    {
        Nodo* nodo = nodo_init(id, h);
        maxtree -> levels[h] -> nodo_first = nodo;
        maxtree -> levels[h] -> nodo_last = nodo;
        nodo -> next = NULL;
        return nodo;
    }
    else
    {
        Nodo* current = maxtree -> levels[h] -> nodo_first;
        while (current)
        {
            if (current -> id == id)
            {
                return current;
            }
            current = current -> next;
        }
        Nodo* nodo = nodo_init(id, h);
        maxtree -> levels[h] -> nodo_last -> next = nodo;
        maxtree -> levels[h] -> nodo_last = nodo;
        nodo -> next = NULL;
        return nodo;
    }
}

// ESTA FUNCIONA

int flood(Maxtree* maxtree, int h)
{
    int m;
    // Mientras la queue del nivel tenga píxeles por revisar
    while (maxtree -> levels[h] -> queue -> size != 0)
    {
        
        // Extraemos el primero
        Pixel* p = queue_extract(maxtree -> levels[h] -> queue);
        int a = maxtree -> levels[h] -> number_nodes;
        p -> status = a + 1;
        
        // Recorremos los píxeles vecinos
        for (int vecino = 0; vecino < 3; vecino++)
        {
            if (p -> vecinos[vecino] != NULL)
            {
                if (p -> vecinos[vecino] -> status == -1)
                {
                    m = p -> vecinos[vecino] -> grisaceo;
                    queue_add(maxtree -> levels[m] -> queue, p -> vecinos[vecino]);
                    p -> vecinos[vecino] -> status = 0;
                    maxtree -> levels[m] -> node_at_level = 1;

                    if (m > h)
                    {
                        while (m > h)
                        {
                            m = flood(maxtree, m);
                        }
                    }
                }
            }
        }
    }
    m = h - 1;
    while (m >= 0 && maxtree -> levels[m] -> node_at_level == 0)
    {
        m--;
    }
    if (m >= 0)
    {   
        int x = maxtree -> levels[h] -> number_nodes;
        int b = maxtree -> levels[m] -> number_nodes;
        int i = x + 1;
        int j = b + 1;
        // printf("lvl h : %d / lvl m : %d / i : %d / j : %d\n", h, m, i, j);
        Nodo* nodo_i = check_node(maxtree, h, i);
        Nodo* nodo_j = check_node(maxtree, m, j);
        nodo_i -> parent = nodo_j;
    }
    else
    {
        // printf("root node\n");
        Nodo* nodo_root = check_node(maxtree, h, 0);
        maxtree -> raiz = nodo_root;
    }
    maxtree -> levels[h] -> node_at_level = 0;
    maxtree -> levels[h] -> number_nodes++;
    return m;
}

void assign_pixels(Maxtree* maxtree)
{
    for (int row = 0; row < maxtree -> height; row++)
    {
        for (int col = 0; col < maxtree -> width; col++)
        {
            Pixel* pixel = maxtree -> pixel_array[row][col];
            int id = pixel -> status;
            int grisaceo = pixel -> grisaceo;
            Nodo* nodo = search_node(maxtree, id, grisaceo);
            printf("%d, %d pixel status %d, nodo %p\n", row, col, id, nodo);
            if (nodo -> pixel_first == NULL)
            {
                nodo -> pixel_first = pixel;
                nodo -> pixel_last = pixel; 
            }
            else
            {
                nodo -> pixel_last -> next = pixel;
                nodo -> pixel_last = pixel;
            }
        }
    }
}

Nodo* search_node(Maxtree* maxtree, int id, int grisaceo)
{
    Nodo* nodo_first = maxtree -> levels[grisaceo] -> nodo_first;
    Nodo* current = nodo_first;
    while (current)
    {
        if (current -> id == id)
        {
            return current;
        }
        current = current -> next;
    }
    return NULL;
}
