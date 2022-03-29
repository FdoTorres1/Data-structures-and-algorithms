#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#include <math.h>
#include "../imagelib/image.h"


Hash* init_hash(int size)
{
  Hash* hash = malloc(sizeof(Hash));
  hash -> array = malloc(size * sizeof(Data*));
  hash -> size = size;

  for(int i = 0; i < size; i++)
  {
    hash -> array[i] = NULL;
  }

  return hash;
}

Data* init_data(int index, int pos, int size)
{
  Data* data = malloc(sizeof(Data));

  data -> index = index;
  data -> pos = pos;
  data -> size = size;

  data -> prev = NULL;
  data -> next = NULL;

  return data;
}

List* init_list(Data* data)
{
  List* list = malloc(sizeof(List));
  list -> head = data;
  list -> tail = data;

  return list;
}

void list_append(List* list, Data* data)
{
  list -> tail -> next = data;
  data -> prev = list -> tail;
  list -> tail = data;
}

void print_hash(Hash* hash)
{
  printf("\n \t\tHASH \n\n");
  
  for(int i = 0; i < hash -> size; i++)
  {
    if(hash -> array[i] == NULL)
    {
      printf("\t%d\t->  NULL\n", i);
    }
    else
    {
      printf("\t%d\t->  ", i);
      Data* current = hash -> array[i] -> head;
      while (current)
      {
        printf(" %d::%d  ", current -> pos, current -> size);
        current = current -> next;
      }
      printf("\n");
    }
  }
  printf("\n");
}

void free_hash(Hash* hash)
{
  for(int i = 0; i < hash -> size; i++)
  {
    if(hash -> array[i] != NULL)
    {
      Data* current = hash -> array[i] -> head;
      current = current -> next;
      while (current)
      {
        free(current -> prev);
        current = current -> next;
      }
      free(hash -> array[i]);
    }
  }
  free(hash -> array);
  free(hash);
}
/*
int get_hash(Hash* hash, Image* image, unsigned long long int key, int width)
{
  unsigned long long int hash_code;

  hash_code = key ^ 14695981039346656037 >> key;

  hash_code = hash_code * 1099511628211;
  hash_code = hash_code % hash -> size;

  return hash_code;
}

unsigned int hash_insert(Hash* hash, Image* image, unsigned long long int key, int width, int pos)
{

  int hash_index = get_hash(hash, image, key, width);
  Data* data = init_data(key, hash_index, pos, width);

  if(hash -> array[hash_index] == NULL)
  {
    List* list = init_list(data);
    hash -> array[hash_index] = list;
  }
  else
  {
    list_append(hash -> array[hash_index], data);
  }
  return key;
}
*/

/* -------------------------------------------------------------- */

int get_hash(Hash* hash, Image* image, int* key, int width)
{
  int hash_code;

  int hash_code_prev = 0;

  for (int pixel = 0; pixel < (width - 1) * (width - 1); pixel++)
  {
    hash_code_prev = hash_code_prev ^ pixel;
  }

  hash_code = hash_code_prev;

  for (int pixel = 0; pixel < width * width; pixel++)
  {
    hash_code = hash_code + key[pixel] ^ pixel;
  }

  hash_code = hash_code ^ hash_code_prev;

  hash_code = (8191 * hash_code) % hash -> size;

  /*
  hash_code = key ^ 14695981039346656037 >> key;

  hash_code = hash_code * 1099511628211;
  hash_code = hash_code % hash -> size;
  */
  free(key);
  return hash_code;
}

void hash_insert(Hash* hash, Image* image, int* subimage, int width, int pos)
{

  int hash_index = get_hash(hash, image, subimage, width);
  Data* data = init_data(hash_index, pos, width);

  if(hash -> array[hash_index] == NULL)
  {
    List* list = init_list(data);
    hash -> array[hash_index] = list;
  }
  else
  {
    list_append(hash -> array[hash_index], data);
  }
}



/* -------------------------------------------------------------- */
/*
int bit_operate(unsigned long long int bits, int element)
{
  if (element == 0)
  {
    bits = (bits << 1) | 0b0;
  }
  else
  {
    bits = (bits << 1) | 0b1;
  }
  return bits;
}
*/
/*
unsigned int get_subimage(Image* image, int width, int pos)
{
  // int* subimage = malloc(width * width * sizeof(int));
  unsigned long long int bits = 0b0;
  int alt_pos = pos;
  for (int i = 0; i < width * width; i++)
  {
    if (i % width == 0)
    {
      // subimage[i] = image -> pixels[alt_pos];

      bits = bit_operate(bits, image -> pixels[alt_pos]);

      //esta no
      // subimage[i] = image -> pixels[pos + (image -> width) * (i / width)];
      alt_pos++;
    }
    else if ((i + 1) % width == 0)
    {
      // subimage[i] = image -> pixels[alt_pos];

      bits = bit_operate(bits, image -> pixels[alt_pos]);

      alt_pos = pos + (image -> width) * ((i + 1) / width);
    }
    else
    {
      // subimage[i] = image -> pixels[alt_pos];

      bits = bit_operate(bits, image -> pixels[alt_pos]);

      alt_pos++;
    }
  }
  return bits;
}
*/

int* get_subimage(Image* image, int width, int pos)
{
  int* subimage = malloc(width * width * sizeof(int));
  int alt_pos = pos;
  for (int i = 0; i < width * width; i++)
  {
    if (i % width == 0)
    {
      subimage[i] = image -> pixels[alt_pos];

      // bits = bit_operate(bits, image -> pixels[alt_pos]);

      //esta no
      // subimage[i] = image -> pixels[pos + (image -> width) * (i / width)];
      alt_pos++;
    }
    else if ((i + 1) % width == 0)
    {
      subimage[i] = image -> pixels[alt_pos];

      // bits = bit_operate(bits, image -> pixels[alt_pos]);

      alt_pos = pos + (image -> width) * ((i + 1) / width);
    }
    else
    {
      subimage[i] = image -> pixels[alt_pos];

      // bits = bit_operate(bits, image -> pixels[alt_pos]);

      alt_pos++;
    }
  }
  return subimage;
}

void prepare_hash(Hash* hash, Image* image, int** pixel_array)
{
  // hasta la de 2x2 de abajo a la derecha
  // int last_pixel = (image -> pixel_count - 1) - (image -> width + 1);

  int pos = 0;
  for (int row = 0; row < image -> width; row++)
  {
    for (int col = 0; col < image -> width; col++)
    {
      for (int sub_width = 2; sub_width < image -> width; sub_width++)
      {
        if (image -> width - row >= sub_width && image -> width - col >= sub_width)
        {
          // unsigned long long int sub_image_bits = get_subimage(image, sub_width, pos);
          // hash_insert(hash, image, sub_image_bits, sub_width, pos);
          int* subimage = get_subimage(image, sub_width, pos);
          hash_insert(hash, image, subimage, sub_width, pos);
        }
      }
      pos++;
    }
  }
}

/* -------------------------------------------------------------- */


void check_insect(Image* image, Image* out_image, Hash* hash, int hash_code, Image* query_image)
{
  if (hash -> array[hash_code] != NULL)
  {
    Data* current = hash -> array[hash_code] -> head;
    while (current)
    {
      int* subimage_array = get_subimage(image, current -> size, current -> pos);

      if (current -> size == query_image -> width)
      {
        int counter = 0;
        for (int i = 0; i < query_image -> pixel_count; i++)
        {
          // printf("\n%d, %d\n", subimage_array[i], query_image -> pixels[i]);
          if (subimage_array[i] != query_image -> pixels[i])
          {
            counter = 0;
            break;
          }
          else
          {
            counter++;
          }
        }
        if (counter == query_image -> pixel_count)
        {
          change_color(image, out_image, current -> pos, current -> size);
        }
      }
      free(subimage_array);
      current = current -> next;
    }
  }
}

void change_color(Image* image, Image* out_image, int pos, int size)
{
  // printf("\npos %d size %d, iguales", pos, size);
  int alt_pos = pos;
  for (int i = 0; i < size * size; i++)
  {
    if (i % size == 0)
    {
      if(image -> pixels[alt_pos] == 0)
      {
        out_image -> pixels[alt_pos] = -1;
      }
      
      //printf("%d\n", out_image -> pixels[alt_pos]);

      //esta no
      // subimage[i] = image -> pixels[pos + (image -> width) * (i / width)];
      alt_pos++;
    }
    else if ((i + 1) % size == 0)
    {
      if(image -> pixels[alt_pos] == 0)
      {
        out_image -> pixels[alt_pos] = -1;
      }
      alt_pos = pos + (out_image -> width) * ((i + 1) / size);
    }
    else
    {
      if(image -> pixels[alt_pos] == 0)
      {
        out_image -> pixels[alt_pos] = -1;
      }
      alt_pos++;
    }
  }
}









/* -------------------------------------------------------------- */



/*
void recursive_subimage(Hash* hash, Image* image, int width, int pos)
{
  unsigned int subimage_top_left = get_subimage(image, width - 1, pos);
  hash_insert(hash, subimage_top_left,width - 1, pos);

  unsigned int subimage_top_right = get_subimage(image, width - 1, pos + 1);
  hash_insert(hash, subimage_top_right, width - 1, pos + 1);

  unsigned int subimage_down_left = get_subimage(image, width - 1, pos + image -> width);
  hash_insert(hash, subimage_down_left, width - 1, pos + image -> width);
  
  unsigned int subimage_down_right = get_subimage(image, width - 1, pos + image -> width + 1);
  hash_insert(hash, subimage_down_right, width - 1, pos + image -> width + 1);

  if (width > 3)
  {
    //init_data
    //hash_insert()
    //printf("done");
    recursive_subimage(hash, image, width - 1, pos);
    recursive_subimage(hash, image, width - 1, pos + 1);
    recursive_subimage(hash, image, width - 1, pos + image -> width);
    recursive_subimage(hash, image, width - 1, pos + image -> width + 1);
  }
  else
  {
    // llamar a las siguientes
  }
}


 --------------------------------------------------------- */

// Comentarios de acá hacia abajo

/* --------------------------------------------------------- */




//printf("\n----------------\n");

  //printf("N = %d, subimages n - 1 = %d, pos = %d\n\n", width, width - 1, pos);
  
  //printf("top_left: \n");
  //int* subimage_top_left = get_subimage(image, width - 1, pos);
  /*for (int i = 0; i < (width - 1) * (width - 1); i++)
  {
    printf("%d, ", subimage_top_left[i]);
  } */
  //printf("top_right: \n");
  //int* subimage_top_right = get_subimage(image, width - 1, pos + 1);
  /* for (int i = 0; i < (width - 1) * (width - 1); i++)
  {
    printf("%d, ", subimage_top_right[i]);
  } */
  //printf("down_left: \n");
  //int* subimage_down_left = get_subimage(image, width - 1, pos + image -> width);
  /* for (int i = 0; i < (width - 1) * (width - 1); i++)
  {
    printf("%d, ", subimage_down_left[i]);
  } */
  //printf("down_right: \n");
  //int* subimage_down_right = get_subimage(image, width - 1, pos + image -> width + 1);
  /*for (int i = 0; i < (width - 1) * (width - 1); i++)
  {
    printf("%d, ", subimage_down_right[i]);
  } 
  */
  //free(subimage_top_left);
  //free(subimage_top_right);
  //free(subimage_down_left);
  //free(subimage_down_right);
  
  //printf("\n----------------\n");
  
  /* PARA LAS 4 SUB - IMAGENES hashear
  hash_code1 = recursive_hash(width - 1) ^ algo;
  hash_code2 = recursive_hash(width - 1) ^ algo;
  hash_code3 = recursive_hash(width - 1) ^ algo;
  hash_code4 = recursive_hash(width - 1) ^ algo;
  init_data()
  hash_insert(hash_code);
  */


/*
int bin_to_decimal(char key)
{
  // citar código

  char binaryNumber[] = key;
  int binary, dec = 0;

  binary = atoi(binaryNumber);

  for(int i = 0; binary; i++, binary /= 10)
      if (binary % 10)
          dec += pow(2, i);

  return dec;
}
*/

/*
void hash_insert(Hash* hash, Data* data)
{
  char key = data -> key;
  int hash_index = get_hash(key);

  if(hash -> array[hash_index] != NULL)
  {
    int a = 0;
  }
  else
  {
    hash -> array[hash_index] = data;
  }
}

*/


/*
char binaryNumber[] = "00000001000001001";
int bin, dec = 0;

bin = atoi(binaryNumber);

for(int i = 0; bin; i++, bin /= 10)
    if (bin % 10)
        dec += pow(2, i);

printf("%d\n", dec);
*/