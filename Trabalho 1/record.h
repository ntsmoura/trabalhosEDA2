/* 
Author: Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: record.h
*/

#include<stdbool.h>

#define MAXNAMESIZE 20
#define MAXNUMREGS 11

//Struct for user data
typedef struct {
	   int key;
	   char name[MAXNAMESIZE];
      unsigned int age;
} user_data;

//Struct for record
typedef struct {
   char status;
   user_data data;
} record;

