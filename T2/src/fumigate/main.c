#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../imagelib/image.h"
#include "hash.h"

#define BLACK 0
#define WHITE 127
#define GRAY 64


int main(int argc, char** argv)
{  
  if (argc != 2)
  {
    printf("Modo de uso: %s INPUT \n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    return 1;
  }

  /* Abrimos el archivo de input */
  FILE* input_file = fopen(argv[1], "r");

  /* Leemos la ruta de la imagen principal */
  char filename[32];
  fscanf(input_file, "%s", filename);
  /* Abrimos la imagen principal */
  Image* image = img_png_read_from_file(filename);

  /* Aca puedes crear la tabla de hash */

  // Preparando
  for(int i = 0; i < image -> pixel_count; i++)
  {
    if(image -> pixels[i] == 127)
    {
      image -> pixels[i] = 1;
    }
  }

  int** pixel_array = malloc(image -> width * sizeof(int*));
  int counter = 0;
  for (int row = 0; row < image -> width; row++)
  {
    pixel_array[row] = malloc(image -> width * sizeof(int));
    for (int col = 0; col < image -> width; col++)
    {
      pixel_array[row][col] = image -> pixels[counter];
      counter++;
    }
  }

  // fin preparando


  int SIZE = (image -> width * 127) - 1;
  Hash* hash = init_hash(SIZE);

  prepare_hash(hash, image, pixel_array);

  for (int row = 0; row < image -> width; row++)
  {
    free(pixel_array[row]);
  }
  free(pixel_array);
  
  /* ----------------------------------------- */

  /* Leemos cada una de las consultas */
  int Q;
  char query_in[32];
  char query_out[32];
  fscanf(input_file, "%d", &Q);


  for (int q = 0; q < Q; q++)
  {
    /* Leemos las rutas de las imagenes */
    fscanf(input_file, "%s", query_in);
    fscanf(input_file, "%s", query_out);

    // printf("\n----  QUERY %d -----\n", q);
    /* Abrimos la imagen de consulta de input */
    Image* query_image = img_png_read_from_file(query_in);

    int* query_image_array = malloc(query_image -> pixel_count * sizeof(int));
    for(int i = 0; i < query_image -> pixel_count; i++)
    {
      if(query_image -> pixels[i] == 127)
      {
        query_image -> pixels[i] = 1;
      }
      query_image_array[i] = query_image -> pixels[i];
    }

    /*
    unsigned long long int query_bits = 0b0;
    for(int i = 0; i < query_image -> pixel_count; i++)
    {
      query_bits = bit_operate(query_bits, query_image -> pixels[i]);
    }
    */

    int hash_code = get_hash(hash, image, query_image_array, query_image -> width);

    /* Creamos una nueva imagen en blanco con las mismas dimensiones que la original */
    Image *out_image = calloc(1, sizeof(Image));
    out_image->height = image->height;
    out_image->width = image->width;
    out_image->pixel_count = image->pixel_count;
    out_image->pixels = calloc(image->pixel_count, sizeof(int));

    for (int i = 0; i < image -> pixel_count; i++)
    {
      out_image -> pixels[i] = image -> pixels[i];
      if (image -> pixels[i] == 0)
      {
        out_image -> pixels[i] = 0;
      }
      else if (image -> pixels[i] == 1)
      {
        out_image -> pixels[i] = 1;
      }
    }
    check_insect(image, out_image, hash, hash_code, query_image);
    /* pintamos los pixeles de su color final (borrar las siguientes tres lineas) */
      //out_image->pixels[0] = BLACK;
      //out_image->pixels[4] = WHITE; 
      //out_image->pixels[5] = GRAY;
    for (int i = 0; i < image -> pixel_count; i++)
    {
      if (out_image -> pixels[i] == 0)
      {
        out_image -> pixels[i] = BLACK;
      }
      else if (out_image -> pixels[i] == 1)
      {
        out_image -> pixels[i] = WHITE;
      }
      else if (out_image -> pixels[i] == -1)
      {
        out_image -> pixels[i] = GRAY;
      }
    }

    /* Escribimos la imagen de output*/
    img_png_write_to_file(out_image, query_out);


    /* Liberamos la memoria de las imagenes*/
    img_png_destroy(query_image);
    img_png_destroy(out_image);
  
  }

  /* Liberamos la memoria de la imagen principal */
  img_png_destroy(image);
  free_hash(hash);
  // free pixel array
  /* Cerramos el archivo de input */
  fclose(input_file);


  return 0;
}
