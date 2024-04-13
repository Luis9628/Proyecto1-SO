#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void print_process_info(int pid) {
    char filename[128];
    FILE *fp;

    sprintf(filename, "/proc/%d/stat", pid);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char comm[256];
    unsigned long int rss;
    fscanf(fp, "%*d %s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*d %lu", comm, &rss);

    fclose(fp);

    printf("PID: %d, Proceso: %s, RSS: %lu kB\n", pid, comm, rss);
}

int main() {
    FILE *fp = fopen("/proc/meminfo", "r");

    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    char line[256];
    unsigned long long total, free, available;
    fgets(line, sizeof(line), fp);
    sscanf(line, "MemTotal: %llu kB\n", &total);
    fgets(line, sizeof(line), fp);
    sscanf(line, "MemFree: %llu kB\n", &free);
    fgets(line, sizeof(line), fp);
    sscanf(line, "MemAvailable: %llu kB\n", &available);
    fclose(fp);

    float utilization = 100.0 * (total - available) / total;

    printf("Porcentaje de utilización de memoria real: %.2f%%\n", utilization);

    DIR *dir;
    struct dirent *entry;
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error al abrir el directorio /proc");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        int pid = atoi(entry->d_name);
        if (pid > 0) {
            print_process_info(pid);
        }
    }

    closedir(dir);

    return 0;
}
