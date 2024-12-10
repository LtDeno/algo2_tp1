#ifndef SPLIT_H_
#define SPLIT_H_

/**
 * Recibe un string y un separador y devuelve un vector dinámico que contenga
 * los substrings delimitados por el separador. Al final del vector debe haber
 * un string NULL para saber que es el final del mismo.
 *
 * Si el string es nulo o no hay memoria disponible, se debe devolver NULL.
 *
 * Tanto el vector dinámico devuelto como los strings almacenados en el mismo
 * deben reservados con malloc/calloc y es el usuario quien se debe encargar de
 * liberar la memoria.
 *
 * Ejemplos:
 *
 * split("123:456:789:0", ':') => ["123", "456", "789", "0", NULL]
 *
 * split("Bienvenidos a Algo2", ' ') => ["Bienvenidos", "a", "Algo2", NULL]
 *
 * split("Hola mundo", '?') => ["Hola mundo", NULL]
 *
 * split(",1,2,3,", ',') => ["", "1", "2", "3", "", NULL]
 *
 * split("", ',') => ["", NULL]
 *
 * split(NULL, ';') => NULL
 *
 */
char** split(const char* string, char separador);

#endif // SPLIT_H_
