/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: main.c
*/

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include "file_manager.h"

int main(){

    char c = '@'; 
    int m;
    //FILE *f;
    int key, age;
    char name[MAXNAMESIZE];

    while(c!='e'){
        scanf("%c",&c);
        switch (c){
            case 'i':{
                scanf("%d",&m);
                create_temporary_file();
                for (int i = 0; i<m; i++){
                    scanf("%d",&key);
                    getchar();
                    scanf("%20[^\n]", name);
                    scanf("%d",&age);
                    insert_temporary_file(key,name,age);
                }
               srand(time(NULL)); //Set seed of rand to current time
                int a = random_number(1,PRIME-1);
                srand(time(0)); //Set seed of rand to the time passed since 1970 0h utc
                int b = random_number(0,PRIME-1);
                //int a = 3;
                //int b = 42;
                srand(0); //Set seed of rand to 0
                calculate_elements_first_level(a,b,PRIME,m);
                create_file(a,b,m);
                create_second_level(a,b,m,PRIME);
                delete_temporary_file();
            }
            break;
            case 'p':{
            }
            break;
            case 'e':{
            }
            break;
            case '*':{
               //print_temporary_file(m);
               //printf("\n");
               print_structure();
               //printf("\n");
            }
            break;
        }
    }
    

}