#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argcv[]){
    if(argc ==1) {
      FILE *fp = fopen("/proc/meminfo", r);

if (fp == NULL) {
            perror("Error al abrir el archivo");
            return 1;
        }

        char line[256];
        fgets(line, sizeof(line), fp);
        fgets(line, sizeof(line), fp);
        fgets(line, sizeof(line), fp); 
        fclose(fp);

        unsigned long long total, free, available;
        sscanf(line, "MemTotal: %llu kB\nMemFree: %llu kB\nMemAvailable: %llu kB", &total, &free, &available);

        float utilization = 100.0 * (total - available) / total;

        printf("Porcentaje de utilización de memoria virtual: %.2f%%\n", utilization);
    } else if (argc == 2 && strcmp(argv[1], "-r") == 0) {
        FILE *fp = fopen("/proc/meminfo", "r");
        if (fp == NULL) {
            perror("Error al abrir el archivo");
            return 1;
        }

        char line[256];
        fgets(line, sizeof(line), fp);
        fgets(line, sizeof(line), fp);
        fclose(fp);

        unsigned long long total, free;
        sscanf(line, "MemTotal: %llu kB\nMemFree: %llu kB", &total, &free);

        float utilization = 100.0 * (total - free) / total;

        printf("Porcentaje de utilización de memoria real: %.2f%%\n", utilization);
    } else {
        printf("Uso: %s [-r]\n", argv[0]);
        return 1;
    }

    return 0;
}
