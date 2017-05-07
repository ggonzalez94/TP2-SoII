#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

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

void calcular_promedio(char *path,int filas,int columnas,float matriz_a_llenar[filas][columnas],int offset){
	FILE *ptr;
	uint16_t cantidad_muestras;
	int muestras_por_gate;
	long int bytes_a_saltar;
	ptr = fopen(path,"rb");
	if (ptr == NULL){
		perror("File: ");
		exit(1);
	}
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
	fclose(ptr);
}
/**
* @brief Calcula el valor absoluto de un numero complejo
*
*/
float valor_absoluto(float real,float imaginario){
	float resultado;
	resultado = sqrt(pow(real,2) + pow(imaginario,2));
	return resultado;
}

/**
* @brief Calcula la correlacion de cada gate y llena un vector de correlacion
*
*/
void calcular_correlacion(int filas,int columnas, float matriz_real[filas][columnas],float matriz_imaginaria[filas][columnas],float vector_correlacion[filas]){
	float correlacion = 0;
	for (int i = 0; i < filas; ++i)
	{
		for (int j = 0; j < columnas-1; ++j)
		{
			correlacion += valor_absoluto(matriz_real[i][j],matriz_imaginaria[i][j]) * valor_absoluto(matriz_real[i][j+1],matriz_imaginaria[i][j+1]);
		}
		correlacion = correlacion/columnas; //Divido entre la cantidad de pulsos
		vector_correlacion[i] = correlacion;
		correlacion = 0;
	}

}
/**
* @brief Adjunta al final del archivo ptr la cantidad de datos(int) y un vector de correlacion
*
*/
void escribir_correlacion(FILE *ptr,int longitud,float vector_correlacion[longitud]){
	int cantidad_datos = longitud;
	//Me voy al final del archivo para adjuntar los datos
	fseek(ptr,0,SEEK_END);
	//Escribo la cantidad de datos por cada vector de correlacion(que siempre es igual a la cantidad de gates)
	fwrite(&cantidad_datos,sizeof(int),1,ptr);
	//Escribo los datos de correlacion
	fwrite(vector_correlacion,sizeof(float),longitud,ptr);
	//Retorno el puntero al inicio del archivo
	fseek(ptr,0,SEEK_SET);
}


