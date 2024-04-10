#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Uso: %s <recurso> [argumentos adicionales]\n", argv[0]);
    return 1;
  }

  char *recurso = argv[1];

  // Crear un subproceso para obtener la información del recurso especificado.
  pid_t pid = fork();
  if (pid == -1) {
    perror("Error al crear el subproceso");
    return 1;
  } else if (pid == 0) {
    // Código del subproceso
    if (strcmp(recurso, "cpu") == 0) {
      if (argc == 2) {
        // Obtener la utilización total de la CPU
        execlp("./cpu", "./cpu", NULL);
      } else if (argc == 3) {
        // Obtener la utilización de la CPU para un proceso específico
        execlp("./cpu", "./cpu", argv[2], NULL);
      }
    } else if (strcmp(recurso, "memoria") == 0) {
      if (argc == 2) {
        // Obtener la utilización total de memoria
        execlp("./memoria", "./memoria", NULL);
      } else if (argc == 3) {
        if (strcmp(argv[2], "-r") == 0) {
          // Obtener la utilización de memoria real
          execlp("./memoria", "./memoria", "-r", NULL);
        } else if (strcmp(argv[2], "-v") == 0 && argc == 4) {
          // Obtener la utilización de memoria virtual para un proceso específico
          execlp("./memoria", "./memoria", "-v", argv[3], NULL);
        }
      }
    } else if (strcmp(recurso, "disco") == 0) {
      if (argc == 2) {
        // Obtener el espacio libre en disco
        execlp("./disco", "./disco", NULL);
      } else if (argc == 3) {
        if (strcmp(argv[2], "-tm") == 0) {
          // Obtener el espacio libre en MiB
          execlp("./disco", "./disco", "-tm", NULL);
        } else if (strcmp(argv[2], "-tu") == 0) {
          // Obtener el espacio libre en GiB
          execlp("./disco", "./disco", "-tu", NULL);
        }
      }
    } else {
      printf("Error: Recurso no válido\n");
      exit(1);
    }
  } else {
    // Código del proceso padre
    int status;
    wait(&status);

    if (WIFEXITED(status)) {
      if (WEXITSTATUS(status) != 0) {
        printf("Error en el subproceso\n");
        return 1;
      }
    } else {
      printf("Error en el subproceso\n");
      return 1;
    }
  }

  return 0;
}
