/* 
Author: Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
*/

#include<stdio.h>
#include<stdlib.h>
#include "record.h"


//h1 function for hashing
int hash_h1(int key){
    return key % NUMBEROFRECORDS;
}

//h2 function for hashing
int hash_h2(int key){
    if (key < NUMBEROFRECORDS) return 1;
    else return (key / NUMBEROFRECORDS) % NUMBEROFRECORDS;
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
	for (i=0; i < NUMBEROFRECORDS; i++) {
		fwrite (&r, sizeof(record), 1, f);
	}
			
	fclose(f);

}

//Search for key in a file using double hashing
bool search_data(int key, int* number_of_access, int* last_position){

    FILE *f;

    if(!(f = fopen("records","r"))) create_file();
    else{
	    record r;
        int h1 = hash_h1(key);
	    fseek (f,set_offset(h1), SEEK_SET);
        fread (&r, sizeof (record), 1, f);
        (*number_of_access)++;
        (*last_position) = h1;
        if(r.status=='o' && r.data.key && r.data.key==key){
            fclose(f);
            return true;
        }
        else if(r.status == 'e' | r.status=='*'){ 
            fclose(f);
            return false;
        }
        else{
            int h2 = hash_h2(key);
            fseek (f,set_offset(h2), SEEK_SET);
            fread (&r, sizeof (record), 1, f);
            (*number_of_access)++;
            (*last_position) = h2;
            if(r.status=='o' && r.data.key && r.data.key==key){ 
                fclose(f);
                return true;
            }
            else if(r.status == 'e' | r.status=='*'){
                fclose(f);
                return false;
            }
            else{
                int position = h1 + h2;
                while(position != h1){
                    fseek (f,set_offset(h2), SEEK_SET);
                    fread (&r, sizeof (record), 1, f);
                    (*number_of_access)++;
                    (*last_position) = position;
                    if(r.status!='o') break;
                    if(r.data.key && r.data.key==key){
                        fclose(f);
                        return true;
                    }
                    position+= h2;
                    position = position%NUMBEROFRECORDS;
                }
                (*last_position) = position;
                fclose(f);
                return false;
            }
        }
    }
}

//Insert data in a file
void insert_data(int key, char *name, int age){

    int* number_of_access;
    int* last_position;

    bool result = search_data(key, number_of_access, last_position);

    FILE *f;

    record r;

    if(!(f = fopen("records","r+"))) exit(-1);
    if (result) printf("chave ja existente: %d",key);
    else{
        fseek (f,set_offset(*last_position), SEEK_SET);
        fread (&r, sizeof (record), 1, f);
        if (*last_position = hash_h1(key) && r.status=='o') printf("insercao de chave sem sucesso - arquivo cheio: %d",key);
        else{
            fseek (f,set_offset(*last_position), SEEK_SET);
            r.data.age = age;
            r.data.key = key;
            r.data.name = name;
            r.status = 'o';
            if (!(fwrite (&r, sizeof(record), 1, f))) exit(-1);
        }
    }
    fclose(f);

}

int main(){

insert_data(15,"Natan",15);
printf("funciona");

}