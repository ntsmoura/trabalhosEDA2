/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: file_manager.h
*/

#ifndef _H_FILE
#define _H_FILE

    #include <stdbool.h>
    #include "record.h"

    void create_file(int a, int b, int m); //Create records file
    int random_number(int min, int max); //Generate random number between [min,max]
    void print_structure(); //Print file content(for development and debug process only)
    void create_temporary_file();  //Create temporary file to store data
    void insert_temporary_file(int key, char name[MAXNAMESIZE], int age); //Insert data in temporary file
    void print_temporary_file(int m); //Print temporary file content (for development and debug process only)
    void delete_temporary_files(); //Delete the temporary file
    void calculate_elements_first_level(int a,int b,int p,int m); //Calculates the number of elements per index and store in the temporary file
    record* first_level_elements(first_level f1, int a, int b, int p, int m, int* f1_size); //Find first level elements of each index
    void create_second_level(int a, int b, int m, int p); //Create second level structure
    void consult_file(int key);
    void print_first_level();
    void print_second_level(int index);
    void print_full_second_level();

#endif
