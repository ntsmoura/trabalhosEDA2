/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: file_manager.c
*/

#include<stdio.h>
#include<stdlib.h>
#include "structures.h"
#include <string.h>

#define MAIN_FILE  "records.dat"
#define TEMPORARY_DATA_FILE  "temporary.dat"
#define TEMPORARY_DATA_VECTOR_FILE  "vector.dat"

//Generate a random int between min and max 
int random_number(int min, int max){
    return min + rand() % (max + 1 - min);
}

//Return the hashing value based on input parameters
int universal_hashing(int key,int a, int b, int prime, int m){
    return ((a*key+b)%prime)%m;
}

//print structure of the file (for debug process only)
void print_structure(){
    FILE *f;
    if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
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
    second_level f2;
    int second_level_size = 0;
    for(int i = 0; i<number; i++){
        fread(&f1,sizeof(first_level),1,f);
        if(f1.info[0]>0) second_level_size+=f1.info[0]; 
        printf("F1: Index: %d, INFO: [%d,%d,%d], SECOND LEVEL: %d, SECOND LEVEL OFFSET: %d\n",f1.index,f1.info[0],f1.info[1],f1.info[2],f1.second_level,f1.second_level_offset);
    }
    for(int i = 0; i<second_level_size;i++){
        fread(&f2,sizeof(second_level),1,f);
        if(f2.r.status)
            printf("F2: Index: %d,FIRST LEVEL INDEX: %d, RECORD(%d).\n",f2.index,f2.first_level_index,f2.r.data.key);
        else  printf("F2: Index: %d,FIRST LEVEL INDEX: %d, RECORD(VAZIO).\n",f2.index,f2.first_level_index);
    }
    fclose(f);
}

//Create the main file if it doesn't exist
void create_file(int a, int b, int p, int m){

    FILE *f;

	if(!(f = fopen(MAIN_FILE,"wb+"))) exit(-1);
    fwrite(&a,sizeof(int),1,f);
    fwrite(&b,sizeof(int),1,f);
    fwrite(&p,sizeof(int),1,f);
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
    if(!(f = fopen(TEMPORARY_DATA_FILE,"wb+"))) exit(-1);
    fclose(f);
}

//Create temporary data "vector" file
void create_temporary_vector_file(){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_VECTOR_FILE,"wb+"))) exit(-1);
    fclose(f);
}

//Return the record from a defined position in data "vector" file
record return_vector_record(int position){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_VECTOR_FILE,"r"))) exit(-1);
    fseek(f,position*sizeof(record),SEEK_SET);
    record r;
    fread(&r,sizeof(record),1,f);
    fclose(f);
    return r;
}

//Insert record inside data "vector" file
void insert_vector_record(record r){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_VECTOR_FILE,"rb+"))) exit(-1);
    fseek(f, 0,SEEK_END);
    fwrite(&r,sizeof(record),1,f);
    fclose(f);
}

//Reset every value to 0 in secondary collision detection "vector"
void reset_vector_collision_flags(int numbers, int m){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_VECTOR_FILE,"rb+"))) exit(-1);
    for(int i = 0; i< m; i++){
        fseek(f, numbers*sizeof(record) + i*sizeof(int),SEEK_SET);
        int value = 0;
        fwrite(&value,sizeof(int),1,f);
    }
    fclose(f);
}

//Insert data in secondary integer "vector" for collision detection
void insert_vector_collision_flag(int numbers, int position){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_VECTOR_FILE,"rb+"))) exit(-1);
    fseek(f, numbers*sizeof(record) + position*sizeof(int),SEEK_SET);
    int value = 1;
    fwrite(&value,sizeof(int),1,f);
    fclose(f);
}

//Return defined position data in secondary integer "vector" for collision detection
int return_vector_collision_flag(int numbers, int position){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_VECTOR_FILE,"rb+"))) exit(-1);
    fseek(f, numbers*sizeof(record) + position*sizeof(int),SEEK_SET);
    int value = 0;
    fread(&value,sizeof(int),1,f);
    fclose(f);
    return value;
}

//Insert data in temporary file
void insert_temporary_file(int key, char name[MAXNAMESIZE], int age){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_FILE,"rb+"))) exit(-1);
    fseek(f,0,SEEK_END);
    record r;
    r.status = true;
    r.data.key = key;
    r.data.age = age;
    strcpy(r.data.name, name);
    fwrite(&r,sizeof(record),1,f);
    fclose(f);
}
 
//Print temporary file content (for debug process only)
void print_temporary_file(int m){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_FILE,"rb"))) exit(-1);
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

//Delete the temporary files
void delete_temporary_files(){
    if(remove(TEMPORARY_DATA_FILE) || remove(TEMPORARY_DATA_VECTOR_FILE)) exit(-1);
}

//Calculates the number of elements per index and store in the temporary file
void calculate_elements_first_level(int a,int b,int p,int m){
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_FILE,"rb+"))) exit(-1);
    fseek(f,0,SEEK_END);
    int i;
    for (i = 0; i<m;i++){
        int number = 0;
        fwrite(&number,sizeof(int),1,f);
    }
    int result = 0;
    int position = 0;
    record r;
    for(i = 0; i<m;i++){
        fseek(f,i*sizeof(record),SEEK_SET);
        fread(&r,sizeof(record),1,f);
        result = universal_hashing(r.data.key,a,b,p,m);
        position = m*sizeof(record) + result*sizeof(int);
        fseek(f,position,SEEK_SET);
        int value = 0;
        fread(&value,sizeof(int),1,f);
        value += 1;
        fseek(f,position,SEEK_SET);
        fwrite(&value,sizeof(int),1,f);
    }
    fclose(f);
}

//Find first level elements of given first level 
//After find each element stores them in temporary data "vector"
void first_level_elements(first_level f1, int a, int b, int p, int m, int* f1_size){
    create_temporary_vector_file();
    FILE *f;
    if(!(f = fopen(TEMPORARY_DATA_FILE,"rb+"))) exit(-1);
    int base_position = m*sizeof(record) + (f1.index)*sizeof(int);
    fseek(f,base_position,SEEK_SET);
    int number;
    fread(&number,sizeof(int),1,f);
    (*f1_size) = number;
    fseek(f,0,SEEK_SET);
    int result = 0;
    int actual = 0;
    record r;
    for(int i = 0; i<m;i++){
        if (actual == number) break;
        fseek(f,i*sizeof(record),SEEK_SET);
        fread(&r,sizeof(record),1,f);
        result = universal_hashing(r.data.key,a,b,p,m);
        if(result == f1.index){
            insert_vector_record(r);
            actual++;
        }
    }
    fclose(f);
}


//Create second level structs in main file
//Consults every first level structure and try to create a corresponding second level (if necessary)
//Try to find a' and b' indexes for each second level avoiding collisions
void create_second_level(int a, int b, int m, int p){
    FILE *f;
    if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
    int second_level_actual_offset = 0;
    for(int i = 0; i< m; i++){
        int position = 4*sizeof(int) + i*sizeof(first_level);
        fseek(f,position,SEEK_SET);
        first_level f1;
        fread(&f1,sizeof(first_level),1,f);
        int f1_size;
        first_level_elements(f1,a,b,p,m, &f1_size);
        int a1=0,b1=0;
        int m1 = f1_size*f1_size;
        
        //Way to set fixed A1 and B1 indexes, to debug purpose only
        //scanf("%d %d",&a1,&b1);
        if (m1 == 1){
            a1 = 0;
            b1 = 0;
        }
        else if(m1 > 1){
            int found = 0;
            //If found = 1 that means the program found a' and b' that avoids collision
            //Set found = 0 if the program found any collision
            while(!found){
                found = 1;
                reset_vector_collision_flags(f1_size,m1);
                a1 = random_number(1,p-1);
                b1 = random_number(0,p-1);
                for(int j = 0; j<f1_size;j++){
                    int key = return_vector_record(j).data.key;
                    int result = universal_hashing(key,a1,b1,p,m1);
                    if(return_vector_collision_flag(f1_size,result) == 1) {
                        found = 0;
                        break;
                    }
                    else insert_vector_collision_flag(f1_size,result);
                }
            }
        }
        //Updates first level information and creates the corresponding second level
        f1.info[0] = m1;
        f1.info[1] = a1;
        f1.info[2] = b1;
        if(m1 > 0){ 
            f1.second_level = true;
            f1.second_level_offset = second_level_actual_offset;
            second_level_actual_offset+=m1;
        }
        fseek(f,position,SEEK_SET);
        fwrite(&f1,sizeof(first_level),1,f);
        fseek(f,0,SEEK_END);
        for (int k = 0; k<m1;k++){
            second_level f2;
            record r;
            r.status = false;
            f2.first_level_index = f1.index;
            f2.index = k;
            f2.r = r;
            fwrite(&f2,sizeof(second_level),1,f);
        }
        for(int k = 0; k < f1_size; k++){
            record r1 = return_vector_record(k);
            int result = universal_hashing(r1.data.key,a1,b1,p,m1);
            int second_level_position = 4*sizeof(int) + m*sizeof(first_level) + f1.second_level_offset*sizeof(second_level) + result*sizeof(second_level);
            second_level f2;
            fseek(f,second_level_position,SEEK_SET);
            fread(&f2,sizeof(second_level),1,f);
            f2.r.status = true;
            f2.r.data.key = r1.data.key;
            strcpy(f2.r.data.name,r1.data.name);
            f2.r.data.age = r1.data.age;
            fseek(f,second_level_position,SEEK_SET);
            fwrite(&f2,sizeof(second_level),1,f);
        }
    }
    fclose(f);
}

//Returns the position corresponding or returns -1
//If theres not the second level in index of first level table
//Or theres only one key in the index and it isnt corresponding
//Or theres more than one key but all of them isnt corresponding
int get_position(int key){
    FILE *f; 
    if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
    fseek(f,0,SEEK_SET); 
    //Gets the a value of first level
    int a;
    fread(&a,sizeof(int),1,f);
    //Gets the b value of second level
    int b;
    fread(&b,sizeof(int),1,f);
    //Gets the prime value of first level
    int prime;
    fread(&prime,sizeof(int),1,f);
    //Gets the m value of first level
    int m;
    fread(&m,sizeof(int),1,f);
    //Gets the position by universal hashing
    int result = universal_hashing(key,a,b,prime,m);
    //Adjust the possition in the file for the first level
    int first_level_position = 4*sizeof(int) + result*sizeof(first_level);
    fseek(f,first_level_position,SEEK_SET);
    first_level f1;
    fread(&f1,sizeof(first_level),1,f);
    if(!f1.second_level){
        fclose(f);
        return -1;
    }
    else if(f1.info[0] == 1){
        int second_level_position = 4*sizeof(int) + m*sizeof(first_level) + f1.second_level_offset*sizeof(second_level);
        fseek(f,second_level_position,SEEK_SET);
        second_level f2;
        fread(&f2,sizeof(second_level),1,f);
        if(f2.r.status && f2.r.data.key == key){
            fclose(f);
            return second_level_position;
        }
        else{
            fclose(f);
            return -1;
        }
    }
    else{
        int result = universal_hashing(key,f1.info[1],f1.info[2],prime,f1.info[0]);
        int second_level_position = 4*sizeof(int) + m*sizeof(first_level) + f1.second_level_offset*sizeof(second_level) + result*sizeof(second_level);
        fseek(f,second_level_position,SEEK_SET);
        second_level f2;
        fread(&f2,sizeof(second_level),1,f);
        if(f2.r.status && f2.r.data.key == key){
            fclose(f);
            return second_level_position;
        }
        else {
            fclose(f);
            return -1;
        }
    }
}

//Query if there is a record with the corresponding key
//Gets the position by the get_position function
void consult_file(int key){
    int pos = get_position(key);
    if(pos == -1)
        printf("chave nao encontrada: %d\n", key);
    else{
        printf("chave: %d\n", key);
        FILE *f; 
        if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
        fseek(f,pos,SEEK_SET);
        second_level f2;
        fread(&f2,sizeof(second_level),1,f);
        printf("%s\n", f2.r.data.name);
        printf("%d\n", f2.r.data.age);
        fclose(f);
    }
}

//Print the first level
void print_first_level(){
    FILE *f; 
    if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
    fseek(f,0,SEEK_SET);
    //Gets the a value of first level
    int a;
    fread(&a,sizeof(int),1,f);
    //Gets the b value of second level
    int b;
    fread(&b,sizeof(int),1,f);
    //Gets the prime value of first level
    int prime;
    fread(&prime,sizeof(int),1,f);
    //Gets the m value of first level
    int m;
    fread(&m,sizeof(int),1,f);
    printf("hashing perfeito: primeiro nivel\n");
    printf("tamanho da tabela: %d\n", m);
    printf("parametro a: %d\n", a);
    printf("parametro b: %d\n", b);
    printf("numero primo: %d\n", prime);

    for(int i=0;i<m;i++){
        int first_level_position = 4*sizeof(int) + i*sizeof(first_level);
        fseek(f,first_level_position,SEEK_SET);
        first_level f1;
        fread(&f1,sizeof(first_level),1,f);
        if(f1.second_level){
            printf("%d:", f1.index);
            int second_level_size = f1.info[0];
            for(int j=0;j<second_level_size;j++){
                int second_level_position = 4*sizeof(int) + m*sizeof(first_level) + f1.second_level_offset*sizeof(second_level) + j*sizeof(second_level);
                fseek(f,second_level_position,SEEK_SET);
                second_level f2;
                fread(&f2,sizeof(second_level),1,f);
                if(f2.r.status)
                    printf(" %d", f2.r.data.key);
            }
             printf("\n");
        }
    }
    fclose(f);
}

//Print the second level
void print_second_level(int index){
    FILE *f; 
    if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
    fseek(f,0,SEEK_SET);
    //Gets the a value of first level
    int a;
    fread(&a,sizeof(int),1,f);
    //Gets the b value of second level
    int b;
    fread(&b,sizeof(int),1,f);
    //Gets the prime value of first level
    int prime;
    fread(&prime,sizeof(int),1,f);
    //Gets the m value of first level
    int m;
    fread(&m,sizeof(int),1,f);
    int first_level_position = 4*sizeof(int) + index*sizeof(first_level);
    fseek(f,first_level_position,SEEK_SET);
    first_level f1;
    fread(&f1,sizeof(first_level),1,f);
    printf("hashing perfeito: segundo nivel - indice: %d\n", f1.index);
    printf("tamanho da tabela: %d\n", f1.info[0]);
    printf("parametro a: %d\n", f1.info[1]);
    printf("parametro b: %d\n", f1.info[2]);
    printf("numero primo: %d\n", prime);
    if(f1.second_level){
        int second_level_size = f1.info[0];
        for(int j=0;j<second_level_size;j++){
            int second_level_position = 4*sizeof(int) + m*sizeof(first_level) + f1.second_level_offset*sizeof(second_level) + j*sizeof(second_level);
            fseek(f,second_level_position,SEEK_SET);
            second_level f2;
            fread(&f2,sizeof(second_level),1,f);
            if(f2.r.status)
                printf("%d: %d\n", f2.index ,f2.r.data.key);
        }
    }
    fclose(f);
}

//Print for every index that has at least one key
void print_full_second_level(){
    FILE *f; 
    if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
    fseek(f,3*sizeof(int),SEEK_SET);
    //Gets the m value of first level
    int m;
    fread(&m,sizeof(int),1,f);
    for(int i=0;i<m;i++){
        int first_level_position = 4*sizeof(int) + i*sizeof(first_level);
        fseek(f,first_level_position,SEEK_SET);
        first_level f1;
        fread(&f1,sizeof(first_level),1,f);
        if(f1.second_level)
            print_second_level(i);
    }
    fclose(f);
}

//Print the number of functions 
void number_of_functions(){
    FILE *f; 
    if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
    fseek(f,2*sizeof(int),SEEK_SET);
    //Gets the prime value of first level
    int prime;
    fread(&prime,sizeof(int),1,f);
    //Gets the m value of first level
    int m;
    fread(&m,sizeof(int),1,f);

    int number_of_functions = prime * (prime-1);
    printf("%d\n", number_of_functions);
    fclose(f);
}