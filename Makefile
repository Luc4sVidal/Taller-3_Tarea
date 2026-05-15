CC=g++
CFLAGS=-Wall -std=c++17 -O3
SOL1=solucion1
SOL2=solucion2
all: clean Soluciones

Soluciones:
	$(CC) $(CFLAGS) -o solucion1 solucion1.cpp
	$(CC) $(CFLAGS) -o solucion2 solucion2.cpp

clean:
	@echo " [CLN] Removing soluciones files"
	@rm -f $(SOL1) $(SOL2)