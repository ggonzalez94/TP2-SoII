/** @file radar.c
 *  @brief Codigo principal del programa radar
 *
 * Desde aca se realiza el calculo de los promedios,correlacion y posterior escritura
 * de los resultados en el archivo correlacion.iq
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#include "radarHelpers.h"

#define GATES 500 //cantidad de gates

int main(){

	clock_t start, end;
 	double cpu_time_used;
 	start = clock();

	FILE *read_ptr;
	FILE *write_ptr;
	char *path_pulsos = "../pulsos.iq";
	int cantidad_pulsos;
	uint16_t x;
	float lectura;
	read_ptr = fopen(path_pulsos,"rb");
	if (read_ptr == NULL){
		perror("File: ");
		exit(1);
	}
	cantidad_pulsos = get_cantidad_pulsos(read_ptr);
	printf("Cantidad de pulsos: %i\n",cantidad_pulsos);

	float H_real[GATES][cantidad_pulsos];
	float H_imaginario[GATES][cantidad_pulsos];
	float V_real[GATES][cantidad_pulsos];
	float V_imaginario[GATES][cantidad_pulsos];
	float H_correlacion[GATES];
	float V_correlacion[GATES];

	omp_set_num_threads(1);
	#pragma omp parallel sections
	{
	    #pragma omp section
	    { 
	        calcular_promedio(path_pulsos,GATES,cantidad_pulsos,V_real,0);
	    }

	    #pragma omp section
	    { 
	        calcular_promedio(path_pulsos,GATES,cantidad_pulsos,V_imaginario,1);
	    }
	    #pragma omp section
	    { 
	        calcular_promedio(path_pulsos,GATES,cantidad_pulsos,H_real,2);
	    }
	    #pragma omp section
	    { 
	        calcular_promedio(path_pulsos,GATES,cantidad_pulsos,H_imaginario,3);
	    }
	}

	omp_set_num_threads(1);
	#pragma omp parallel sections
	{

		#pragma omp section
		{
			calcular_correlacion(GATES,cantidad_pulsos,H_real,H_imaginario,H_correlacion);
		}
		#pragma omp section
		{
			calcular_correlacion(GATES,cantidad_pulsos,V_real,V_imaginario,V_correlacion);
		}
	}

	//Escribo los resultados en un binario
	write_ptr = fopen("../correlacion.iq","w+b");
	if (write_ptr == NULL){
		perror("File: ");
		exit(1);
	}
	escribir_correlacion(write_ptr,GATES,V_correlacion);
	escribir_correlacion(write_ptr,GATES,H_correlacion);


	fclose(read_ptr);
	fclose(write_ptr);

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("%f\n",cpu_time_used );
	return 0;
}