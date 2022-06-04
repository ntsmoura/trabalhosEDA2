/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: record.h
*/

#include<stdbool.h>

#define MAXNAMESIZE 21 //21 because the string contain 20 valid characters and 1 NULL carachter 
#define NREGSPORPAGINA 3 //number of records per page
#define MAIN_FILE  "records.dat" //name of the main file

//Struct for user data
typedef struct {
	   char name[MAXNAMESIZE];
      char title[MAXNAMESIZE];
      char file[MAXNAMESIZE];
      unsigned int year;
} user_data;

//Struct for record
typedef struct {
   bool status;
   user_data data;
} record;

//Struct for page
typedef struct {
   record records[NREGSPORPAGINA];
   int qty;
   int linked_page;
} page;


//struct for node
typedef struct {
   int level;
   bool is_page;
   int left_son;
   int right_son;
   union{
      unsigned int year;
      char name[MAXNAMESIZE];
      page p;
   };
} node;