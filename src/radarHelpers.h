int get_cantidad_pulsos(FILE *ptr);
void calcular_promedio(FILE *ptr,int filas,int columnas,float matriz_a_llenar[filas][columnas],int offset);
void calcular_promedio_pulso(FILE *ptr,int filas,int columnas,float matriz_a_llenar[filas][columnas],int columna_a_llenar,int muestras_por_gate);