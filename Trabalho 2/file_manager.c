/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: file_manager.c
*/

#include<stdio.h>
#include<stdlib.h>
#include "record.h"
#include <time.h>
#include <string.h>
//Generate a random int between min and max (try to better randomize it using seed as clock)
int random(int min, int max){
    srand(clock());
    return min + rand() % (max + 1 - min);
}

//Return the hashing value based on input parameters
int universal_hashing(int key,int a, int b, int prime, int m){
    return ((a*key+b)%prime)%m;
}

//print structure of the file (for debug process only)
void print_structure(){
    FILE *f;
    if(!(f = fopen("records.dat","r"))) exit(-1);
    int number;
    fread(&number,sizeof(int),1,f);
    printf("%d\n",number);
    fread(&number,sizeof(int),1,f);
    printf("%d\n",number);
    fread(&number,sizeof(int),1,f);
    printf("%d\n",number);
    fread(&number,sizeof(int),1,f);
    printf("%d\n",number);
    first_level f1;
    for(int i = 0; i<number; i++){
        fread(&f1,sizeof(first_level),1,f);
        printf("F1: Index: %d, INFO: [%d,%d,%d], SECOND LEVEL: %d, SECOND LEVEL OFFSET: %d\n",f1.index,f1.info[0],f1.info[1],f1.info[2],f1.second_level,f1.second_level_offset);
    }
    fclose(f);
}

//Create a file if it doesn't exist
void create_file(int a, int b, int m){

    FILE *f;

	if(!(f = fopen("records.dat","w+"))) exit(-1);
    fwrite(&a,sizeof(int),1,f);
    fwrite(&b,sizeof(int),1,f);
    int prime = PRIME;
    fwrite(&prime,sizeof(int),1,f);
    fwrite(&m,sizeof(int),1,f);
    first_level f1;
    int i;
	for (i=0; i < m; i++) {
        f1.index = i;
        f1.info[0] = 0;
        f1.info[1] = 0;
        f1.info[2] = 0;
        f1.second_level = false;
        f1.second_level_offset = -1;
		fwrite (&f1, sizeof(first_level), 1, f);
	}
			
	fclose(f);

}

//Create temporary file to store data
void create_temporary_file(){
    FILE *f;
    if(!(f = fopen("temporary.dat","w+"))) exit(-1);
    fclose(f);
}

//Insert data in temporary file
void insert_temporary_file(int key, char name[MAXNAMESIZE], int age){
    FILE *f;
    if(!(f = fopen("temporary.dat","r+"))) exit(-1);
    fseek(f,0,SEEK_END);
    record r;
    r.status = 'o';
    r.data.key = key;
    r.data.age = age;
    strcpy(r.data.name, name);
    fwrite(&r,sizeof(record),1,f);
    fclose(f);
}
 
//Print temporary file content (for debug process only)
void print_temporary_file(int m){
    FILE *f;
    if(!(f = fopen("temporary.dat","r"))) exit(-1);
    record r;
    int i;
    int number;
    for(i = 0; i<m;i++){
        fread(&r,sizeof(record),1,f);
        printf("CHAVE: %d, NOME: %s, IDADE: %d\n",r.data.key,r.data.name,r.data.age);
    }
    for(i = 0; i<m;i++){
        fread(&number, sizeof(int),1,f);
        printf("COLISOES NIVEL %d: %d\n",i,number);
    }
    fclose(f);
    
}

//Calculates the number of elements per index and store in the temporary file
void calculate_elements_first_level(int a,int b,int p,int m){
    FILE *f;
    if(!(f = fopen("temporary.dat","r+"))) exit(-1);
    fseek(f,0,SEEK_END);
    int number = 0;
    int i;
    for (i = 0; i<m;i++){
        fwrite(&number,sizeof(int),1,f);
    }
    record r;
    int result = 0;
    int position = 0;
    for(i = 0; i<m;i++){
        fseek(f,i*sizeof(record),SEEK_SET);
        fread(&r,sizeof(record),1,f);
        result = universal_hashing(r.data.key,a,b,p,m);
        position = m*sizeof(record) + result*sizeof(int);
        fseek(f,position,SEEK_SET);
        int value = 0;
        fread(&value,sizeof(int),1,f);
        value++;
        fseek(f,position,SEEK_SET);
        fwrite(&value,sizeof(int),1,f);
    }
    fclose(f);
}

//Find first level elements of each index
record* first_level_elements(first_level f1, int a, int b, int p, int m, int* f1_size){
    FILE *f;
    if(!(f = fopen("temporary.dat","r+"))) exit(-1);
    int base_position = m*sizeof(record) + (f1.index)*sizeof(int);
    fseek(f,base_position,SEEK_SET);
    int number;
    fread(&number,sizeof(int),1,f);
    (*f1_size) = number;
    fseek(f,0,SEEK_SET);
    record* elements = calloc(number,sizeof(record));
    int result = 0;
    int actual = 0;
    record r;
    for(int i = 0; i<m;i++){
        if (actual == number) break;
        fseek(f,i*sizeof(record),SEEK_SET);
        fread(&r,sizeof(record),1,f);
        result = universal_hashing(r.data.key,a,b,p,m);
        if(result == f1.index){
            record copy;
            copy.status = r.status;
            copy.data.key = r.data.key;
            strcpy(copy.data.name,r.data.name);
            copy.data.age = r.data.age;
            *(elements+actual) = copy;
            actual++;
        }
    }
    fclose(f);
    return elements;
    

}