CC = gcc
CFLAGS = -Wall

all: cpu memoria disco estadisticas

cpu: cpu.c
	$(CC) $(CFLAGS) -o cpu cpu.c

memoria: memoria.c
	$(CC) $(CFLAGS) -o memoria memoria.c

disco: disco.c
	$(CC) $(CFLAGS) -o disco disco.c

estadisticas: estadisticas.c
	$(CC) $(CFLAGS) -o estadisticas estadisticas.c

clean:
	rm -f cpu memoria disco estadisticas
