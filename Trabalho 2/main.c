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
    int key, age, index;
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
                //Set seed of rand to current time
                srand(time(NULL)); 
                int a = random_number(1,PRIME-1);
                int b = random_number(0,PRIME-1);
                //int a = 3;
                //int b = 42;
                calculate_elements_first_level(a,b,PRIME,m);
                create_file(a,b,PRIME,m);
                create_second_level(a,b,m,PRIME);
                delete_temporary_files();
                printf("estrutura de hashing perfeito criada\n");
            }
            break;
            case 'c':{
                scanf("%d",&key);
                consult_file(key);
            }
            break;
            case 'p':{
                print_first_level();
            }
            break;
            case 's':{
                scanf("%d",&index); //sempre válido
                print_second_level(index);
            }
            break;
            case 'h':{
                print_first_level();
                print_full_second_level();
            }
            break;
            case 'n':{
                number_of_functions();
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