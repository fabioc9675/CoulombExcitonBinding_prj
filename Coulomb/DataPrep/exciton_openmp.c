#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

//#define M_PI 3.14159265358979323846

#define COLS 200
int cols = 200;

// Constantes físicas
const double e = 1.60217662e-19;         // Carga elemental [C]
const double epsilon0 = 8.854187817e-12; // Permitividad del vacío [F/m]
const double epsilon_GaAs = 12.58*epsilon0;
const double conversion_factor = (e * e) / (4.0 * M_PI * epsilon_GaAs) * (1.0/e) * 1e3;

// // Parámetros de discretización (en metros)
// const double dro_e = 1e-9;
// const double dz_e = 0.1e-9;
// const double dro_h = 1e-9;
// const double dz_h = 0.1e-9;

int capacity = 45000;

typedef struct {
    double rho;  // Coordenada radial (en nm)
    double z;    // Coordenada axial (en nm)
    double psi[COLS]; // Funciones de onda para cada parámetro
} WaveFunction2D;

// Función para calcular la integral elíptica completa de primera especie (K(k))
double compute_ck(double XK) {
    double PK = 1.0 - XK;
    
    if(XK >= 1.0) return 1.0e300;
    
    // Coeficientes del desarrollo polinomial
    const double a0 = 0.01451196212;
    const double a1 = 0.03742563713;
    const double a2 = 0.03590092383;
    const double a3 = 0.09666344259;
    const double a4 = 1.38629436112;
    
    const double b0 = 0.00441787012;
    const double b1 = 0.03328355346;
    const double b2 = 0.06880248576;
    const double b3 = 0.12498593597;
    const double b4 = 0.5;
    
    // Cálculo usando el esquema de Horner
    double AK = ((((a0 * PK + a1) * PK + a2) * PK + a3) * PK + a4);
    double BK = ((((b0 * PK + b1) * PK + b2) * PK + b3) * PK + b4);
    
    return AK - BK * log(PK);
}

void read_wavefunctions(const char* filename, WaveFunction2D** data, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    *data = (WaveFunction2D*)malloc(capacity * sizeof(WaveFunction2D));
    *count = 0;
    
    while(fscanf(file, "%lf %lf", &(*data)[*count].rho, &(*data)[*count].z) == 2) {
        for(int k = 0; k < cols; k++) {
            fscanf(file, "%lf", &(*data)[*count].psi[k]);
        }
        (*count)++;
        if(*count >= capacity) {
            capacity *= 2;
            *data = (WaveFunction2D*)realloc(*data, capacity * sizeof(WaveFunction2D));
        }
    }
    
    fclose(file);
}

int main(int argc, char* argv[]) {

    // Valores por defecto
    const char* archivo_electron = "noI_noB\\xe_no_no_0_1.txt";
    const char* archivo_hueco    = "noI_noB\\xh_no_no_0_1.txt";
    const char* archivo_salida   = "noI_noB\\result_no_no_0_1.dat";
    double dro_e = 2e-9;
    double dz_e  = 0.1e-9;
    double dro_h = 2e-9;
    double dz_h  = 0.1e-9;
    double res = 0.1;
    double inc = 0.0;

    if (argc == 12) {
        archivo_electron = argv[1];
        archivo_hueco    = argv[2];
        archivo_salida   = argv[3];
        dro_e = atof(argv[4])*1e-9;
        dz_e  = atof(argv[5])*1e-9;
        dro_h = atof(argv[6])*1e-9;
        dz_h  = atof(argv[7])*1e-9;
        res  = atof(argv[8]);
        inc  = atof(argv[9]);
        cols = atof(argv[10]);
        capacity = atof(argv[11]);
    } else if (argc != 1) {
        printf("Uso: %s [archivo_electron archivo_hueco archivo_salida dro_e dz_e dro_h dz_h]\n", argv[0]);
        return 1;
    }

    printf("Archivo electron: %s\n", archivo_electron);
    printf("Archivo hueco:    %s\n", archivo_hueco);
    printf("Archivo salida:   %s\n", archivo_salida);
    printf("dro_e = %e, dz_e = %e\n", dro_e, dz_e);
    printf("dro_h = %e, dz_h = %e\n", dro_h, dz_h);

    double start_time = omp_get_wtime();
    
    // Leer funciones de onda para electrones y huecos
    WaveFunction2D *electrons = NULL, *holes = NULL;
    int num_electrons, num_holes;

    double op_start = omp_get_wtime();
    
    #pragma omp parallel sections
    {
        #pragma omp section
        read_wavefunctions(archivo_electron, &electrons, &num_electrons);
        
        #pragma omp section
        read_wavefunctions(archivo_hueco, &holes, &num_holes);
    }

    printf("Apertura de archivos: %.2f s\n", omp_get_wtime() - op_start);
    op_start = omp_get_wtime();
    
    // Paso 1: Normalización de las funciones de onda
    double norms_e[COLS] = {0}, norms_h[COLS] = {0};
    
    #pragma omp parallel for
    for(int k = 0; k < cols; k++) {
        // Normalización electrones
        for(int i = 0; i < num_electrons; i++) {
            double rho = electrons[i].rho * 1e-9; // Convertir nm a m
            double dV = 2 * M_PI * rho * dro_e * dz_e;
            norms_e[k] += pow(electrons[i].psi[k], 2) * dV;
        }
        
        // Normalización huecos
        for(int j = 0; j < num_holes; j++) {
            double rho = holes[j].rho * 1e-9; // Convertir nm a m
            double dV = 2 * M_PI * rho * dro_h * dz_h;
            norms_h[k] += pow(holes[j].psi[k], 2) * dV;
        }
    }

    printf("Normalizacion de datos: %.2f s\n", omp_get_wtime() - op_start);
    op_start = omp_get_wtime();
    
    // Paso 2: Cálculo de la integral de Coulomb
    double results[COLS] = {0};
    double param_times[COLS] = {0};
    
    #pragma omp parallel for schedule(dynamic)
    for(int k = 0; k < cols; k++) {
        double param_start = omp_get_wtime();
        double sqrt_norm_e = sqrt(norms_e[k]);
        double sqrt_norm_h = sqrt(norms_h[k]);
        double sum = 0.0;
        
        for(int i = 0; i < num_electrons; i++) {
            double rho_e = electrons[i].rho * 1e-9;
            double z_e = electrons[i].z * 1e-9;
            double psi_e = electrons[i].psi[k] / sqrt_norm_e;
            double dVe = rho_e * dro_e * dz_e; // Sin el 2π
            
            for(int j = 0; j < num_holes; j++) {
                double rho_h = holes[j].rho * 1e-9;
                double z_h = holes[j].z * 1e-9;
                double psi_h = holes[j].psi[k] / sqrt_norm_h;
                double dVh = rho_h * dro_h * dz_h; // Sin el 2π
                
                // Cálculo de parámetros geométricos
                double delta_rho = rho_e - rho_h;
                double delta_z = z_e - z_h;
                double r_sq = delta_rho*delta_rho + delta_z*delta_z;
                if(r_sq < 1e-30) continue; // Evitar división por cero
                
                double r = sqrt(r_sq);
                double rp = (4.0 * rho_e * rho_h) / r_sq;
                double XK = rp / (1.0 + rp);
                
                // Calcular integral elíptica
                double CK = compute_ck(XK);
                
                // Término angular
                double angular = (8.0 * M_PI / r) * CK / sqrt(1.0 + rp);
                
                sum += (psi_e * psi_e) * (psi_h * psi_h) * angular * dVe * dVh;
            }
        }
        
        results[k] = sum * conversion_factor;

        param_times[k] = omp_get_wtime() - param_start;

        #pragma omp critical
        {
            printf("[P] Parametro %02d (xb=%.1f): %.3e meV [T: %.2f s]\n", 
                  k+1, (k)*res + inc, results[k], param_times[k]);
        }
    }
    
    // Paso 3: Escritura de resultados
    FILE* output = fopen(archivo_salida, "w");
    //fprintf(output, "# R_2(nm)\tEnergía(meV)\n");
    for(int k = 0; k < cols; k++) {
        fprintf(output, "%.1f\t%.6e\n", (k)*res + inc, results[k]);
    }
    fclose(output);
    
    // Liberar memoria
    free(electrons);
    free(holes);
    
    printf("Tiempo total de ejecución: %.2f segundos\n", omp_get_wtime() - start_time);
    return 0;
}