#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex{
	char id_ent[255];
	struct node *next_node;
	struct vertex *next_vertex;
} vertex_t;

typedef struct node{
	char id_orig[255];
	char id_dest[255];
	char id_rel[255];
	struct node *next_node;
} node_t;

void addent(char* id_ent);
void delent(char* id_ent);
void addrel(char* id_orig, char* id_dest, char* id_rel);
void delrel(char* id_orig, char* id_dest, char* id_rel);
void report();





bool check_if_exists(char* id_ent);
bool check_if_rel_exists(char* id_orig, char* id_dest, char* id_rel);

void print_graph();
void delete_nodes(char* id);












vertex_t* HEAD = NULL;

/* MAIN */
int main (int argc, char** argv){
	char cmd[7];
	char arg0[255];
	char arg1[255];
	char arg2[255];

	for(;;){
		scanf("%s", cmd);

		if (strncmp("report", cmd, 6) == 0)
			report();

		if (strncmp("end", cmd, 6) == 0){
			break;
		}

		if (strncmp("addent", cmd, 6) == 0){
			scanf("%s", arg0);
			addent(arg0);
		}

		if (strncmp("delent", cmd, 6) == 0){
			scanf("%s", arg0);
			delent(arg0);
		}

		if (strncmp("addrel", cmd, 6) == 0){
			scanf("%s %s %s", arg0, arg1, arg2);
			addrel(arg0, arg1, arg2);
		}

		if (strncmp("delrel", cmd, 6) == 0){
			scanf("%s %s %s", arg0, arg1, arg2);
			delrel(arg0, arg1, arg2);
		}

	strncpy(cmd, "", 7);
	strncpy(arg0, "", 255);
	strncpy(arg1, "", 255);
	strncpy(arg2, "", 255);

	}

	return 0;
}

void addent(char* id_ent){
	vertex_t *new_vertex = NULL;
	new_vertex = malloc(sizeof(vertex_t));
	new_vertex->next_node = NULL;
	strcpy(new_vertex->id_ent, id_ent);

	vertex_t *tmp;

	/* Creating the first node in the graph */
	if(HEAD == NULL){
		HEAD = new_vertex;
	}

	/* Looping through the graph and adding the node */
	else{

		for(tmp = HEAD; tmp->next_vertex != NULL; tmp = tmp->next_vertex);

		new_vertex->next_vertex = tmp->next_vertex;
		tmp->next_vertex = new_vertex;

	}

	return;
}

bool check_if_exists(char* id_ent){
	vertex_t *tmp;

	for(tmp = HEAD; tmp!=NULL; tmp = tmp->next_vertex)
		if(strcmp(tmp->id_ent, id_ent)==0)
			return true;

	return false;
}

bool check_if_rel_exists(char* id_orig, char* id_dest, char* id_rel){
	vertex_t *tmp_vertex;
	node_t *tmp_node;

	for(tmp_vertex = HEAD; strcmp(tmp_vertex->id_ent, id_orig)!=0 && tmp_vertex->next_vertex!=NULL; tmp_vertex = tmp_vertex->next_vertex);

	tmp_node=tmp_vertex->next_node;

	while(tmp_node != NULL){
		if(strcmp(tmp_node->id_rel, id_rel)==0 && strcmp(tmp_node->id_dest, id_dest)==0)
			return true;

		tmp_node = tmp_node->next_node;
	}

	return false;

}

void delent(char* id_ent){
	vertex_t *tmp = HEAD;
	vertex_t *tmp_;

	if(strcmp(tmp->id_ent, id_ent)==0){
		if(tmp->next_node!=NULL)
			delete_nodes(id_ent);
		HEAD = tmp->next_vertex;
		free(tmp);
	}

	else{

		for(; strcmp(tmp->id_ent, id_ent)!=0; tmp = tmp->next_vertex)
			tmp_ = tmp;

		if(tmp->next_node!=NULL)
			delete_nodes(id_ent);
		tmp_->next_vertex = tmp->next_vertex;
		free(tmp);

	}

	return;
}

void addrel(char* id_orig, char* id_dest, char* id_rel){
	
	if(check_if_exists(id_orig)==false){
		printf("id orig non esiste");
		return;
	}

	if(check_if_exists(id_dest)==false){
		printf("id dest non esiste");
		return;
	}

	if(check_if_rel_exists(id_orig, id_dest, id_rel)==true)
		return;

	vertex_t *tmp_vertex;
	
	node_t *new_node;
	new_node = malloc(sizeof(node_t));
	strcpy(new_node->id_orig, id_orig);
	strcpy(new_node->id_dest, id_dest);
	strcpy(new_node->id_rel, id_rel);
	new_node->next_node = NULL;



	for(tmp_vertex = HEAD; strcmp(tmp_vertex->id_ent, id_orig)!=0; tmp_vertex = tmp_vertex->next_vertex);

	if(tmp_vertex->next_node==NULL){
		tmp_vertex->next_node = new_node;

		return;
	}

	else{
		new_node->next_node = tmp_vertex->next_node;
		tmp_vertex->next_node = new_node;

		return;
	}

}

void delrel(char* id_orig, char* id_dest, char* id_rel){
	vertex_t *tmp_vertex;
	node_t *tmp, *tmp_;

	for(tmp_vertex = HEAD; strcmp(tmp_vertex->id_ent, id_orig)!=0; tmp_vertex = tmp_vertex->next_vertex);

	tmp = tmp_vertex->next_node;
	tmp_ = tmp;

	if(tmp == NULL)
		return;


	while(tmp->next_node!=NULL && strcmp(tmp->id_dest, id_dest)!=0)
		tmp_ = tmp;

	if(strcmp(tmp->id_rel, id_rel)==0){
		tmp_->next_node = tmp->next_node;
		free(tmp);

		return;
	}

	else{
		return;
	}

}

void report(){
	print_graph();
}

void delete_nodes(char* id){
	return;

}


void print_graph(){
	vertex_t *tmp;
	node_t *tmp_node;

	printf("#########################################\n");

	for(tmp = HEAD; tmp!=NULL; tmp = tmp->next_vertex){
		printf("%s: \n", tmp->id_ent);
		for(tmp_node = tmp->next_node; tmp_node != NULL; tmp_node = tmp_node->next_node)
			printf("	id_orig: %s; id_rel: %s; id_dest: %s; \n", tmp_node->id_orig, tmp_node->id_rel, tmp_node->id_dest);

	}

	printf("#########################################\n");
	
	return;
}