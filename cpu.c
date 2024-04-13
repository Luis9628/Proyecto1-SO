#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        FILE *fp = fopen("/proc/stat", "r");
        if (fp == NULL) {
            perror("Error al abrir el archivo");
            return 1;
        }

        char line[256];
        fgets(line, sizeof(line), fp);
        fclose(fp);

        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
        sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

        unsigned long long total_time = user + nice + system + idle + iowait + irq + softirq + steal;
        unsigned long long idle_time = idle + iowait;
        float utilization = 100.0 * (total_time - idle_time) / total_time;

        printf("Porcentaje de utilización total de la CPU: %.2f%%\n", utilization);
    } else if (argc == 2) {
        int pid = atoi(argv[1]);
        char filename[256];
        snprintf(filename, sizeof(filename), "/proc/%d/stat", pid);

        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            perror("Error al abrir el archivo");
            return 1;
        }

        unsigned long long utime, stime;
        fscanf(fp, "%*d %*s %*s %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %llu %llu", &utime, &stime);
        fclose(fp);

        unsigned long long total_time = utime + stime;
        unsigned long long elapsed_time = 300; 
        float utilization = 100.0 * total_time / (elapsed_time * sysconf(_SC_CLK_TCK));

        printf("Porcentaje de utilización para el proceso %d: %.2f%%\n", pid, utilization);
    } else {
        printf("Uso: %s [PID]\n", argv[0]);
        return 1;
    }

    return 0;
}
