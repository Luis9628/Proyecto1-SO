#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        FILE *fp = fopen("/proc/mounts", "r");
        if (fp == NULL) {
            perror("Error al abrir el archivo");
            return 1;
        }

        char line[256];
        fgets(line, sizeof(line), fp);
        fclose(fp);

        char device[256], mountpoint[256], fs[256];
        sscanf(line, "%s %s %s", device, mountpoint, fs);

        FILE *dfp = popen("df / | tail -n 1", "r");
        if (dfp == NULL) {
            perror("Error al abrir el comando");
            return 1;
        }

        unsigned long long total_size;
        fscanf(dfp, "%*s %*lu %*lu %*lu %*lu %llu", &total_size);
        pclose(dfp);

        unsigned long long free_space;
        sscanf(line, "%*s %*s %*s %*s %llu %*s", &free_space);

        float utilization = 100.0 * free_space / total_size;

        printf("Porcentaje total de espacio libre en disco: %.2f%%\n", utilization);
    } else if (argc == 2 && (strcmp(argv[1], "-tm") == 0 || strcmp(argv[1], "-tu") == 0)) {
        FILE *fp = popen("df -BM | grep '^/dev/' | awk '{print $4}'", "r");
        if (fp == NULL) {
            perror("Error al abrir el comando");
            return 1;
        }

        char output[20];
        fgets(output, sizeof(output), fp);
        pclose(fp);

        printf("Porcentaje total de espacio libre en disco: %s", output);
    } else {
        printf("Uso: %s [-tm | -tu]\n", argv[0]);
        return 1;
    }

    return 0;
}
