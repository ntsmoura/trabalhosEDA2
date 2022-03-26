/* 
Author: Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
*/

#include<stdbool.h>

#define NUMBEROFRECORDS 11

//Struct for user data
typedef struct {
	   unsigned int key;
	   char* name;
      unsigned int age;
} user_data;

//Struct for record
typedef struct {
   char status;
   user_data data;
} record;

