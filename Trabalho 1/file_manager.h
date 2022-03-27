/* 
Author: Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: file_manager.h
*/

#ifndef _H_FILE
#define _H_FILE

    #include <stdbool.h>
    #include "record.h"

    void create_file(); //Create records file
    bool search_data(int key, int* number_of_access, int* last_position, record* r); //Search for key in a file using double hashing
    void insert_data(int key, char name[], int age); //Insert data in a file
    void print_data(); //Print all data from a file
    void delete_data(int key); //Delete record in a file if find given jey

#endif
