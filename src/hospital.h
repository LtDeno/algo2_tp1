#ifndef HOSPITAL_H_
#define HOSPITAL_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct _hospital_pkm_t hospital_t;
typedef struct _pkm_t pokemon_t;


/**
 * Reserva memoria para un hospital y la inicializa.
 *
 * Devuelve NULL en caso de no poder.
 */
hospital_t* hospital_crear();

/**
 * Lee un archivo con entrenadores que hacen tratar a sus pokemon en el hospital y los agrega al mismo.
 *
 * Ver en el enunciado el formato del archivo. No se olviden de reservar memoria
 * para los elementos que leen y de liberar todo en caso de error (no, el
 * hospital no). Si algo falla, el hospital no es modificado.
 *
 * En caso de error devuelve false. Caso contrario, true.
 */
bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo);

/**
 * Devuelve la cantidad de entrenadores que actualmente hacen atender a sus
 * pokemon en el hospital.
 */
size_t hospital_cantidad_entrenadores(hospital_t* hospital);

/**
 * Devuelve la cantidad de pokemon que son atendidos actualmente en el hospital.
 */
size_t hospital_cantidad_pokemon(hospital_t* hospital);

/**
 * Aplica una función a cada uno de los pokemon almacenados en el hospital. La
 * función debe aplicarse a cada pokemon en orden alfabético.
 *
 * La función a aplicar recibe el pokemon y devuelve true o false. Si la función
 * devuelve true, se debe seguir aplicando la función a los próximos pokemon si
 * quedan. Si la función devuelve false, no se debe continuar.
 *
 * Devuelve la cantidad de pokemon a los que se les aplicó la función (hayan devuelto true o false).
 */
size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p));

/**
 * Guarda un archivo con el nombre dado. El formato del archivo debe ser igual
 * al del caso de lectura.
 *
 * Devuelve true si pudo guardarlo o false en caso de error.
 */
bool hospital_guardar_archivo(hospital_t* hospital, const char* nombre_archivo);

/**
 *  Libera el hospital y toda la memoria utilizada por el mismo.
 */
void hospital_destruir(hospital_t* hospital);

/**
 * Devuelve el nivel de un pokemon o 0 si el pokemon es NULL.
 */
size_t pokemon_nivel(pokemon_t* pokemon);

/**
 * Devuelve el nombre de un pokemon o NULL si el pokemon es NULL.
 */
const char* pokemon_nombre(pokemon_t* pokemon);

#endif // HOSPITAL_H_
