int get_cantidad_pulsos(FILE *ptr);
void calcular_promedio(char *path,int filas,int columnas,float matriz_a_llenar[filas][columnas],int offset);
void calcular_promedio_pulso(FILE *ptr,int filas,int columnas,float matriz_a_llenar[filas][columnas],int columna_a_llenar,int muestras_por_gate);
float valor_absoluto(float real,float imaginario);
void calcular_correlacion(int filas,int columnas, float matriz_real[filas][columnas],float matriz_imaginaria[filas][columnas],float vector_correlacion[filas]);
void escribir_correlacion(FILE *ptr,int longitud,float vector_correlacion[longitud]);