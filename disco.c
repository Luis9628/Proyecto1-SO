#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTES_TO_MB(bytes) ((bytes) / (1024 * 1024)) // Macro para convertir bytes a megabytes

int main(int argc, char *argv[]) {
    if (argc == 1) {
        FILE *fp = fopen("/proc/mounts", "r");
        if (fp == NULL) {
            perror("Error al abrir el archivo");
            return 1;
        }

        char line[256];
        while (fgets(line, sizeof(line), fp) != NULL) {
            char device[256], mountpoint[256], fs[256];
            sscanf(line, "%s %s %s", device, mountpoint, fs);
            if (strcmp(mountpoint, "/") == 0) {
                break;
            }
        }
        fclose(fp);

        FILE *dfp = popen("df / | tail -n 1", "r");
        if (dfp == NULL) {
            perror("Error al abrir el comando");
            return 1;
        }

        unsigned long long total_size = 0, used_space = 0, available_space = 0;
        char output[512];
        while (fgets(output, sizeof(output), dfp) != NULL) {
            // Buscar los valores en la línea de salida utilizando expresiones regulares
            if (sscanf(output, "%*s %*s %*s %llu %llu %llu", &total_size, &used_space, &available_space) == 3) {
                break;
            }
        }
        pclose(dfp);

        float utilization = ((float)used_space / total_size) * 100;

        printf("Espacio utilizado en el disco: %lld MB\n", BYTES_TO_MB(used_space));
        printf("Espacio disponible en el disco: %lld MB\n", BYTES_TO_MB(available_space));
        printf("Porcentaje total de espacio utilizado en disco: %.2f%%\n", utilization);
    } else if (argc == 2 && (strcmp(argv[1], "-tm") == 0 || strcmp(argv[1], "-tu") == 0)) {
        FILE *dfp = popen("df / | grep '^/' | awk '{print $5}'", "r");
        if (dfp == NULL) {
            perror("Error al abrir el comando");
            return 1;
        }

        char output[20];
        fgets(output, sizeof(output), dfp);
        pclose(dfp);

        printf("Porcentaje total de espacio libre en disco: %s", output);
    } else {
        printf("Uso: %s [-tm | -tu]\n", argv[0]);
        return 1;
    }

    return 0;
}

