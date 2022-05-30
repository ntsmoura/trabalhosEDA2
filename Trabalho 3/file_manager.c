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

//Create the main file if it doesn't exist
void create_file(){
    FILE *f;
	if(!(f = fopen(MAIN_FILE,"wb+"))) exit(-1);

	int n = 0;
	int linked_pages_number = 0;

	fwrite(&n,sizeof(int),1,f);
	fwrite(&linked_pages_number,sizeof(int),1,f);

	fclose(f);
}

//Create k-d tree structure
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


//Insert key nodes in k-d tree
void insert_node(record r){
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,2*sizeof(int),SEEK_SET);
	node actual;
	fread(&actual,sizeof(node),1,f);
	int found = 0;
	int level = 1;
	int actual_position = 2*sizeof(int);
	bool is_left_son = false;
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

//Insert empty pages and link them to respective parents
void insert_empty_pages(){
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,0,SEEK_SET);
	int number;
	node actual;
	node empty_page;
	empty_page.is_page = true;
	empty_page.left_son = -1;
	empty_page.right_son = -1;
	page p;
	p.qty = 0;
	p.linked_page = -1;
	empty_page.p = p;
	fread(&number,sizeof(int),1,f);
	int current_number = number;
	for(int i = 0; i<number;i++){
		int actual_position = 2*sizeof(int)+i*sizeof(node);
		fseek(f,actual_position,SEEK_SET);
		fread(&actual,sizeof(node),1,f);
		if(actual.left_son==-1) {
			actual.left_son = current_number;
			current_number+=1;
			fseek(f,0,SEEK_END);
			fwrite(&empty_page,sizeof(node),1,f);
		}
		if(actual.right_son==-1) {
			actual.right_son = current_number;
			current_number+=1;
			fseek(f,0,SEEK_END);
			fwrite(&empty_page,sizeof(node),1,f);
		}
		fseek(f,actual_position,SEEK_SET);
		fwrite(&actual,sizeof(node),1,f);
		fseek(f,0,SEEK_SET);
		fwrite(&current_number,sizeof(int),1,f);
	}
	fclose(f);

}

//Try to insert one record inside a page, if the page is full try to find linked pages with rooms, creates a new linked page if necessary
void insert_into_page(record r, int page_node_position){
	int number, linked_page_number, found,linked_page_position;
	node n;
	page p;
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,0,SEEK_SET);
	fread(&number,sizeof(int),1,f);
	fread(&linked_page_number,sizeof(int),1,f);
	fseek(f,page_node_position,SEEK_SET);
	fread(&n,sizeof(node),1,f);
	p = n.p;
	found = 0;
	//Check if the page inside the node has a room for the record, if not, create the first linked page of that node page
	if(p.qty < NREGSPORPAGINA){
		p.records[p.qty] = r;
		p.qty+=1;
		n.p = p;
		fseek(f,page_node_position,SEEK_SET);
		fwrite(&n,sizeof(node),1,f);
	} 
	else if(p.linked_page == -1){
		page new_page;
		new_page.qty = 1;
		new_page.records[0] = r;
		new_page.linked_page = -1;
		p.linked_page = linked_page_number;
		linked_page_number+=1;
		n.p = p;
		fseek(f,page_node_position,SEEK_SET);
		fwrite(&n,sizeof(node),1,f);
		fseek(f,1*sizeof(int),SEEK_SET);
		fwrite(&linked_page_number,sizeof(page),1,f);
		fseek(f,0,SEEK_END);
		fwrite(&new_page,sizeof(page),1,f);
		found = 1;
	}
	else {
		while(!found){
			if(p.linked_page!=-1){
				linked_page_position = 2*sizeof(int) + number*sizeof(node) + p.linked_page*sizeof(page);
				fseek(f,linked_page_position,SEEK_SET);
				fread(&p,sizeof(page),1,f);
			} else{
				if(p.qty < NREGSPORPAGINA){
					p.records[p.qty] = r;
					p.qty+=1;
					fseek(f,linked_page_position,SEEK_SET);
					fwrite(&p,sizeof(page),1,f);
					found = 1;
				} else{
					page new_page;
					new_page.qty = 1;
					new_page.records[0] = r;
					new_page.linked_page = -1;
					p.linked_page = linked_page_number;
					linked_page_number+=1;
					fseek(f,linked_page_position,SEEK_SET);
					fwrite(&p,sizeof(page),1,f);
					fseek(f,1*sizeof(int),SEEK_SET);
					fwrite(&linked_page_number,sizeof(page),1,f);
					fseek(f,0,SEEK_END);
					fwrite(&new_page,sizeof(page),1,f);
					found = 1;
				}
			}
		}
	}
	fclose(f);

}

//for debug purposes only
void print_node(node n){
	if(!n.is_page){
		if(n.level%2!=0) printf("LEVEL: %d, IS PAGE?: %d, LEFT SON: %d, RIGHT SON: %d, VALUE: %s\n",n.level,n.is_page,n.left_son,n.right_son,n.name);
		else printf("LEVEL: %d, IS PAGE?: %d, LEFT SON: %d, RIGHT SON: %d, VALUE: %d\n",n.level,n.is_page,n.left_son,n.right_son,n.year);
	} else printf("PAGE - QTY:%d, LINKED PAGE: %d, RECORDS NAMES: %s %s %s\n",n.p.qty,n.p.linked_page,n.p.records[0].data.name,n.p.records[1].data.name,n.p.records[2].data.name);
}

//for debug purposes only
void print_nodes(){
	int number;
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
	fseek(f,0,SEEK_SET);
	fread(&number,sizeof(int),1,f);
	fseek(f,2*sizeof(int),SEEK_SET);
	for(int i = 0; i<number;i++){
		node n;
		fread(&n,sizeof(node),1,f);
		print_node(n);
	}
}

//for debug purposes only
void print_linked_pages(){
	int number,linked_pages_number;
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb"))) exit(-1);
	fseek(f,0,SEEK_SET);
	fread(&number,sizeof(int),1,f);
	fread(&linked_pages_number,sizeof(int),1,f);
	fseek(f,2*sizeof(int)+number*sizeof(node),SEEK_SET);
	for(int i = 0; i<linked_pages_number;i++){
		page p;
		fread(&p,sizeof(page),1,f);
		printf("LINKED PAGE - QTY:%d, LINKED PAGE: %d, RECORDS NAMES: %s %s %s\n",p.qty,p.linked_page,p.records[0].data.name,p.records[1].data.name,p.records[2].data.name);
	}
}
