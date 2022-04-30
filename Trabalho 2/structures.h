/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: structures.h
*/

#include<stdbool.h>

#define MAXNAMESIZE 21 //21 because the string contain 20 valid characters and 1 NULL carachter 
#define PRIME 101 //since bigger input key is 100 we defined  101 as our prime number

//Struct for user data
typedef struct {
	   int key;
	   char name[MAXNAMESIZE];
      unsigned int age;
} user_data;

//Struct for record
typedef struct {
   bool status;
   user_data data;
} record;

//struct for first level
typedef struct {
   int index;
   int info[3];
   bool second_level;
   int second_level_offset;
} first_level;

//Struct for second level
typedef struct {
   int index;
   int first_level_index;
   record r;
} second_level;

