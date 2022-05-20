/* 
Author: Caio Nery e Natan Moura
Teacher: Flávio Assis Silva
Class: MATA54
File: file_manager.c
*/

#include <stdio.h>
#include<stdlib.h>
#include "structures.h"
#include <string.h>

//Creates the main file if it doesn't exist
void create_file(){
    FILE *f;
	if(!(f = fopen(MAIN_FILE,"wb+"))) exit(-1);

	int n = 0;
	int linked_pages_number = 0;

	fwrite(&n,sizeof(int),1,f);
	fwrite(&linked_pages_number,sizeof(int),1,f);

	fclose(f);
}

//Creates k-d tree structure
void create_root(record r){
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,0,SEEK_SET);
	int number;
	fread(&number,sizeof(int),1,f);
	number = number+1;
	fseek(f,0,SEEK_SET);
	fwrite(&number,sizeof(int),1,f);
	fseek(f,2*sizeof(int),SEEK_SET);
	node n;
	n.level = 1;
	n.is_page = false;
	n.left_son = -1;
	n.right_son = -1;
	strcpy(n.name,r.data.name);
	fwrite(&n,sizeof(node),1,f);
	fclose(f);
}

void insert_node(record r){
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,2*sizeof(int),SEEK_SET);
	node actual;
	fread(&actual,sizeof(node),1,f);
	int found = 0;
	int level = 1;
	int actual_position = 2*sizeof(int);
	bool is_left_son;
	while(!found){
		level = level + 1;
		if(actual.level%2!=0) {
			int result = strcmp(r.data.name,actual.name);
			if(result <= 0){
				if(actual.left_son == -1){
					found = 1;
					is_left_son = true;
				}
				else {
					actual_position = 2*sizeof(int)+actual.left_son*sizeof(node);
					fseek(f,actual_position,SEEK_SET);
					fread(&actual,sizeof(node),1,f);
				}
			} 
			else {
				if(actual.right_son == -1){
					found = 1;
				}
				else {
					actual_position = 2*sizeof(int)+actual.right_son*sizeof(node);
					fseek(f,actual_position,SEEK_SET);
					fread(&actual,sizeof(node),1,f);
				}
			}
		} else {
			if(r.data.year <= actual.year){
				if(actual.left_son == -1){
					found = 1;
					is_left_son = true;
				}
				else {
					actual_position = 2*sizeof(int)+actual.left_son*sizeof(node);
					fseek(f,actual_position,SEEK_SET);
					fread(&actual,sizeof(node),1,f);
				}
			} 
			else {
				if(actual.right_son == -1){
					found = 1;
				}
				else {
					actual_position = 2*sizeof(int)+actual.right_son*sizeof(node);
					fseek(f,actual_position,SEEK_SET);
					fread(&actual,sizeof(node),1,f);
				}
			}
		}
	}
	fseek(f,0,SEEK_SET);
	int number;
	fread(&number,sizeof(int),1,f);
	if(is_left_son) actual.left_son = number;
	else actual.right_son = number;
	fseek(f,actual_position,SEEK_SET);
	fwrite(&actual,sizeof(node),1,f);
	fseek(f,0,SEEK_END);
	node new_node;
	new_node.is_page = false;
	new_node.left_son = -1;
	new_node.right_son = -1;
	new_node.level = level;
	if(level%2!=0) strcpy(new_node.name,r.data.name);
	else new_node.year = r.data.year;
	fwrite(&new_node,sizeof(node),1,f);

	number = number +1;
	fseek(f,0,SEEK_SET);
	fwrite(&number,sizeof(int),1,f);


	fclose(f);

}

//for debug purposes only
void print_node(node n){
	if(!n.is_page){
		if(n.level%2!=0) printf("LEVEL: %d, IS PAGE?: %d, LEFT SON: %d, RIGHT SON: %d, VALUE: %s\n",n.level,n.is_page,n.left_son,n.right_son,n.name);
		else printf("LEVEL: %d, IS PAGE?: %d, LEFT SON: %d, RIGHT SON: %d, VALUE: %d\n",n.level,n.is_page,n.left_son,n.right_son,n.year);
	} else printf("PAGE\n");
}

//for debug purposes only
void print_nodes(int number){
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
	fseek(f,2*sizeof(int),SEEK_SET);
	for(int i = 0; i<number;i++){
		node n;
		fread(&n,sizeof(node),1,f);
		print_node(n);
	}
}
