#pragma once

struct nodo
{
  int id;
  int tipo; // 0 cliente 1 centro.
  int representante_id;
};
typedef struct nodo Nodo;

struct subset
{
  int parent;
  int rank;
  int centro;
};
typedef struct subset Subset;

struct edge
{
	int nodo1;
  int nodo2;
  int costo;
  int id;
};
typedef struct edge Edge;

struct grafo
{
	int V;
  int E;
  int n_clientes;
  int n_centros;
  Edge** edge_array;
  // Nodo** nodo_array;
};
typedef struct grafo Grafo;


Grafo* grafo_init(int n_clientes, int n_centros, int E);
Edge* edge_init(int nodo1, int nodo2, int costo, int id);
Nodo* nodo_init(Grafo* grafo, int id);

int find(Subset* subsets, int i);

void apply_union(Subset* subsets, int x, int y);

int find_centro(Subset* subsets, int i);

int compare(const void *p, const void *q);

void kruskal(Grafo* grafo, FILE* output_file);


void free_grafo(Grafo* grafo);

