#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo.h"

Grafo* grafo_init(int n_clientes, int n_centros, int E)
{
  Grafo* grafo = malloc(sizeof(Grafo));

  int V = n_clientes + n_centros;

  grafo -> edge_array = malloc(E * sizeof(Edge*));
  // grafo -> nodo_array = malloc(V * sizeof(Nodo*));
  grafo -> E = E;
  grafo -> V = V;
  grafo -> n_clientes = n_clientes;
  grafo -> n_centros = n_centros;

  return grafo;
  
}

Edge* edge_init(int nodo1, int nodo2, int costo, int id)
{
  Edge* edge = malloc(sizeof(Edge));
  edge -> nodo1 = nodo1;
  edge -> nodo2 = nodo2;
  edge -> costo = costo;
  edge -> id = id;
  return edge;
}

Nodo* nodo_init(Grafo* grafo, int id)
{
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo -> id = id;

  if (id < grafo -> n_clientes)
  {
    nodo -> tipo = 0;
  }
  else
  {
    nodo -> tipo = 1;
  }

  return nodo;
}

int compare(const void *a, const void *b)
{
  Edge* a1 = *(Edge**)a;
  Edge* b1 = *(Edge**)b;
  return (a1 -> costo - b1 -> costo);
}

void free_grafo(Grafo* grafo)
{
  for (int i = 0; i < grafo -> E; i++)
  {
    free(grafo -> edge_array[i]);
  }
  free(grafo -> edge_array);
  free(grafo);
}

int find(Subset* subsets, int i)
{
  if (subsets[i].parent != i)
  {
    subsets[i].parent = find(subsets, subsets[i].parent);
  }
  return subsets[i].parent;
}

int check_centro(Subset* subsets, int x_root, int y_root)
{
  if (subsets[x_root].centro != -1)
  {
    return subsets[x_root].centro;
  }
  else if (subsets[y_root].centro != -1)
  {
    return subsets[y_root].centro;
  }
  else
  {
    return -1;
  }
}

void apply_union(Subset* subsets, int x, int y)
{
  int x_root = find(subsets, x);
  int y_root = find(subsets, y);
  int centro = check_centro(subsets, x_root, y_root);
  
  if (subsets[x_root].rank < subsets[y_root].rank)
  {
    subsets[x_root].parent = y_root;
    subsets[y_root].centro = centro;
    
  }
  else if (subsets[x_root].rank > subsets[y_root].rank)
  {
    subsets[y_root].parent = x_root;
    subsets[x_root].centro = centro;
  }
  else
  {
    subsets[y_root].parent = x_root;
    subsets[x_root].centro = centro;
    subsets[x_root].rank++;
  }
}

void kruskal(Grafo* grafo, FILE* output_file)
{
  int e = 0;
  int i = 0;
  qsort(grafo -> edge_array, grafo -> E, sizeof(grafo -> edge_array[0]), compare);

  Subset* subsets = malloc(grafo -> V * sizeof(Subset));

  Edge** solution = malloc(grafo -> V * sizeof(Edge*)); // rellenar con nulls
  
  for (int v = 0; v < grafo -> V; v++)
  {
    solution[v] = NULL;
    subsets[v].parent = v;
    subsets[v].rank = 0;
    if (v < grafo -> n_clientes)
    {
      subsets[v].centro = -1;
    }
    else
    {
      subsets[v].centro = v;
    }
    
  }

  while (e < (grafo -> V) - 1 && i < grafo -> E)
  {
    Edge* next_edge = grafo -> edge_array[i++];

    int x = find(subsets, next_edge -> nodo1);
    int y = find(subsets, next_edge -> nodo2);

    if (x != y && (subsets[x].centro == -1 || subsets[y].centro == -1))
    {
      // si en alguno de los dos conjuntos no hay plantas, agregar
      // check_centro

      solution[e++] = next_edge;
      apply_union(subsets, x, y);
    }
  }
  
  int costo_minimo = 0;
  for (i = 0; i < e; ++i)
  {
    if (solution[i] != NULL)
    {
      costo_minimo += solution[i] -> costo;
    }
  }
  fprintf(output_file, "%d\n", costo_minimo);

  for (i = 0; i < e; ++i)
  {
    if (solution[i] != NULL)
    {
      fprintf(output_file, "%d\n", solution[i] -> id);
    }
  }
  free(solution);
  free(subsets);
}