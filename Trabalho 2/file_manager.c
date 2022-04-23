/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: file_manager.c
*/

#include<stdio.h>
#include<stdlib.h>
#include "record.h"
#include <string.h>
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

//Delete the temporary file
void delete_temporary_file(){
    if(remove("temporary.dat")) exit(-1);
}

//Calculates the number of elements per index and store in the temporary file
void calculate_elements_first_level(int a,int b,int p,int m){
    FILE *f;
    if(!(f = fopen("temporary.dat","r+"))) exit(-1);
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


//Create second level structure
void create_second_level(int a, int b, int m, int p){
    FILE *f;
    if(!(f = fopen("records.dat","r+"))) exit(-1);
    int second_level_actual_offset = 0;
        for(int i = 0; i< m; i++){
            int position = 4*sizeof(int) + i*sizeof(first_level);
            fseek(f,position,SEEK_SET);
            first_level f1;
            fread(&f1,sizeof(first_level),1,f);
            int f1_size;
            record *elements = first_level_elements(f1,a,b,p,m, &f1_size);
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
                while(!found){
                    found = 1;
                    int* second_level_keys = calloc(m1,sizeof(int));
                    for(int k = 0; k<m1;k++) *(second_level_keys+k) = 0;
                    a1 = random_number(1,PRIME-1);
                    b1 = random_number(0,PRIME-1);
                    for(int j = 0; j<f1_size;j++){
                        int key = (*(elements+j)).data.key;
                        int result = universal_hashing(key,a1,b1,p,m1);
                        if(*(second_level_keys+result) == 1) {
                            found = 0;
                            break;
                        }
                        else (*(second_level_keys+result)) = 1;
                    }
                    free(second_level_keys);
                }
            }
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
                record r1 = *(elements+k);
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
            free(elements);
        }
    fclose(f);
}