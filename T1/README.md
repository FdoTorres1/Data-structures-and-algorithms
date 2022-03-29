# Base-T1-2021-1
Código base para tu tarea 1.

## Requerimientos
Debes tener instalada la librería `libpng` para poder leer las imágenes cómo matrices.  
Esta la puedes instalar con el comando:
```sudo apt install libpng-dev```

## Compilar y ejecutar
Compilar:
```make```

Ejecutar:
```./filters <input.png> <output.png> <tipo_de_filtro> <A G | D>```

./filters tests/test_1.png out.png area 100 50
./filters tests/moon.png out.png area 100 50


## Ejemplos:
Filtrar nodos con area mayor a 100 pixeles y grisáceo mayor a 50:
```./filters tests/img1 out.png area 100 50```

Filtrar nodos con delta menor a 0.25:
```./filters tests/img1 out.png delta 0.25```


## Notas

Por favor les pido en lo posible revisen mi código y no basen mi nota solo en los output, encuentro una pena que por más que uno haya avanzado si no se tiene el output idéntico no se cuenta. Siento que aprendí mucho con esta tarea a pesar de que no lo logré.

Para esta tarea utilicé las siguientes referencias como apoyo:

* Salembier, Philippe & Oliveras, Albert & Garrido, Luis.(1998). Antiextensive connected operators for image and sequence processing. IEEE transactions on image processing : a publication of the IEEE Signal Processing Society. 7. 555-70. 10.1109/83.663500. 