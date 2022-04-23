/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: main.c
*/

#include<stdio.h>
#include<stdlib.h>
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
                int a = random(1,PRIME-1);
                int b = random(0,PRIME-1);
                calculate_elements_first_level(a,b,PRIME,m);
                create_file(a,b,m);
            }
            break;
            case 'p':{
            }
            break;
            case 'e':{
            }
            break;
            case '*':{
               print_structure();
               printf("\n");
               print_temporary_file(m);
            }
            break;
        }
    }
    

}