#include "split.h"
#include <stdlib.h>

/* Devuelve la cantidad de separadores pasados en el string pasado.
   Devuelve 0 si el string es NULL. */
size_t cant_separadores(const char* string, char separador){
	if(!string[0]){
		return 0;
	}

	if (string[0] == separador){
		return 1 + cant_separadores(string + 1, separador);
	} else {
		return cant_separadores(string + 1, separador);
	}
}

/* Cuenta la cantidad de caracteres hasta el siguiente separador o fin de string.
   Devuelve 0 si el string es NULL. */
size_t cant_chars_hasta_terminador(const char* string, char separador){
	if (!string){
		return 0;
	}

	if (string[0] == separador || !string[0]){
		return 0;
	} else {
		return 1 + cant_chars_hasta_terminador(string + 1, separador);
	}
}

/* El puntero devuelto contiene la el tamanio pedido como cantidad de caracteres copiados del string.
   Devuelve NULL si no se pudo crear el puntero.*/
char* conseguir_substring(const char* string, size_t tamanio_substring){
	char* substring = malloc((tamanio_substring + 1) * sizeof(char));
	if (!substring){
		return NULL;
	}

	for (size_t contador = 0 ; contador < tamanio_substring ; contador++){
		substring[contador] = string[contador];
	}
	substring[tamanio_substring] = 0;

	return substring;
}

char** split(const char* string, char separador){
	if (!string){
		return NULL;
	}

	size_t cant_substrings = cant_separadores(string, separador) + 1;

	char** splited = (char**)calloc(cant_substrings + 1, sizeof(void*));
	if (!splited){
		return NULL;
	}

	for (size_t contador_substrings = 0 ; contador_substrings < cant_substrings ; contador_substrings++){
		size_t tamanio_substring = cant_chars_hasta_terminador(string, separador);

		char* substring = conseguir_substring(string, tamanio_substring);

		splited[contador_substrings] = substring;
		string += tamanio_substring + 1;
	}

	return splited;
}