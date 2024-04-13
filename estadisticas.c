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

  pid_t pid = fork();
  if (pid == -1) {
    perror("Error al crear el subproceso");
    return 1;
  } else if (pid == 0) {
    if (strcmp(recurso, "cpu") == 0) {
      if (argc == 2) {
        execlp("./cpu", "./cpu", NULL);
      } else if (argc == 3) {
        execlp("./cpu", "./cpu", argv[2], NULL);
      }
    } else if (strcmp(recurso, "memoria") == 0) {
      if (argc == 2) {
        execlp("./memoria", "./memoria", NULL);
      } else if (argc == 3) {
        if (strcmp(argv[2], "-r") == 0) {
          execlp("./memoria", "./memoria", "-r", NULL);
        } else if (strcmp(argv[2], "-v") == 0 && argc == 4) {
          execlp("./memoria", "./memoria", "-v", argv[3], NULL);
        }
      }
    } else if (strcmp(recurso, "disco") == 0) {
      if (argc == 2) {
        execlp("./disco", "./disco", NULL);
      } else if (argc == 3) {
        if (strcmp(argv[2], "-tm") == 0) {
          execlp("./disco", "./disco", "-tm", NULL);
        } else if (strcmp(argv[2], "-tu") == 0) {
          execlp("./disco", "./disco", "-tu", NULL);
        }
      }
    } else {
      printf("Error: Recurso no válido\n");
      exit(1);
    }
  } else {
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
