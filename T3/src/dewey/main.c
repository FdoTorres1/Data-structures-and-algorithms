#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo.h"


int main(int argc, char** argv)
{
  // Revisamos los argumentos
  if(argc != 3) {
    printf("Modo de uso: %s <network.txt> <output.txt>\n", argv[0]);
    return 1;
  }

  /* ------------- POR IMPLEMENTAR -------------- */
  /* El código de tu tarea va aquí.               */

  /* Abrimos archivos input/output */

  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  int n_clientes;
  int n_centros;
  int n_carreteras;

  fscanf(input_file, "%d", &n_clientes);
  fscanf(input_file, "%d", &n_centros);
  fscanf(input_file, "%d", &n_carreteras);

  Grafo* grafo = grafo_init(n_clientes, n_centros, n_carreteras);

  int nodo1;
  int nodo2;
  int costo;
  int n_lines = n_carreteras;

  int counter = 0;
  while (n_lines)
  {
    n_lines--;

    fscanf(input_file, "%d", &nodo1);
    fscanf(input_file, "%d", &nodo2);
    fscanf(input_file, "%d", &costo);

    Edge* edge = edge_init(nodo1, nodo2, costo, counter);
    grafo -> edge_array[counter] = edge;

    counter++;
  }

  kruskal(grafo, output_file);

  // Liberamos estructuras

  free_grafo(grafo);

  // Cerramos archivos
  fclose(input_file);
  fclose(output_file);

  // Terminamos exitosamente
  return 0;
}