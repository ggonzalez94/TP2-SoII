#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "radarHelpers.h"

#define GATES 500 //cantidad de gates

int main(){
	FILE *ptr;
	int cantidad_pulsos;
	uint16_t x;
	float lectura;
	ptr = fopen("../pulsos.iq","rb");
	cantidad_pulsos = get_cantidad_pulsos(ptr);
	printf("Cantidad de pulsos: %i\n",cantidad_pulsos);

	float H_real[GATES][cantidad_pulsos];
	float H_imaginario[GATES][cantidad_pulsos];
	float V_real[GATES][cantidad_pulsos];
	float V_imaginario[GATES][cantidad_pulsos];
	
	calcular_promedio(ptr,GATES,cantidad_pulsos,V_real,0);
	calcular_promedio(ptr,GATES,cantidad_pulsos,V_imaginario,1);
	calcular_promedio(ptr,GATES,cantidad_pulsos,H_real,2);
	calcular_promedio(ptr,GATES,cantidad_pulsos,H_imaginario,3);
	//printf("%f\n",H_real[2][2]);
	// fread(&x,sizeof(uint16_t),1,ptr);
	// float H_real[x][x];
	// printf("Cantidad de datos: %i\n",x );
	// for (int i=0;i<100;i++){
	// 	fread(&lectura,sizeof(float),1,ptr);
	// 	printf("Valor %i: %f\n",i,lectura );
	// }

	// fread(&x,2,1,ptr);
	// printf("Cantidad de datos: %i\n",x );
	// for (int i=0;i<100;i++){
	// 	fread(&lectura,sizeof(float),1,ptr);
	// 	printf("Valor %i: %f\n",i,lectura );
	// }

	//fclose(ptr);

	return 0;
}