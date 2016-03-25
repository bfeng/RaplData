all: rapl-data

rapl-data: RaplData.o main.o
	g++ RaplData.o main.o -lm -o rapl-data

main.o: main.cpp
	g++ -Wall -c main.cpp

rapl-data.o: RaplData.cpp RaplData.h
	g++ -Wall -c RaplData.cpp

clean:
	rm -f *.o rapl-data
