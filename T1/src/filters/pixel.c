#include <stdlib.h>
#include <stdio.h>
#include "../imagelib/image.h"
#include "pixel.h"

int find_min(Image* img)
{
    int min = 127;
    for (int pixel = 0; pixel < img -> pixel_count; pixel++)
    {
        if(img -> pixels[pixel] < min)
        {
            min = img -> pixels[pixel];
        }
    }
    return min;
}

Pixel* pixel_init(int id, int grisaceo, int row, int col, Image* img)
{
    Pixel* pixel = malloc(sizeof(Pixel));

    pixel -> id = id;
    pixel -> grisaceo = grisaceo;
    pixel -> pixel_up = NULL;
    pixel -> pixel_down = NULL;
    pixel -> pixel_left = NULL;
    pixel -> pixel_right = NULL;

    pixel -> status = -1; // 0 visited, else visited, > 0 nodo
    pixel -> next = NULL;

    Pixel** vecinos = malloc(4 * sizeof(Pixel*));
    pixel -> vecinos = vecinos;

    pixel -> vecindario_next = NULL;
    return pixel;
}

void asignar_vecinos(Image* img, Pixel*** pixel_array)
{
    for (int row = 0; row < img -> height; row++)
    {
        for (int col = 0; col < img -> width; col++)
        {
            if (row == 0)
            {
                pixel_array[row][col] -> pixel_down = pixel_array[row + 1][col];
                if (col != 0 && col != img -> width - 1)
                {
                    pixel_array[row][col] -> pixel_right = pixel_array[row][col + 1];
                    pixel_array[row][col] -> pixel_left = pixel_array[row][col - 1];
                }
            }
            else if (row == img -> height - 1)
            {
                pixel_array[row][col] -> pixel_up = pixel_array[row - 1][col];
                if (col != 0 && col != img -> width - 1)
                {
                    pixel_array[row][col] -> pixel_right = pixel_array[row][col + 1];
                    pixel_array[row][col] -> pixel_left = pixel_array[row][col - 1];
                }
            }

            if (col == 0)
            {
                pixel_array[row][col] -> pixel_right = pixel_array[row][col + 1];
                if (row != 0 && row != img -> height - 1)
                {
                    pixel_array[row][col] -> pixel_down = pixel_array[row + 1][col];
                    pixel_array[row][col] -> pixel_up = pixel_array[row - 1][col];
                }
            }
            else if (col == img -> width - 1)
            {
                pixel_array[row][col] -> pixel_left = pixel_array[row][col - 1];
                if (row != 0 && row != img -> height - 1)
                {
                    pixel_array[row][col] -> pixel_down = pixel_array[row + 1][col];
                    pixel_array[row][col] -> pixel_up = pixel_array[row - 1][col];
                }
            }

            if (row != 0 && row != img -> height - 1 && col != 0 && col != img -> width - 1)
            {
                pixel_array[row][col] -> pixel_up = pixel_array[row - 1][col];
                pixel_array[row][col] -> pixel_down = pixel_array[row + 1][col];
                pixel_array[row][col] -> pixel_left = pixel_array[row][col - 1];
                pixel_array[row][col] -> pixel_right = pixel_array[row][col + 1];
	        }

            pixel_array[row][col] -> vecinos[0] = pixel_array[row][col] -> pixel_up;
            pixel_array[row][col] -> vecinos[1] = pixel_array[row][col] -> pixel_down;
            pixel_array[row][col] -> vecinos[2] = pixel_array[row][col] -> pixel_left;
            pixel_array[row][col] -> vecinos[3] = pixel_array[row][col] -> pixel_right;
        }   
    }
}

Queue* queue_init(int umbral)
{
    Queue* queue = malloc(sizeof(Queue));

    queue -> umbral = umbral;
    queue -> size = 0;
    queue -> front = NULL;
    queue -> back = NULL;

    return queue;
}

void queue_add(Queue* queue, Pixel* pixel)
{
    if (queue -> size == 0)
    {
        queue -> front = pixel;
    }
    else
    {
        pixel -> next = queue -> front;
        queue -> front  = pixel;
    }
    queue -> size++;
}

Pixel* queue_extract(Queue* queue)
{
    if (queue -> size == 0)
    {
        return NULL;
    }
    else
    {
        Pixel* pixel = queue -> front;
        if (queue -> size == 1)
        {
            queue -> front = NULL;
            queue -> size--;
            return pixel;
        }
        else
        {
            queue -> front = pixel -> next;
            queue -> size--;
            return pixel;
        }
    }
}

