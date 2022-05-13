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

int* get_subimage(Image* image, int width, int pos)
{
  int* subimage = malloc(width * width * sizeof(int));
  int alt_pos = pos;
  for (int i = 0; i < width * width; i++)
  {
    if (i % width == 0)
    {
      subimage[i] = image -> pixels[alt_pos];
      alt_pos++;
    }
    else if ((i + 1) % width == 0)
    {
      subimage[i] = image -> pixels[alt_pos];
      alt_pos = pos + (image -> width) * ((i + 1) / width);
    }
    else
    {
      subimage[i] = image -> pixels[alt_pos];
      alt_pos++;
    }
  }
  return subimage;
}

void prepare_hash(Hash* hash, Image* image, int** pixel_array)
{
  int pos = 0;
  for (int row = 0; row < image -> width; row++)
  {
    for (int col = 0; col < image -> width; col++)
    {
      for (int sub_width = 2; sub_width < image -> width; sub_width++)
      {
        if (image -> width - row >= sub_width && image -> width - col >= sub_width)
        {
          int* subimage = get_subimage(image, sub_width, pos);
          hash_insert(hash, image, subimage, sub_width, pos);
        }
      }
      pos++;
    }
  }
}

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
  int alt_pos = pos;
  for (int i = 0; i < size * size; i++)
  {
    if (i % size == 0)
    {
      if(image -> pixels[alt_pos] == 0)
      {
        out_image -> pixels[alt_pos] = -1;
      }
      
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
