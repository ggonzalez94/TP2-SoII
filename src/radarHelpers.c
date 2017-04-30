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

/**
* @brief Llena el promedio de todos los gates para 1 pulso(llena una columna)
*/
void calcular_promedio_pulso(FILE *ptr,int filas,int columnas,float matriz_a_llenar[filas][columnas],int columna_a_llenar,int muestras_por_gate){
	float promedio = 0;
	float lectura;

	for (int i = 0; i < filas; ++i)
	{
		//Calculo para un gate
		for (int j = 0; j < muestras_por_gate; ++j)
		{
			fread(&lectura,sizeof(float),1,ptr);
			promedio += lectura;
			//Me desplazo al siguiente valor de la variable
			fseek(ptr,3*sizeof(float),SEEK_CUR);
		}
		promedio = promedio/muestras_por_gate;
		matriz_a_llenar[i][columna_a_llenar] = promedio;
	}

}
/**
* @brief Calcula el promedio de una matriz para todos los pulsos y gates
*
*/

void calcular_promedio(FILE *ptr,int filas,int columnas,float matriz_a_llenar[filas][columnas],int offset){
	uint16_t cantidad_muestras;
	int muestras_por_gate;
	long int bytes_a_saltar;
	fseek(ptr,0,SEEK_SET);
	//Lleno 1 pulso a la vez
	for(int columna_actual=0;columna_actual<columnas;columna_actual++){
		fpos_t posicion_inicial;
		fgetpos(ptr,&posicion_inicial);
		fread(&cantidad_muestras,sizeof(uint16_t),1,ptr);
		//Debo encontrar una mejor forma de hacerlo, aca pierdo muchos valores
		muestras_por_gate = cantidad_muestras/filas;
		//Anado un offset dependiendo de que matriz se trata
		fseek(ptr,sizeof(float)*offset,SEEK_CUR);
		calcular_promedio_pulso(ptr,filas,columnas,matriz_a_llenar,columna_actual,muestras_por_gate);
		//Pongo el puntero para leer el sgte pulso
		bytes_a_saltar = cantidad_muestras*sizeof(float)*4+2;
		fsetpos(ptr,&posicion_inicial);
		fseek(ptr,bytes_a_saltar,SEEK_CUR);
	}
}


