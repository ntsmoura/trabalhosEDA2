/* 
Author: Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: main.c
*/

#include<stdio.h>
#include<stdlib.h>
#include "file_manager.h"

int main(){

    char c = '@';
    int key, age;
    char name[MAXNAMESIZE];
    int* number_of_access = malloc(sizeof(int));
    int* last_position = malloc(sizeof(int));
    int* first_insert_position = malloc(sizeof(int));
    int number_of_success = 0, number_of_fails = 0, number_of_attempts_success = 0, number_of_attempts_fails = 0;
    record* r = malloc(sizeof(record));
    
    while(c!='e'){
        scanf("%c",&c);
        switch (c){
            case 'i':{
                scanf("%d",&key);
                scanf("%s",name);
                scanf("%d",&age);
                insert_data(key,name,age);
            }
            break;
            case 'c':{
                *number_of_access = 0;
                *last_position = 0;
                scanf("%d",&key);
                bool result = search_data(key,number_of_access,last_position,r);
                if(result){
                    printf("chave: %d\n",key);
                    printf("%s\n",r->data.name);
                    printf("%d\n",r->data.age);
                    number_of_success++;
                    number_of_attempts_success += (*number_of_access);
                } else{
                    printf("chave nao encontrada: %d\n",key);
                    number_of_fails++;
                    number_of_attempts_fails += (*number_of_access);
                }
            }
            break;
            case 'm':{
                if(number_of_success > 0) printf("%.1f\n",((float)number_of_attempts_success)/((float)number_of_success));
                else printf("%.1f\n",0.0);
                if(number_of_fails > 0) printf("%.1f\n",((float)number_of_attempts_fails)/((float)number_of_fails));
                else printf("%.1f\n",0.0);
            }
            break;
            case 'p':{
                print_data();
            }
            break;
            case 'r':{
                scanf("%d",&key);
                delete_data(key);
            }
            break;
            case 'e':{
                free(number_of_access);
                free(last_position);
                free(first_insert_position);
                free(r);
            }
            break;
        }
    }
    

}