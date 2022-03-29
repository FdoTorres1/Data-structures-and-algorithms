#pragma once

#include "../imagelib/image.h"

struct pixel;
typedef struct pixel Pixel;

struct pixel
{
	int id;
    int grisaceo;
    Pixel* pixel_up;
    Pixel* pixel_down;
    Pixel* pixel_left;
    Pixel* pixel_right;
    int status;
    Pixel* prev;
    Pixel* next;
    Pixel* head;
    Pixel* tail;
    Pixel** vecinos;
    int parent;
    Pixel* vecindario_next;
    
};

struct queue;
typedef struct queue Queue;

struct queue
{
    int umbral;
    Pixel* front;
    Pixel* back;
    int size;
};

/* Encuentra el menor grisáceo de la imagen */
int find_min(Image* img);

/* Instancia píxel */
Pixel* pixel_init(int id, int grisaceo, int row, int col, Image* img);

/* Asigna vecinos a cada pixel según corresponda */
void asignar_vecinos(Image* img, Pixel*** pixel_array);

/* Queue init */
Queue* queue_init(int umbral);

void queue_add(Queue* queue, Pixel* pixel);

Pixel* queue_extract(Queue* queue);