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

//Create k-d tree root
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

//Search for the leaf node to insert node or record
int search_node(record r, bool* is_left_son, int* level){
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,2*sizeof(int),SEEK_SET);
	node actual;
	fread(&actual,sizeof(node),1,f);
	int found = 0;
	int actual_position = 2*sizeof(int);
	while(!found){
		*level+=1;
		if(actual.level%2!=0) {
			int result = strcmp(r.data.name,actual.name);
			if(result <= 0){
				if(actual.left_son == -1){
					found = 1;
					*is_left_son = true;
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
					*is_left_son = true;
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
	fclose(f);
	return actual_position;
}


//Insert key nodes in k-d tree
void insert_node(record r){
	bool* is_left_son = malloc(sizeof(bool));
	int* level = malloc(sizeof(int));
	*level = 1;
	*is_left_son = false;
	int actual_position = search_node(r,is_left_son,level);
	int number;
	node actual;
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,0,SEEK_SET);
	fread(&number,sizeof(int),1,f);
	fseek(f,actual_position,SEEK_SET);
	fread(&actual,sizeof(node),1,f);
	if(*is_left_son) actual.left_son = number;
	else actual.right_son = number;
	fseek(f,actual_position,SEEK_SET);
	fwrite(&actual,sizeof(node),1,f);
	fseek(f,0,SEEK_END);
	node new_node;
	new_node.is_page = false;
	new_node.left_son = -1;
	new_node.right_son = -1;
	new_node.level = *level;
	if(*level%2!=0) strcpy(new_node.name,r.data.name);
	else new_node.year = r.data.year;
	fwrite(&new_node,sizeof(node),1,f);

	number = number +1;
	fseek(f,0,SEEK_SET);
	fwrite(&number,sizeof(int),1,f);

	free(level);
	free(is_left_son);
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
		fwrite(&linked_page_number,sizeof(int),1,f);
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
					fwrite(&linked_page_number,sizeof(int),1,f);
					fseek(f,0,SEEK_END);
					fwrite(&new_page,sizeof(page),1,f);
					found = 1;
				}
			}
		}
	}
	fclose(f);

}

//Insert record into page finding the correct page and using isert_into_page
void insert_record(record r){
	bool* is_left_son = malloc(sizeof(bool));
	int* level = malloc(sizeof(int));
	*level = 1;
	*is_left_son = false;
	int page_position = search_node(r,is_left_son,level);
	insert_into_page(r, page_position);
}

//print index value
void print_index_value(node n){
    if(n.level%2==0) printf(" %u ",n.year);
	else printf(" %s ",n.name);
}


//load k-d tree indexes into a vector
node* load_indexes_vector(int *indexes_size){
	FILE *f;
	int number;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,0,SEEK_SET);
	fread(&number,sizeof(int),1,f);
	*indexes_size = number/2;
	node* indexes = calloc(*indexes_size,sizeof(node));
	fseek(f,2*sizeof(int),SEEK_SET);
	for(int i = 0; i<*indexes_size;i++){
		node n;
		fread(&n,sizeof(node),1,f);
		*(indexes+i) = n;
	}
	fclose(f);
	return indexes;
}

//Print k-d tree indexes
void print_indexes(int position, node* indexes, int indexes_size){
	if(!(position>=indexes_size)){
		print_indexes((indexes+position)->left_son,indexes,indexes_size);
		if((indexes+position)->level%2==0) printf("ano:");
		else printf("nome:");
		print_index_value(*(indexes+position));
		printf("fesq:");
		int fesq = (indexes+position)->left_son;
		if(fesq < indexes_size) print_index_value(*(indexes+fesq));
		else printf(" pagina ");
		printf("fdir:");
		int fdir = (indexes+position)->right_son;
		if(fdir < indexes_size) print_index_value(*(indexes+fdir));
		else printf(" pagina ");
		printf("\n");
		print_indexes((indexes+position)->right_son,indexes,indexes_size);
	}
}

//Find the page with the correspondent index
void find_page(int position, int* count, int page_index, node* indexes, int indexes_size, bool* found, int* page_position){
	if(position>=indexes_size){
		if(*found == false && *count == page_index){
			*found = true;
			*page_position = position;
		}
		*count += 1;
	} else if(!(*found)){
		find_page((indexes+position)->left_son,count, page_index, indexes, indexes_size, found, page_position);
		find_page((indexes+position)->right_son,count, page_index, indexes, indexes_size, found, page_position);
	}
}

//Print the content of the page and all the pages linked to it
void print_page(int page_node_index){
	FILE *f;
	node n;
	int number;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	fseek(f,0,SEEK_SET);
	fread(&number,sizeof(int),1,f);
	fseek(f,2*sizeof(int)+page_node_index*sizeof(node),SEEK_SET);
	fread(&n,sizeof(node),1,f);
	page p = n.p;
	bool stop_print = false;
	while(!stop_print){
		for(int i = 0; i<p.qty;i++)printf("%s\n%s\n%u\n%s\n",p.records[i].data.name,p.records[i].data.title,p.records[i].data.year,p.records[i].data.file);
		if(p.linked_page !=-1){
			fseek(f,2*sizeof(int)+number*sizeof(node)+p.linked_page*sizeof(page),SEEK_SET);
			fread(&p,sizeof(page),1,f);
		} else stop_print = true;
	}
	fclose(f);
}


//Print page information for debug
void print_page_debug(page p){
	printf("PAGE - QTY:%d, LINKED PAGE: %d, RECORDS INFO: ",p.qty,p.linked_page);
	for(int i = 0; i<p.qty;i++)printf("(%s,%u) ",p.records[i].data.name,p.records[i].data.year);
	printf("\n");
}


//for debug purposes only
void print_node(node n){
	if(!n.is_page){
		if(n.level%2!=0) printf("LEVEL: %d, IS PAGE?: %d, LEFT SON: %d, RIGHT SON: %d, VALUE: %s\n",n.level,n.is_page,n.left_son,n.right_son,n.name);
		else printf("LEVEL: %d, IS PAGE?: %d, LEFT SON: %d, RIGHT SON: %d, VALUE: %d\n",n.level,n.is_page,n.left_son,n.right_son,n.year);
	} else{
		printf("LEVEL: %d, IS PAGE?: %d, LEFT SON: %d, RIGHT SON: %d PAGE: ",n.level,n.is_page,n.left_son,n.right_son);
		print_page_debug(n.p);
	}
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
	fclose(f);
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
		print_page_debug(p);
	}
	fclose(f);
}

//Find the page with the correspondent index
void find_node_simple(FILE* f, node* actual, bool* found, char name[MAXNAMESIZE]){
	if(actual->is_page){
		for(int i = 0; i<actual->p.qty;i++){
			if(strcmp(name, actual->p.records[i].data.name) == 0){
				printf("nome: %s\n", actual->p.records[i].data.name);
				printf("%s\n", actual->p.records[i].data.title);
				printf("%u\n", actual->p.records[i].data.year);
				printf("%s\n", actual->p.records[i].data.file);
				*found = true;
			}
		}
	}
	
	node son;
	if(actual->level%2!=0){
		if(strcmp(name, (actual)->name) <= 0) {
			if(actual->left_son != -1){
				fseek(f,2*sizeof(int) + (actual->left_son)*sizeof(node),SEEK_SET);
				fread(&son,sizeof(node),1,f);
				find_node_simple(f, &son, found, name);
			}
		}
		else if(strcmp(name, actual->name) > 0){
			if(actual->right_son != -1){
				fseek(f,2*sizeof(int) + (actual->right_son)*sizeof(node),SEEK_SET);
				fread(&son,sizeof(node),1,f);
				find_node_simple(f, &son, found,name);
			}
		}
	}
	else{
		if(actual->left_son != -1){
			fseek(f,2*sizeof(int) + (actual->left_son)*sizeof(node),SEEK_SET);
			fread(&son,sizeof(node),1,f);
			find_node_simple(f, &son, found,name);
		}
		if(actual->right_son != -1){
			fseek(f,2*sizeof(int) + (actual->right_son)*sizeof(node),SEEK_SET);
			fread(&son,sizeof(node),1,f);
			find_node_simple(f, &son, found, name);
		}		
	}
}

void simple_search_name(char name[MAXNAMESIZE]){
	bool found = false;
	FILE *f;
	if(!(f = fopen(MAIN_FILE,"rb+"))) exit(-1);
	//Pega a raiz
	fseek(f,2*sizeof(int) + 0*sizeof(node),SEEK_SET);
	node root;
	fread(&root,sizeof(node),1,f);
	find_node_simple(f, &root, &found, name);
	if(!found)
		printf("nao foi encontrado registro com nome: %s\n", name);
	fclose(f);
}