#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../imagelib/image.h"
#include "pixel.h"
#include "maxtree.h"

int main(int argc, char** argv)
{
    // Revisamos los argumentos
    if(argc < 4) {
        printf("Modo de uso: %s <input.png> <output.png> <command> [args]\n", argv[0]);
        return 1;
    }

    // Cargamos la imagen original desde el archivo
    Image* image = img_png_read_from_file(argv[1]);

    /* ------------- POR IMPLEMENTAR -------------- */
    /* Aqui debes crear el MaxTree de la imagen.    */

    printf("\n");
    printf("-----INICIO-----\n");
    printf("\n");

    int U = find_min(image);
    
    Maxtree* maxtree = malloc(sizeof(Maxtree));
    Pixel*** pixel_array = load_pixel_array(image, U, maxtree);
    asignar_vecinos(image, pixel_array);

    Level** levels = init_levels();

    maxtree -> pixel_array = pixel_array;
    maxtree -> levels = levels;
    maxtree -> height = image -> height;
    maxtree -> width = image -> width;

    queue_add(maxtree -> levels[0] -> queue, maxtree -> pixel_min);
    flood(maxtree, U);

    printf("\n");
    assign_pixels(maxtree);
    //assign_vecindarios(maxtree);
    printf("\n");
    printf("-----------------\n");
    printf("\n");

    // Creamos una nueva imagen de igual tamaño, para el output
    Image* new_img = calloc(1, sizeof(Image));
    *new_img = (Image) {
        .height = image->height,
        .width = image->width,
        .pixel_count = image->pixel_count,
        .pixels = calloc(image->pixel_count, sizeof(int))
    };

    // Filtramos el arbol y lo guardamos en la imagen, segun el filtro que corresponda
    if (! strcmp("delta", argv[3]))
    {
        // Filtro DELTA
        float max_delta = atof(argv[4]);

        /* ------------- POR IMPLEMENTAR -------------- */
        /* Aqui debes implementar el filtro delta y     */
        /* guardar la imagen filtrada en new_img.       */

    }
    else if (! strcmp("area", argv[3]))
    {
        // Filtro AREA-COLOR
        int min_area = atoi(argv[4]);
        int threshold = atoi(argv[5]);

        /* ------------- POR IMPLEMENTAR -------------- */
        /* Aqui debes implementar el filtro de area y   */
        /* guardar la imagen filtrada en new_img.       */

        // area_filter(maxtree, threshold, min_area);
        
    }
    int count = 0;
    for (int row = 0; row < maxtree -> height; row++)
    {
        for (int col = 0; col < maxtree -> width; col++)
        {
            new_img -> pixels[count] = maxtree -> pixel_array[row][col] -> grisaceo;
            count++;
        }
    }
    // Exportamos la nueva imagen
    printf("Guardando imagen en %s\n", argv[2]);
    img_png_write_to_file(new_img, argv[2]);
    printf("Listo!\n");

    // Liberamos los recursos
    // free pixels
    // free maxtree
    // free nodes
    // free queues
    img_png_destroy(image);
    img_png_destroy(new_img);

    // Terminamos exitosamente
    return 0;
}