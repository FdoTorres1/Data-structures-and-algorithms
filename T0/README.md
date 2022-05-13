# Tarea 0 2021-1

La base de python se ejecuta con 
``` python3 ./python/main.py tests/test.txt out.txt```

El programa en c se compila con ```make``` y se ejecuta con

``` ./kevin-21 tests/test.txt out.txt```

Esta tarea era introductoria a C, debíamos tomar como base código en python.

Hola! Bienvenid@ a mi README.

Comentarte en primer lugar que al comienzo, sobre todo para la construcción de ```structs```, donde la modelación no era tan similar a la del archivo base ```.py```, me basé mucho en los archivos ```linked_list.c``` y ```matrix.c```, junto con el resto de los archivos que se encontraban en la carpeta de Ayudantía 0 en la parte de ```structs```.

Para las funciones que venían dadas en archivo ```.py``` se me complicó un poco traspasarlas a ```C``` al principio dado que tenía armados mis ```struct``` de forma no muy conveniente con la lista ligada según salía en el material de la ayudantía, así que para seguir bien como estaba modelado en el ```.py```, quité el ```struct``` de la lista y dejé el de nodos y los uní entre si.

Avanzando en la tarea hubieron funciones que se me complejizaron más que otras, después de debuggear harto logré mis test easy, pero al primer medium error. Con esto, tuve que cambiar muchas funciones desde cero, en especial ```NEGATIVE```. Finalmente logré todos los medium. Los hard no supe qué era lo que me fallaba dado que el error era profundo y ```valgrind``` no me arrojó información de debuggeo valiosa por más que intenté.

De todas formas, se me hizo muy difícil el comienzo de la tarea, creo que estuve un poco menos de una semana entendiendo y haciendo hasta ```INFORM```, y el resto me salió en un par de días... me costó harto adaptarme a C.

Saludos!