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
    int random(int min, int max); //Generate random number between [min,max]
    void print_structure(); //print file content(for development and debug process only)
    void create_temporary_file();  //Create temporary file to store data
    void insert_temporary_file(int key, char name[MAXNAMESIZE], int age); //Insert data in temporary file
    void print_temporary_file(int m); //print temporary file content (for development and debug process only)
    void calculate_elements_first_level(int a,int b,int p,int m); //Calculates the number of elements per index and store in the temporary file


#endif
