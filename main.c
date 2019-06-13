#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex{
	char id_ent[255];
	struct node *next_node;
	struct vertex *next_vertex;
	struct vertex *prev_vertex;
} vertex_t;

typedef struct node{
	char id_orig[255];
	char id_dest[255];
	char id_rel[255];
	struct node *next_node;
	struct node *prev_node;
} node_t;
















void addent(char* id_ent);
void delent(char* id_ent);
void addrel(char* id_orig, char* id_dest, char* id_rel);
void delrel(char* id_orig, char* id_dest, char* id_rel);
void report();













void add_vertex(char* id);
void print_vertices();
void delete_vertex(char* id);














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
			print_vertices();
			//break;
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
	add_vertex(id_ent);

	return;
}

void delent(char* id_ent){
	delete_vertex(id_ent);
}

void addrel(char* id_orig, char* id_dest, char* id_rel){
	printf("I'm addrel, arg0, arg1 and arg2 are: %s, %s and %s\n", id_orig, id_dest, id_rel);
}
void delrel(char* id_orig, char* id_dest, char* id_rel){
	printf("I'm delrel, arg0, arg1 and arg2 are: %s, %s and %s\n", id_orig, id_dest, id_rel);
}

void report(){
	printf("I'm report, I have no args by default\n");
}

void add_vertex(char* id){
	vertex_t *new_vertex = NULL;
	new_vertex = malloc(sizeof(vertex_t));
	new_vertex->next_vertex = NULL;
	new_vertex->next_node = NULL;
	new_vertex->prev_vertex = NULL;
	strcpy(new_vertex->id_ent, id);

	vertex_t *tmp = HEAD;
	vertex_t *tmp_2 = tmp->prev_vertex;


	/* Creating the first node in the graph */
	if(HEAD == NULL){
		HEAD = new_vertex;
		return;
	}

	/* Looping through the graph and adding the node */
	else{
		while(tmp->next_vertex != NULL){														// && strcmp(tmp->id_ent, id)<=0
			tmp = tmp->next_vertex;
			tmp_2 = tmp->prev_vertex;
		}

		tmp->prev_vertex = tmp_2;
		tmp->next_vertex = new_vertex;


		return;
	}

}

void delete_vertex(char* id){
	vertex_t *tmp_vertex = HEAD;
	node_t *tmp_node_1 = NULL;
	node_t *tmp_node_2 = NULL;

	while(!strcmp(tmp_vertex->id_ent, id))
		tmp_vertex = tmp_vertex->next_vertex;

	if(tmp_vertex->next_node != NULL){
		tmp_node_1 = tmp_vertex->next_node;
		tmp_node_2 = tmp_node_1->next_node;

		while(tmp_node_2->next_node != NULL){
			tmp_node_1 = tmp_node_1->next_node;
			tmp_node_2 = tmp_node_1->next_node;
		}

		while(tmp_node_1->prev_node != NULL){
			free(tmp_node_2);
			tmp_node_2 = tmp_node_1;
			tmp_node_1 = tmp_node_1->prev_node;
		}
	}

	free(tmp_vertex);

}


void print_vertices(){
	vertex_t *tmp = HEAD;

	while(tmp != NULL){
		printf("%s\n", tmp->id_ent);
		tmp = tmp->next_vertex;
	}

	return;
}