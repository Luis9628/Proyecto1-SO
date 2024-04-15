#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define PAGE_SIZE 4096 // Tamaño de página en bytes

unsigned long long total;
void get_real_memory_usage(int pid) {
    FILE *fp;
    char path[100];
    unsigned long size, resident, share;

    sprintf(path, "/proc/%d/statm", pid);
    
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%lu %lu %lu", &size, &resident, &share);
    fclose(fp);
    
    unsigned long mem_usage = (resident - share) * PAGE_SIZE / 1024;
    float percMem = ((float)mem_usage / total) * 100;
    
    //----------------------------------------------------------------------------------------------------------------------------------------
  
    sprintf(path, "/proc/%d/stat", pid);
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char comm[256];
    fscanf(fp, "%*d %s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*d", comm);

    fclose(fp);
    
    printf("PID: %d, Proceso: %s, MEM:%.2f%%\n", pid, comm, percMem);
}

void get_virtual_memory_usage(int pid) {
    FILE *fp;
    char comm[256];
    unsigned long long vsize;
    char path[100];
    
    sprintf(path, "/proc/%d/stat", pid);
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    fscanf(fp, "%*d %s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*d %llu", comm, &vsize);
    fclose(fp);
    
    printf("PID: %d, Proceso: %s, VMEM:%.2f%%\n", pid, comm, vsize);
    
}

int main(int argc, char *argv[]) {
    FILE *fp = fopen("/proc/meminfo", "r");
    char line[256];
    int determina = 0;

    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }
    
    if(argc == 1){
        unsigned long long swapTo, swapFre;
        
        while (fgets(line, 256, fp)) {
            if (strstr(line, "SwapTotal")) {
                sscanf(line, "%*s %llu", &swapTo);
            }
            if (strstr(line, "SwapFree")) {
                sscanf(line, "%*s %llu", &swapFre);
            }
        }
        
        //printf("El Swap Total es: %llu\n", swapTo);
        //printf("El Swap Free es: %llu\n", swapFre); 

        double swapUsed = (double)swapTo - (double)swapFre; 
        float percentage = ((double)swapUsed / (double)swapTo) * 100;
        printf("Porcentaje de utilización de memoria virtual: %.2f%%\n", percentage);
        determina = 1;
    
    }else if (argc == 2 && (strcmp(argv[1], "-r") == 0)){
        unsigned long long free, available;
        fgets(line, sizeof(line), fp);
        sscanf(line, "MemTotal: %llu kB\n", &total);
        fgets(line, sizeof(line), fp);
        sscanf(line, "MemFree: %llu kB\n", &free);
        fgets(line, sizeof(line), fp);
        sscanf(line, "MemAvailable: %llu kB\n", &available);
        fclose(fp);

        float utilization = 100.0 * (total - available) / total;

        printf("Porcentaje de utilización de memoria real: %.2f%%\n", utilization);
        determina = 2;

    }

    DIR *dir;
    struct dirent *entry;
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error al abrir el directorio /proc");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        int pid = atoi(entry->d_name);
        if ((pid > 0) && (determina == 2)) {
            get_real_memory_usage(pid);
        }else{
            if((pid > 0) && (determina == 1)){
                get_virtual_memory_usage(pid);
            }
        }
    }

    closedir(dir);
    
    return 0;
}
