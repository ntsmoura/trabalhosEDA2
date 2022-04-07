/* 
Author: Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: file_manager.c
*/

#include<stdio.h>
#include<stdlib.h>
#include "record.h"
#include <string.h>

//h1 function for hashing
int hash_h1(int key){
    return key % MAXNUMREGS;
}

//h2 function for hashing
int hash_h2(int key){
    int right = 1;
    int left = (key / MAXNUMREGS) % MAXNUMREGS;
    if (left >= right) return left;
    else return right;
}

//Calculates offset for given position
long int set_offset(int position){
    return (position)*sizeof(record);
}

//Create a file if it doesn't exist
void create_file(){

    FILE *f;

	if(!(f = fopen("records","w+"))) exit(-1);

    record r;
    //Set record status: e -> empty, o -> ocuped, * -> previously ocuped
    r.status = 'e';

    int i;
	for (i=0; i < MAXNUMREGS; i++) {
		fwrite (&r, sizeof(record), 1, f);
	}
			
	fclose(f);

}

//Search for key in a file using double hashing
bool search_data(int key, int* number_of_access, int* last_position, int* first_insert_position, record* return_record){

    FILE *f;

    if(!(f = fopen("records","r"))) {
        create_file();
        if(!(f = fopen("records","r"))) exit(-1);
    }
    record r;
    int h1 = hash_h1(key);
	fseek (f,set_offset(h1), SEEK_SET);
    fread (&r, sizeof (record), 1, f);
    (*number_of_access)++;
    (*last_position) = h1;
    if(r.status=='o' && r.data.key==key){
        fclose(f);
        *return_record = r;
        return true;
    }
    else if(r.status == 'e'){ 
        fclose(f);
        return false;
    }
    else{
        if(r.status == '*' && (*first_insert_position == -1)) (*first_insert_position) = h1;
        int h2 = hash_h2(key);
        int position = (h1 + h2) % MAXNUMREGS;
        while(position != h1){
            fseek (f,set_offset(position), SEEK_SET);
            fread (&r, sizeof (record), 1, f);
            (*number_of_access)++;
            (*last_position) = position;
            if(r.status=='e') break;
            else if(r.status == '*'){
                if(*first_insert_position == -1) (*first_insert_position) = position;
                position+= h2;
                position = position % MAXNUMREGS;
                continue;
            }
            else if(r.data.key==key){
                fclose(f);
                *return_record = r;
                return true;
            }
            position+= h2;
            position = position % MAXNUMREGS;
        }
        (*last_position) = position;
        fclose(f);
        return false;
    }

}

//Insert data in a file
void insert_data(int key, char name[MAXNAMESIZE], int age){

    int* number_of_access = malloc(sizeof(int));
    int* last_position = malloc(sizeof(int));
    int* first_insert_position = malloc(sizeof(int));
    record* re = malloc(sizeof(record));

    *number_of_access = 0;
    *last_position = 0;
    *first_insert_position = -1;
 
    bool result = search_data(key, number_of_access, last_position, first_insert_position, re);

    FILE *f;

    record r;
    
    if(!(f = fopen("records","r+"))) exit(-1);
    if (result) printf("chave ja existente: %d\n",key);
    else{
        fseek (f,set_offset(*last_position), SEEK_SET);
        fread (&r, sizeof (record), 1, f);
        if(*first_insert_position !=-1) (*last_position) = *first_insert_position;
        if (*last_position == hash_h1(key) && r.status=='o') printf("insercao de chave sem sucesso - arquivo cheio: %d\n",key);
        else{
            r.data.key = key;
            strcpy(r.data.name, name);
            r.data.age = age;
            r.status = 'o';
            fseek (f,set_offset(*last_position), SEEK_SET);
            if (!(fwrite (&r, sizeof(record), 1, f))) exit(-1);
            printf("insercao com sucesso: %d\n",key);
        }
    }

    free(number_of_access);
    free(last_position);
    free(first_insert_position);
    free(re);
    fclose(f);

}

//Delete record in a file if find given jey
void delete_data(int key){

    int* number_of_access = malloc(sizeof(int));
    int* last_position = malloc(sizeof(int));
    int* first_insert_position = malloc(sizeof(int));
    record* re = malloc(sizeof(record));

    *number_of_access = 0;
    *last_position = 0;
 
    bool result = search_data(key, number_of_access, last_position, first_insert_position, re);

    FILE *f;

    record r;
    
    if(!(f = fopen("records","r+"))) exit(-1);
    if (result){
        fseek (f,set_offset(*last_position), SEEK_SET);
        fread (&r, sizeof (record), 1, f);
        r.status = '*';
        fseek (f,set_offset(*last_position), SEEK_SET);
        if (!(fwrite (&r, sizeof(record), 1, f))) exit(-1);
        printf("chave removida com sucesso: %d\n",key);
    }
    else{
        printf("chave nao encontrada: %d\n",key);
    }

    free(number_of_access);
    free(first_insert_position);
    free(last_position);
    free(re);
    fclose(f);

}

//Print all data from a file
void print_data(){

    FILE *f;

    if(!(f = fopen("records","r"))){
        create_file();
        if(!(f = fopen("records","r"))) exit(-1);
    }
    int i;
    record r;
    fseek (f, 0, SEEK_SET);

    for(i = 0; i< MAXNUMREGS;i++){
        fread (&r, sizeof(record), 1, f);
        printf("%d: ", i);
        if(r.status=='o') printf("%d %s %d\n",r.data.key,r.data.name,r.data.age);
        else if(r.status=='e') printf("vazio\n");
        else printf("*\n");
    }

    fclose(f);

}
