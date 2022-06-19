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
    void insert_record(record r);//insert record into the correct page
    void print_indexes(int position, node* indexes, int indexes_size); //print k-d tree indexes
    node* load_indexes_vector(int* indexes_size); //load k-d tree indexes into a vector
    int find_page(int position, int* count, int page_index, node* indexes, int indexes_size, bool* found, int* page_position); //Find page node position
    void print_page(int page_node_index); //Print the specified page and all the linked ones
    void simple_search_name(char name[MAXNAMESIZE]); //Try to find all the records with the author's name
    void find_author_simple(FILE* f, node* actual, bool* found, char name[MAXNAMESIZE]); //Find the page of the author to print their record
    void range_author_search(char name1[MAXNAMESIZE], char name2[MAXNAMESIZE]);//Try to find all the records in the range of the authors' names
    void find_authors_in_range(FILE* f, node* actual, char name1[MAXNAMESIZE], char name2[MAXNAMESIZE]); //Find the pages in range of the authors' names and print the records
    void range_year_search(unsigned int ano1, unsigned int ano2); //Try to find all the records in range of the years of release
    void find_years_in_range(FILE* f, node* actual, unsigned int ano1, unsigned int ano2); //Find the pages in range of the years of release and print the records
    void range_author_year_search(char name1[MAXNAMESIZE], char name2[MAXNAMESIZE], unsigned int ano1, unsigned int ano2); //Try to find all the records in range of the authors' names and the years of release
    void find_authors_years_in_range(FILE* f, node* actual, char name1[MAXNAMESIZE], char name2[MAXNAMESIZE], unsigned int ano1, unsigned int ano2); //Find the pages in range of the authors' names and the years of release and print the records
    int* create_pi_table(char word[MAXWORDSIZE]); //Create pi table
    //void print_line_and_positions(char file_name[MAXNAMESIZE],char word[MAXWORDSIZE]); 
    int find_word_inside_file(int flag, char file_name[MAXNAMESIZE],char word[MAXWORDSIZE]);//Find word inside a text file
    

#endif
