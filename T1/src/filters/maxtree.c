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
/*
void assign_vecindarios(Maxtree* maxtree)
{
    for (int row = 0; row < maxtree -> height; row++)
    {
        for (int col = 0; col < maxtree -> width; col++)
        {
            Pixel* pixel = maxtree -> pixel_array[row][col];
            for (int u = 0; u < 128; u++)
            {
                if (pixel -> grisaceo > u && maxtree -> levels[u] -> nodo_first != NULL)
                {
                    Nodo* nodo_first = maxtree -> levels[u] -> nodo_first;
                    Nodo* current = nodo_first;
                    while (current)
                    {
                        if (current -> pixel_last != NULL)
                        {
                            current -> pixel_last -> next = pixel;
                            current -> pixel_last = pixel;
                        }
                        current = current -> next;
                    }
                }
            }
        }
    }
}
/*
void assign_vecindarios(Maxtree* maxtree)
{
    for (int u = 127; u > -1; u--)
    {
        if (maxtree -> levels[u] -> nodo_first != NULL)
        {
            Nodo* current = maxtree -> levels[u] -> nodo_first;
            while (current)
            {
                if (current -> parent != NULL)
                {
                    add_pixels_vecindarios(current);
                }
                current = current -> next;
            }
        }
    }
}

void add_pixels_vecindarios(Nodo* current)
{
    Pixel* pixel_current = current -> pixel_first;
    while (pixel_current)
    {
        current -> parent -> pixel_last -> next = pixel_current;
        current -> parent -> pixel_last = pixel_current;
        pixel_current = pixel_current -> next;
    }
}
*/
/* 
void area_filter(Maxtree* maxtree, int G, int A)
{
    printf("filtraré por area, soy pro\n");
    printf("G = %d, A = %d\n", G, A);
    for (int u = 0; u < 128; u++)
    {
        if (maxtree -> levels[u] -> nodo_first != NULL)
        {
            Nodo* current = maxtree -> levels[u] -> nodo_first;
            while (current)
            {
                if (current -> pixel_last != NULL)
                {
                    int result1 = check_1(current, G);

                }
                current = current -> next;
            }
        }
    }
}

int check_1(Nodo* current, int G)
{
    Pixel* pixel_current = current -> pixel_first;
    int c_pixeles = 0;
    int c_mayor = 0;
    while (pixel_current)
    {
        if (pixel_current -> grisaceo > G)
        {
            c_mayor++;
        }
        c_pixeles++;
        pixel_current = pixel_current -> next;
    }
    if (c_mayor == c_pixeles)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int check_2(Nodo* current, int A)
{

}

/* copia flood funcional
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
                if (p -> vecinos[vecino] -> status == 0)
                {
                    m = p -> vecinos[vecino] -> grisaceo;
                    queue_add(maxtree -> levels[m] -> queue, p -> vecinos[vecino]);
                    p -> vecinos[vecino] -> status = -1;
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
        int i = maxtree -> levels[h] -> number_nodes++;
        int j = maxtree -> levels[m] -> number_nodes++;
        //parent
    }
    else
    {
        printf("root node\n");
        // Nodo h no tiene padre (i=1), es el nodo raíz
    }
    maxtree -> levels[h] -> node_at_level = 0;
    maxtree -> levels[h] -> number_nodes++;
    return m;
}
*/

// ESTA FUNCIONA copia mejor
/*
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
                if (p -> vecinos[vecino] -> status == 0)
                {
                    m = p -> vecinos[vecino] -> grisaceo;
                    queue_add(maxtree -> levels[m] -> queue, p -> vecinos[vecino]);
                    p -> vecinos[vecino] -> status = -1;
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
        int i = maxtree -> levels[h] -> number_nodes++;
        int j = maxtree -> levels[m] -> number_nodes++;
        printf("lvl h : %d / lvl m : %d / i : %d / j : %d\n", h, m, i, j);
        Nodo* nodo_i = check_node(maxtree, h, i);
        Nodo* nodo_j = check_node(maxtree, m, j);
        nodo_i -> parent = nodo_j;
    }
    else
    {
        printf("root node\n");
        Nodo* nodo_root = check_node(maxtree, h, 0);
        maxtree -> raiz = nodo_root;
    }
    maxtree -> levels[h] -> node_at_level = 0;
    maxtree -> levels[h] -> number_nodes++;
    return m;
}
*/

/* 
int flood(Maxtree* maxtree, Pixel* pixel)
{
    printf("grisaceo = %d\n", pixel -> grisaceo);
    printf("node in level %d\n", maxtree -> levels[pixel -> grisaceo] -> node_at_level);

    pixel -> status = 1;
    if (maxtree -> levels[pixel -> grisaceo] -> node_at_level == 0)
    {
        maxtree -> levels[pixel -> grisaceo] -> node_at_level = 1;

        Nodo* nodo_first = nodo_init(0, pixel -> grisaceo);
        nodo_first -> pixel_first = pixel;
        nodo_first -> pixel_last = pixel;

        maxtree -> levels[pixel -> grisaceo] -> nodo_first = nodo_first;
        maxtree -> levels[pixel -> grisaceo] -> number_nodes = 1;
        maxtree -> levels[pixel -> grisaceo] -> nodo_last = nodo_first;
    }

    for (int v = 0; v < 3; v++)
    {
        if (pixel -> vecinos[v] != NULL)
        {
            Pixel* vecino = pixel -> vecinos[v];
            if (vecino -> status == 0)
            {

                if (vecino -> grisaceo == pixel -> grisaceo)
                {
                    // asignar el mismo nodo que el del pixel
                    maxtree -> levels[pixel -> grisaceo] -> nodo_last -> pixel_last -> next = vecino;
                    maxtree -> levels[pixel -> grisaceo] -> nodo_last -> pixel_last = vecino;
                    
                }
                else if (vecino -> grisaceo > pixel -> grisaceo)
                {
                    if (maxtree -> levels[pixel -> grisaceo] -> node_at_level == 0)
                    {
                        maxtree -> levels[pixel -> grisaceo] -> node_at_level = 1;

                        Nodo* nodo_first = nodo_init(0, pixel -> grisaceo);
                        nodo_first -> pixel_first = pixel;
                        nodo_first -> pixel_last = pixel;

                        maxtree -> levels[pixel -> grisaceo] -> nodo_first = nodo_first;
                        maxtree -> levels[pixel -> grisaceo] -> number_nodes = 1;
                        maxtree -> levels[pixel -> grisaceo] -> nodo_last = nodo_first;
                    }
                    else
                    {
                        if pixel -> grisaceo != maxtree -> levels[vecino -> grisaceo] -> nodo_last
                        {
                            crear nuevo nodo
                        }
                    }
                    
                }
                flood(maxtree, vecino);
            }
        }
    }
}
*/
