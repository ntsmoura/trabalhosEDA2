############################# Makefile ##########################
all: file_manager
file_manager: file_manager.o main.o
		gcc -o file_manager file_manager.o main.o
file_manager.o: file_manager.c
		gcc -o file_manager.o -c file_manager.c -Wall -Wextra -Werror -Wpedantic
main.o: main.c file_manager.h
		gcc -o main.o -c main.c -Wall -Wextra -Werror -Wpedantic
clean:
		rm -rf *.o
mrproper: clean
