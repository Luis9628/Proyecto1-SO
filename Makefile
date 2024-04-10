CC = gcc
CFLAGS = -Wall

all: cpu memoria disco estadistica

cpu: cpu.c
    $(CC) $(CFLAGS) -o cpu cpu.c

memoria: memoria.c
   $(CC) $(CFLAGS) -o memoria memoria.c

disco: disco.c
    $(CC) $(CFLAGS) -o disco disco.c

estadistica: estadistica.c
    $(CC) $(CFLAGS) -o estadistica estadistica.c

clean:
    rm -f cpu memoria disco estadistica
