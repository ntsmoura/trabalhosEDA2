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
            create_root(*records);
            for(int i = 1; i<n;i++) insert_node(*(records+i));
            insert_empty_pages();
            for(int i = 0; i<n;i++) insert_record(*(records+i));
            printf("arvore k-d gerada\n");
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
                    printf("inserido registro com nome: %s\n",r.data.name);
                }
                break;
                case 'p':{
                    int page_index;
                    int* indexes_size = malloc(sizeof(int));
                    int count = 0;
                    int page_position = -1;
                    bool found = false;
                    node* indexes = load_indexes_vector(indexes_size);
                    scanf("%d",&page_index);
                    find_page(0,&count,page_index,indexes,*indexes_size,&found, &page_position);
                    printf("pagina: %d\n",page_index);
                    print_page(page_position);
                    free(indexes_size);
                    free(indexes);
                    
                }
                break;
                case 'c':{
                    char name[MAXNAMESIZE];
                    getchar();
                    scanf("%20[^\n]", name);
                    simple_search_name(name);
                }
                break;
                case 'n':{
                    char name1[MAXNAMESIZE];
                    getchar();
                    scanf("%20[^\n]", name1);
                    char name2[MAXNAMESIZE];
                    getchar();
                    scanf("%20[^\n]", name2);
                    range_author_search(name1, name2);
                }
                break;
                case 'a':{
                    unsigned int ano1;
                    getchar();
                    scanf("%u", &ano1);
                    unsigned int ano2;
                    getchar();
                    scanf("%u", &ano2);
                    range_year_search(ano1, ano2);
                }
                break;
                case 'q':{
                    char name1[MAXNAMESIZE];
                    getchar();
                    scanf("%20[^\n]", name1);
                    char name2[MAXNAMESIZE];
                    getchar();
                    scanf("%20[^\n]", name2);
                    unsigned int ano1;
                    getchar();
                    scanf("%u", &ano1);
                    unsigned int ano2;
                    getchar();
                    scanf("%u", &ano2);
                    range_author_year_search(name1, name2, ano1, ano2);
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
                 case 'm':{
                    char word[MAXWORDSIZE];
                    scanf("%s",word);
                    printf("tabela pi para a palavra: %s:\n",word);
                    int* pi_table = create_pi_table(word);
                    int i = 0;
                    while(word[i]!='\0'){
                        printf("'%c': %d\n",word[i],pi_table[i+1]);
                        i = i+1;
                    }
                }
                break;
                //for debug purposes only
                case '*':{
                    print_nodes();
                    print_linked_pages();
                }
                break;
            }
        }
    }
    

}