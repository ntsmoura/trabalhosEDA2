/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: file_manager.h
*/

#ifndef _H_FILE
#define _H_FILE

    #include <stdbool.h>
    #include "structures.h"

    void create_file(); //Creates the main file
    void create_root(record r); //Creates the root of the tree
    void print_nodes(int number); //for debug purposes only
    void insert_node(record r); //insert nodes in the k-d tree

#endif
