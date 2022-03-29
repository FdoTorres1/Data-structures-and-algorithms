#include <stdlib.h>
#include <stdio.h>
#include "person.h"
#include "world.h"

Person* person_init(int id, int state)
{
  Person* person = malloc(sizeof(Person));
  person -> id = id;
  person -> state = state;
  person -> head = NULL;
  person -> tail = NULL;
  person -> parent = NULL;
  person -> prev = NULL;
  person -> next = NULL;
  person -> recovered = 0;

  return person;
}

Person* append_contact(Person* self, int id, int state)
{
  Person* new = person_init(id, state);
  new -> parent = self;

  if (self -> head == NULL)
  {
    self -> head = new;
  }
  else
  {
    new -> prev = self -> tail;
    self -> tail -> next = new;
  }
  self -> tail = new;

  return new;
}

Person* search_contact(Person* self, int id)
{
  Person* current = self -> head;

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

void recursive_inform(Person* self, int depth, FILE* output_file)
{
  for (int i = 0; i < depth; i++)
  {
    fprintf(output_file, "    ");
  }
  fprintf(output_file, "%d:%d\n", self -> id, self -> state);
  Person* current = self -> head;
  while (current)
  {
    recursive_inform(current, depth + 1, output_file);
    current = current -> next;
  }
}

void person_move(Person* person_no_head, Person* person2)
{
  Person* current = person2 -> head;

  person_no_head -> head = person2 -> head;
  person2 -> head = NULL;

  while (current)
  {
    if (person2 -> tail == current)
    {
      person_no_head -> tail = current;
    }
    if (person_no_head -> state == 2)
    {
      if (current -> state == 0)
      {
        current -> state = 1;
      }
    }
    current -> parent = person_no_head;
    current = current -> next;
  }
}

void person_swap(Person* person1, Person* person2)
{
  Person* personvar = person_init(person2 -> id, person2 -> state);
  personvar -> head = person2 -> head;
  personvar -> tail = person2 -> tail;
  Person* current1 = person1 -> head;

  person2 -> head = person1 -> head;

  while (current1)
  {
    if (personvar -> state == 2)
    {
      if (current1 -> state == 0)
      {
        current1 -> state = 1;
      }
    }
    current1 -> parent = person2;
    current1 = current1 -> next;
  }

  Person* current2 = personvar -> head;

  person1 -> head = personvar -> head;

  while (current2)
  {
    if (person1 -> state == 2 && current2 -> state == 0)
    {
      current2 -> state = 1;
    }
    current2 -> parent = person1;
    current2 = current2 -> next;
  }
}

void person_recursive_destroy(Person* person, int depth)
{
  Person* current = person -> head;
  while (current)
  {
    person_recursive_destroy(current, depth + 1);
    free(current);
    current = current -> next;
  }
}

void recursive_statistics(int* n_statistics, Person* person, int depth, FILE* output_file)
{
  Person* current = person -> head;
  
  while (current)
  {
    if (current -> state == 0)
    {
      n_statistics[0] ++;
    }
    else if (current -> state == 1)
    {
      n_statistics[1] ++;
    }
    else if (current -> state == 2)
    {
      n_statistics[2] ++;
    }
    else if (current -> state == 3)
    {
      n_statistics[3] ++;
    }
    else if (current -> state == 4)
    {
      n_statistics[4] ++;
    }
    recursive_statistics(n_statistics, current, depth + 1, output_file);
    current = current -> next;
  }
}