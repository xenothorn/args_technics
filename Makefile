all: sprint
	./sprint && echo "\n"

sprint: main 
	g++ -o sprint main.o

main: main.cpp
	g++ main.cpp --std=c++17 -Wall -ggdb -c

clean:
	rm sprint *.o