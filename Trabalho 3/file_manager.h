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

    void create_file(); //Create the main file
    void create_root(record r); //Create the root of the tree
    void print_nodes(); //for debug purposes only
    void print_linked_pages();  //for debug purposes only
    void insert_node(record r); //insert nodes in the k-d tree
    void insert_empty_pages(); //insert empty pages in the k-d tree
    void insert_into_page(record r, int page_node_position);//insert record into a not full page
    

#endif
