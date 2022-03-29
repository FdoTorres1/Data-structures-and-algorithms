#pragma once
#include "person.h"

struct world;
typedef struct world World;

struct world
{
    Person*** world;
    int** people_count;
    int countries;
    int max_regions;
    int* n_regions_countries;
};

World* world_init_countries(int countries);
void world_init_regions(World* myworld, int idc, int regions);

void add_contacts(World* myworld, int country, int region, int depth, int* route, int n_contacts);
Person* add_contact(Person* person, int new_id);

Person* world_search(World* myworld, int country, int region, int depth, int* route);

void world_inform(World* myworld, int country, int region, FILE* output_file);
void world_recover(World* myworld, int country, int region, int depth, int* route);
void world_positive(World* myworld, int country, int region, int depth, int* route);
void world_negative(World* myworld, int country, int region, int depth, int* route);

void world_destroy(World* myworld);
void world_destroy_persons(World* myworld, int countries, int regions);

void world_correct(World* myworld, int country, int region, int depth1, int depth2, int* route1, int* route2);
void world_statistics(World* myworld, int county, int region, FILE* output_file);
