#include "hospital.h"
#include "split.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _hospital_pkm_t{
    size_t cantidad_pokemon;
    pokemon_t* vector_pokemones;
    size_t cantidad_entrenadores;
};

struct _pkm_t{
    char* nombre;
    size_t nivel;
};

hospital_t* hospital_crear(){
    return calloc(1, sizeof(hospital_t));
}

/* Recibe un archivo no nulo y abierto correctamente.
   Devuelve el string hasta el siguiente salto de linea o el fin del archivo.
   Devuelve NULL si no se pudo reservar memoria o si no se leyo ningun byte ->
   A. K. A. ya se recorrio todo el archivo.*/
char* obtener_siguiente_linea(FILE* archivo){
    size_t bytes_leidos = 0;
    size_t longitud = 128;
    char* linea = (char*)malloc(longitud);
    if (!linea){
        return NULL;
    }

    while (fgets(linea + bytes_leidos, (int)(longitud - bytes_leidos), archivo)){
        size_t chars_leidos = strlen(linea + bytes_leidos);
        
        if ((chars_leidos > 0) && (linea[bytes_leidos + chars_leidos - 1] == '\n')){
            linea[bytes_leidos + chars_leidos - 1] = 0;
            return linea;

        } else {
            char* linea_aux = (char*)realloc(linea, longitud * 2);
            if (!linea_aux){
                free(linea);
                return NULL;
            }
            linea = linea_aux;
            longitud *= 2;
        }
        bytes_leidos += chars_leidos;
    }

    if (bytes_leidos == 0){
        free(linea);
        return NULL;
    }

    return linea;
}

/* Utiliza insercion ordenada (alfabeticamente) para agregar un pokemon al vector de pokemones.
   Ya debe haber memoria reservada para el pokemon a agregar. */
void agregar_pokemon_ordenadamente(hospital_t* hospital, pokemon_t* pokemon){
    int contador;
    bool pokemon_agregado = false;
    pokemon_t* pokemon_aux = (pokemon_t*)calloc(1, sizeof(pokemon_t));
    if (!pokemon_aux){
        return;
    }

    for (contador = 0 ; contador < (*hospital).cantidad_pokemon ; contador++){
        if (strcmp((*pokemon).nombre, (*hospital).vector_pokemones[contador].nombre) < 0){
            (*pokemon_aux).nombre = (*hospital).vector_pokemones[contador].nombre;
            (*pokemon_aux).nivel = (*hospital).vector_pokemones[contador].nivel;

            (*hospital).vector_pokemones[contador].nombre = (*pokemon).nombre;
            (*hospital).vector_pokemones[contador].nivel = (*pokemon).nivel;

            (*pokemon).nombre = (*pokemon_aux).nombre;
            (*pokemon).nivel = (*pokemon_aux).nivel;

            pokemon_agregado = true;
        }
    }

    if (pokemon_agregado){
        (*hospital).vector_pokemones[contador].nombre = (*pokemon_aux).nombre;
        (*hospital).vector_pokemones[contador].nivel = (*pokemon_aux).nivel;
        (*hospital).cantidad_pokemon++;
    }

    if ((!pokemon_agregado) || (contador = 0)){
        (*hospital).vector_pokemones[contador].nombre = (*pokemon).nombre;
        (*hospital).vector_pokemones[contador].nivel = (*pokemon).nivel;
        (*hospital).cantidad_pokemon++;
    }

    free(pokemon_aux);
}

/* Devuelve la cantidad de pokemones (siendo esto la cantidad de elementos en el split sin contar
   el id del entrenador, el nombre del entrenador y los niveles de los pokemon). */
size_t cantidad_pokemones(char** linea_splitted){
    size_t contador = 0;
    while(linea_splitted[contador]){
        contador++;
    }
    return (contador - 2) / 2;
}

/* Recibe un puntero a hospital_t no nulo y correctamente inicializado, al igual que un puntero
   de strings spliteados no nulo. Ya debe haber memoria reservada para cada pokemon.
   Asigna los contenidos del split arbitrariamente al hospital segun la consigna del TP.
   El vector de pokemones se encontrara ordenado alfabeticamente.
   Aumenta la cantidad de pokemones del hospital segun cuantos habia en el split y la cantidad
   de entrenadores segun cuantos entrenadores habia en el split.*/
void splitted_a_hospital(hospital_t* hospital, char** splitted){
    size_t cant_pokemones_agregar = cantidad_pokemones(splitted);
    pokemon_t* pokemon_actual = (pokemon_t*)calloc(1, sizeof(pokemon_t));
    if (!pokemon_actual){
        return;
    }

    for (size_t i = 0 ; i < cant_pokemones_agregar; i++){
        (*pokemon_actual).nombre = splitted[2 + 2*i];
        (*pokemon_actual).nivel = (size_t)atoi(splitted[3 + 2*i]);
        agregar_pokemon_ordenadamente(hospital, pokemon_actual);
    }

    (*hospital).cantidad_entrenadores++;

    free(pokemon_actual);
}

/* Libera la memoria en el heap que es apuntada por el split pasado por parametro.
   El split no debe ser nulo. No libera la memoria de los nombres de los pokemon. */
void liberar_split_hospital(char** splitted){
    size_t cant_pokemones_a_liberar = cantidad_pokemones(splitted);
    free(splitted[0]);
    free(splitted[1]);
    for (size_t i = 0 ; i < cant_pokemones_a_liberar ; i++){
        free(splitted[3 + 2*i]);
    }
    free(splitted);
}

bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo){
        return false;
    }

    char* linea = obtener_siguiente_linea(archivo);
    if (!linea){
        fclose(archivo);
        return true;
    }
    
    while(linea){
        char** linea_splitted = split(linea, ';');
        size_t cant_pokemones_a_agregar = cantidad_pokemones(linea_splitted);

        pokemon_t* vector_pokemones_aux = (pokemon_t*)realloc((*hospital).vector_pokemones, (hospital->cantidad_pokemon + cant_pokemones_a_agregar) * sizeof(pokemon_t));
        if (!vector_pokemones_aux){
            fclose(archivo);
            free(linea);
        }
        hospital->vector_pokemones = vector_pokemones_aux;

        splitted_a_hospital(hospital, linea_splitted);

        free(linea);
        liberar_split_hospital(linea_splitted);

        linea = obtener_siguiente_linea(archivo);
    }

    fclose(archivo);
    free(linea);

    return true;
}

size_t hospital_cantidad_pokemon(hospital_t* hospital){
    if (!hospital){
        return 0;
    }
    return (*hospital).cantidad_pokemon;
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if (!hospital){
        return 0;
    }
    return (*hospital).cantidad_entrenadores;
}

size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if (!hospital || !funcion){
        return 0;
    }
    size_t cant_aplicaciones = 0;
    bool seguir_aplicando = true;

    while ((seguir_aplicando) && (cant_aplicaciones < (*hospital).cantidad_pokemon)){
        seguir_aplicando = funcion(&(*hospital).vector_pokemones[cant_aplicaciones]);
        cant_aplicaciones++;    
    }

    return cant_aplicaciones;
}

void hospital_destruir(hospital_t* hospital){
    if (!hospital){
        return;
    } 

    for (size_t i = 0 ; i < ((*hospital).cantidad_pokemon) ; i++){
        free((*hospital).vector_pokemones[i].nombre);
    }
    free((*hospital).vector_pokemones);
    free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon){
    if (!pokemon){
        return 0;
    }
    return (*pokemon).nivel;
}

const char* pokemon_nombre(pokemon_t* pokemon){
    if (!pokemon){
        return NULL;
    }
    return (*pokemon).nombre;   
}
