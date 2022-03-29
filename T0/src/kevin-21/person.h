#pragma once

struct nodo;
typedef struct nodo Person;

struct nodo
{
    int id;
    int state;
    Person* head;
    Person* tail;
    Person* parent;
    Person* prev;
    Person* next;
    int recovered;
};

Person* person_init(int id, int state);

Person* append_contact(Person* self, int other_id, int state);
Person* search_contact(Person* self, int id);

void recursive_inform(Person* person, int depth, FILE* output_file);
void person_recursive_destroy(Person* person, int depth);

void person_swap(Person* person1, Person* person2);
void person_move(Person* person1, Person* person2);

void recursive_statistics(int* n_statistics, Person* person, int depth, FILE* output_file);


