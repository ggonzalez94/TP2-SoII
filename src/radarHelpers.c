#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int get_cantidad_pulsos(FILE *ptr){
	int cantidad_pulsos = 0;
	int tamano_archivo;
	uint16_t validSamples;
	long int bytes_a_saltar;
	//Calculo cuantos bytes tiene el archivo
	fseek(ptr,0,SEEK_END);
	tamano_archivo = ftell(ptr);
	//Vuelvo al inicio del archivo
	fseek(ptr,0,SEEK_SET);
	while(true){
		fread(&validSamples,sizeof(uint16_t),1,ptr); //Lee la cantidad de datos eso debo multiplicarlo X4 por H y V(real e im) y X4 por un float tener 4 bytes
		bytes_a_saltar = validSamples*sizeof(float)*4;
		if (ftell(ptr) + bytes_a_saltar >= tamano_archivo){
			break;
		}
		cantidad_pulsos++;
		fseek(ptr,bytes_a_saltar,SEEK_CUR);
		//printf("%ld\n", ftell(ptr));
	}
	fseek(ptr,0,SEEK_SET); //Vuelvo el puntero al inicio del archivo

	return cantidad_pulsos;
}