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

//print structure of the file (for debug process only)
void print_structure(){
    FILE *f;
    f = fopen("records.dat","r");
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
 