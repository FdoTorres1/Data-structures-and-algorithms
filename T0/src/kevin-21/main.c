#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "world.h"
#include "person.h"

/* Retorna true si ambos strings son iguales */
bool string_equals(char *string1, char *string2)
{
  return !strcmp(string1, string2);
}

/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    return false;
  }

  return true;
}

int main(int argc, char **argv)
{
  /* Si los parámetros del programa son inválidos */
  if (!check_arguments(argc, argv))
  {
    /* Salimos del programa indicando que no terminó correctamente */
    return 1;
  }

  /* Abrimos el archivo de input */
  FILE *input_file = fopen(argv[1], "r");

  /* Abrimos el archivo de output */
  FILE *output_file = fopen(argv[2], "w");

  char command[32];
  
  /* Leemos la cantidad de países */
  int n_countries;
  fscanf(input_file, "%d", &n_countries);

  /* [Por implementar] Generamos nuestro mundo */
  World* myworld = world_init_countries(n_countries);

  /* Leemos la cantidad de regiones de cada país */
  int n_regions;
  int* region_array;
  region_array = (int*)malloc(n_countries * sizeof(int));

  for (int cty = 0; cty < n_countries; cty++)
  {
    fscanf(input_file, "%d", &n_regions);
    region_array[cty] = n_regions;
    /* [Por implementar] Poblamos el país con regiones */
    world_init_regions(myworld, cty, n_regions);
  }
  myworld -> n_regions_countries = region_array;

  /* Número de eventos/líneas */
  int n_lines;
  fscanf(input_file, "%d", &n_lines);

  /* Declaramos variables para guardar: 
    - Id de país.
    - Id de región. 
    - Profundidad.
    - Id de contacto para las rutas
    - Número de contactos.
  */
  int country_id;
  int region_id;
  int depth;
  int contact_id;
  int n_contacts;

  while (n_lines)
  {
    /* Disminuimos en 1 el número de líneas por leer */
    n_lines--;

    /* Leemos la instrucción */
    fscanf(input_file, "%s", command);

    /* Obtenemos país, región, y profundidad */
    fscanf(input_file, "%d", &country_id);
    fscanf(input_file, "%d", &region_id);
    

    /* POSITIVE */
    if (string_equals(command, "ADD_CONTACTS"))
    {
      fscanf(input_file, "%d", &depth);
      int* route;
      route = (int*)malloc(depth * sizeof(int));
      /* Obtenemos la ruta desde el archivo*/
      for (int r = 0; r < depth; r++)
      {
        fscanf(input_file, "%d", &contact_id);
        route[r] = contact_id;
      }
      /* Obtenemos el numero de contactos */
      fscanf(input_file, "%d", &n_contacts);
      add_contacts(myworld, country_id, region_id, depth, route, n_contacts);
      free(route);

    } 
    else if (string_equals(command, "POSITIVE"))
    {

      fscanf(input_file, "%d", &depth);
      int* route;
      route = (int*)malloc(depth * sizeof(int));
      /* Obtenemos la ruta desde el archivo*/
      for (int r = 0; r < depth; r++)
      {
        fscanf(input_file, "%d", &contact_id);
        route[r] = contact_id;;
      }
      world_positive(myworld, country_id, region_id, depth, route);
      free(route);

    } 
    else if (string_equals(command, "NEGATIVE"))
    {
      
      fscanf(input_file, "%d", &depth);
      int* route;
      route = (int*)malloc(depth * sizeof(int));
      /* Obtenemos la ruta desde el archivo*/
      for (int r = 0; r < depth; r++)
      {
        fscanf(input_file, "%d", &contact_id);
        route[r] = contact_id;
      }
      world_negative(myworld, country_id, region_id, depth, route);
      free(route);

    } 
    else if (string_equals(command, "RECOVERED"))
    {
      
      fscanf(input_file, "%d", &depth);
      int* route;
      route = (int*)malloc(depth * sizeof(int));
      /* Obtenemos la ruta desde el archivo*/
      for (int r = 0; r < depth; r++)
      {
        fscanf(input_file, "%d", &contact_id);
        route[r] = contact_id;
      }
      world_recover(myworld, country_id, region_id, depth, route);
      free(route);

    } 
    else if (string_equals(command, "CORRECT"))
    {
      fscanf(input_file, "%d", &depth);
      int depth1 = depth;
      int* route1;
      route1 = (int*)malloc(depth1 * sizeof(int));
      /* Obtenemos la primera ruta desde el archivo*/
      
      for (int r = 0; r < depth1; r++)
      {
        fscanf(input_file, "%d", &contact_id);
        route1[r] = contact_id;
      }
      /* Obtenemos la segunda ruta desde el archivo*/
      fscanf(input_file, "%d", &depth);
      int depth2 = depth;
      int* route2;
      route2 = (int*)malloc(depth2 * sizeof(int));
      for (int r = 0; r < depth2; r++)
      {
        fscanf(input_file, "%d", &contact_id);
        route2[r] = contact_id;
      }
      world_correct(myworld, country_id, region_id, depth1, depth2, route1, route2);
      free(route1);
      free(route2);
    
    } 
    else if (string_equals(command, "INFORM"))
    {
      fprintf(output_file, "INFORM %d %d\n", country_id, region_id);
      world_inform(myworld, country_id, region_id, output_file);
      
    } 
    else if (string_equals(command, "STATISTICS"))
    {
      fprintf(output_file, "STATISTICS %d %d\n", country_id, region_id);
      world_statistics(myworld, country_id, region_id, output_file);
    }

  }
  /*  [Por implementar] Liberamos nuestra estructura */
  world_destroy(myworld);
  free(region_array);
  fclose(input_file);
  fclose(output_file);
  return 0;
}