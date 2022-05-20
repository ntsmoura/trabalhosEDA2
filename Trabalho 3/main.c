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
    int n;
    FILE *f;
    //char name[MAXNAMESIZE];
    //char title[MAXNAMESIZE];
    //char file[MAXNAMESIZE];
    //unsigned int year;

    while(c!='e'){
        if(!(f = fopen(MAIN_FILE,"rb"))){
            create_file();
            scanf("%d",&n);
            getchar();
            record *records = calloc(n,sizeof(record));
            for(int i = 0; i<n;i++){
                record r;
                r.status = true;
                scanf("%20[^\n]", r.data.name);
                getchar();
                scanf("%20[^\n]", r.data.title);
                getchar();
                scanf("%u",&r.data.year);
                getchar();
                scanf("%20[^\n]", r.data.file);
                getchar();
                *(records+i) = r;
            }
            fclose(f);
            create_root(*records);
            for(int i = 1; i<n;i++) insert_node(*(records+i));
            print_nodes(n);
        }
        else{
            scanf("%c",&c);
            switch (c){
                case 'i':{
                }
                break;
                case 'p':{
                }
                break;
                case 'e':{
                }
                break;
            }
        }
    }
    

}