#include <stdio.h>
#include <stdlib.h>
#include "world.h"
#include "person.h"
#include <stdbool.h>

World* world_init_countries(int countries)
{
  World* myworld = malloc(sizeof(World));
  myworld -> countries = countries;
  
  /* Países son filas y regiones columnas, pueden haber países con más regiones */
  myworld -> world = malloc(countries * sizeof(int*));

  myworld -> people_count = malloc(countries * sizeof(int*));
  
  return myworld;
}

void world_init_regions(World* myworld, int idc, int regions)
{
    
    myworld -> world[idc] = malloc(regions * sizeof(Person));

    myworld -> people_count[idc] = malloc(regions * sizeof(int));
    
    for(int region = 0; region < regions; region++)
    {
      Person* zero = person_init(0, 2);
      
      myworld -> world[idc][region] = zero;

      myworld -> people_count[idc][region] = 1;
    }
}

Person* world_search(World* myworld, int country, int region, int depth, int* route)
{
  Person* person = myworld -> world[country][region];
  for (int id = 0; id < depth; id++)
  {
    person = search_contact(person, route[id]);
  }
  return person;
}

Person* add_contact(Person* person, int new_id)
{
  int new_state = 0;
  if (person -> state == 2)
  {
    new_state = 1;
  }
  else
  {
    new_state = 0;
  }
  Person* new_person = append_contact(person, new_id, new_state);

  return new_person;
}

void add_contacts(World* myworld, int country, int region, int depth, int* route, int n_contacts)
{
  Person* person = world_search(myworld, country, region, depth, route);
  for (int i = 0; i < n_contacts; i++)
  {
    int new_id = myworld -> people_count[country][region];
    Person* new_person = add_contact(person, new_id);
    myworld -> people_count[country][region]++;
  }
}

void world_destroy(World* myworld)
/* no suena bien el nombre pero podría entenderse como un Thanos snap :) */
{
  for(int cty = 0; cty < myworld -> countries; cty++)
  {
    free(myworld -> world[cty]);
    free(myworld -> people_count[cty]);
  }
  free(myworld -> world);
  free(myworld -> people_count);
  free(myworld);
}

void world_inform(World* myworld, int country, int region, FILE* output_file)
{
  Person* person = myworld -> world[country][region];
  recursive_inform(person, 0, output_file);
}

void world_recover(World* myworld, int country, int region, int depth, int* route)
{
  Person* person = world_search(myworld, country, region, depth, route);
  person -> state = 3;
  person -> recovered = 1;
}

void world_positive(World* myworld, int country, int region, int depth, int* route)
{
  Person* person = world_search(myworld, country, region, depth, route);
  if (person -> recovered == 0)
  {
    person -> state = 2;
  }
  if (person -> head != NULL)
  {
    Person* current = person -> head;
    while (current)
      {
        current -> state = 1;
        current = current -> next;
      }
  }
}

void world_destroy_persons(World* myworld, int country, int region)
{
  Person* person = myworld -> world[country][region];
  person_recursive_destroy(person, 0);
}

void world_negative(World* myworld, int country, int region, int depth, int* route)
{
  Person* person = world_search(myworld, country, region, depth, route);

  if (person -> parent -> head == person)
  {
    if (person -> next != NULL)
    {
      person -> parent -> head = person -> next;
      if (person -> next -> prev != NULL)
      {
        person -> next -> prev = NULL;
      }
    }
    else
    {
      person -> parent -> head = NULL;
      person -> parent -> tail = NULL;
    }
  }
  else if (person -> parent -> tail == person)
  {
    if (person -> prev != NULL)
    {
      person -> parent -> tail = person -> prev;
      person -> prev -> next = NULL;
    }
    else
    {
      person -> parent -> head = NULL;
      person -> parent -> tail = NULL;
    }
  }
  else
  {
    person -> prev -> next = person -> next;
    person -> next -> prev = person -> prev;
  }
  person_recursive_destroy(person, depth);
}

void world_correct(World* myworld, int country, int region, int depth1, int depth2, int* route1, int* route2)
{
  Person* person1 = world_search(myworld, country, region, depth1, route1);
  Person* person2 = world_search(myworld, country, region, depth2, route2);

  if (person1 -> head == NULL && person2 -> head == NULL)
  {
    /* pass */
  }
  
  else if (person1 -> head == NULL)
  {
    person_move(person1, person2);
  }
  else if (person2 -> head == NULL)
  {
    person_move(person2, person1);
  }
  else if (person1 -> head != NULL && person2 -> head != NULL)
  {
    person_swap(person1, person2);
  }
}

void world_statistics(World* myworld, int country, int region, FILE* output_file)
{
  int* n_statistics = malloc(5 * sizeof(int));
  for (int d = 0; d < 4; d++)
  {
    n_statistics[d] = 0;
  }
  Person* zero = myworld -> world[country][region];
  recursive_statistics(n_statistics, zero, 0, output_file);
  fprintf(output_file, "0:%d\n", n_statistics[0]);
  fprintf(output_file, "1:%d\n", n_statistics[1]);
  fprintf(output_file, "2:%d\n", n_statistics[2]);
  fprintf(output_file, "3:%d\n", n_statistics[3] + 1);
  free(n_statistics);
}
