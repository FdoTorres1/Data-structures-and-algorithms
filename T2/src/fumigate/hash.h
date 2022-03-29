#pragma once
#include "../imagelib/image.h"

struct data;
typedef struct data Data;

struct data
{
  unsigned long long int key;
  int index;

  int pos;
  int size;

  Data* prev;
  Data* next;

};

struct list;
typedef struct list List;

struct list
{
  Data* head;
  Data* tail;
};

struct hash;
typedef struct hash Hash;

struct hash
{
  List** array;
  int size;
};


Hash* init_hash(int size);
Data* init_data(int index, int pos, int size);
List* init_list(Data* data);

void print_hash(Hash* hash);

void free_hash(Hash* hash);

/* --------------------------- */

void prepare_hash(Hash* hash, Image* image, int** pixel_array);

int bit_operate(unsigned long long int bits, int element);
// unsigned int get_subimage(Image* image, int width, int pos);
int* get_subimage(Image* image, int width, int pos);
// void recursive_subimage(Hash* hash, Image* image, int width, int pos);


void hash_insert(Hash* hash, Image* image, int* subimage, int width, int pos);
int get_hash(Hash* hash, Image* image, int* key, int width);

void list_append(List* list, Data* data);

void change_color(Image* image, Image* out_image, int pos, int size);
void check_insect(Image* image, Image* out_image, Hash* hash, int hash_code, Image* query_image);