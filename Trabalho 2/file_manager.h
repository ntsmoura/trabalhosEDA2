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
    void print_structure(); //print debug (for development and debug process only)


#endif
