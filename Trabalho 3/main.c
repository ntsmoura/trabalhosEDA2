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
            insert_empty_pages();
            for(int i = 1; i<n;i++) insert_record(*(records+i));
            print_nodes();
            print_linked_pages();
        }
        else{
            scanf("%c",&c);
            switch (c){
                case 'i':{
                    record r;
                    r.status = true;
                    getchar();
                    scanf("%20[^\n]", r.data.name);
                    getchar();
                    scanf("%20[^\n]", r.data.title);
                    getchar();
                    scanf("%u",&r.data.year);
                    getchar();
                    scanf("%20[^\n]", r.data.file);
                    getchar();
                    insert_record(r);
                    print_nodes();
                    print_linked_pages();
                }
                break;
                case 'p':{
                }
                break;
                case 'e':{
                }
                break;
                case 't':{
                    int* indexes_size = malloc(sizeof(int));
                    node* indexes = load_indexes_vector(indexes_size);
                    print_indexes(0,indexes,*indexes_size);
                    free(indexes);
                }
                break;
            }
        }
    }
    

}