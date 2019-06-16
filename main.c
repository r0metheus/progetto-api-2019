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

typedef struct relation{
	char id_rel[255];
	struct relation *next;
} relation_t;




void addent(char* id_ent);
void delent(char* id_ent);
void addrel(char* id_orig, char* id_dest, char* id_rel);
void delrel(char* id_orig, char* id_dest, char* id_rel);
void report();

bool idExists(char* id_ent);
bool relExists(char* id_orig, char* id_dest, char* id_rel);

void print_graph();
void delete_nodes(char* id_dest, vertex_t *vertex);
bool relationExistsInHistory(char *id_rel);
void addrel_to_history(char *id_rel);
void print_relations();










vertex_t *HEAD = NULL;
relation_t *ROOT = NULL;

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

	// Bisognerebbe fare l'inserimento in ordine lessicografico
	vertex_t *new_vertex = NULL;
	new_vertex = malloc(sizeof(vertex_t));
	new_vertex->next_node = NULL;
	strcpy(new_vertex->id_ent, id_ent);

	vertex_t *tmp;

	if(idExists(id_ent))
		return;

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

bool idExists(char* id_ent){
	vertex_t *tmp;

	for(tmp = HEAD; tmp!=NULL; tmp = tmp->next_vertex)
		if(strcmp(tmp->id_ent, id_ent)==0)
			return true;

	return false;
}

bool relExists(char* id_orig, char* id_dest, char* id_rel){
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
	vertex_t *tmp_vertex1, *tmp_vertex2;

	if(strcmp(HEAD->id_ent, id_ent)==0){
		tmp_vertex1 = HEAD;
		HEAD = tmp_vertex1->next_vertex;
		free(tmp_vertex1);
	}

	else{

		for(tmp_vertex1 = HEAD; tmp_vertex1!=NULL && strcmp(tmp_vertex1->id_ent, id_ent)!=0; tmp_vertex1 = tmp_vertex1->next_vertex)
			tmp_vertex2 = tmp_vertex1;
		tmp_vertex2->next_vertex = tmp_vertex1->next_vertex;
		free(tmp_vertex1);

	}

	for(tmp_vertex1 = HEAD; tmp_vertex1!=NULL; tmp_vertex1 = tmp_vertex1->next_vertex){
		delete_nodes(id_ent, tmp_vertex1);
	}

	return;
}

void addrel(char* id_orig, char* id_dest, char* id_rel){
	
	if(!idExists(id_orig)){
		return;
	}

	if(!idExists(id_dest)){
		return;
	}

	if(relExists(id_orig, id_dest, id_rel))
		return;

	if(!relationExistsInHistory(id_rel))
		addrel_to_history(id_rel);

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
	if(relExists(id_orig, id_dest, id_rel)==false)
		return;

	else{
		vertex_t *tmp_vertex;
		node_t *tmp_node1, *tmp_node2;

		for(tmp_vertex = HEAD; tmp_vertex->next_vertex!=NULL && strcmp(tmp_vertex->id_ent, id_orig)!=0; tmp_vertex = tmp_vertex->next_vertex);
		
		tmp_node1 = tmp_vertex->next_node;

		if(strcmp(tmp_node1->id_dest, id_dest)==0 && strcmp(tmp_node1->id_rel, id_rel)==0){
			tmp_vertex->next_node = tmp_node1->next_node;
			free(tmp_node1);
		}

		else{
			for(; tmp_node1->next_node!=NULL && strcmp(tmp_node1->id_dest, id_dest)!=0 &&strcmp(tmp_node1->id_rel, id_rel)!=0; tmp_node1 = tmp_node1->next_node)
				tmp_node2=tmp_node1;

			tmp_node2->next_node=tmp_node1->next_node;
			free(tmp_node1);
		}	

	}
}

void addrel_to_history(char *id_rel){

// INSERIRE IN ORDINE LESSICOGRAFICO LE RELAZIONI

}

bool relationExistsInHistory(char *id_rel){
	relation_t *tmp_relation;

	for(tmp_relation = ROOT; tmp_relation!=NULL; tmp_relation = tmp_relation->next){
		if (strcmp(tmp_relation->id_rel, id_rel)==0)
			return true;
	}

	return false;
}

void report(){
	vertex_t *tmp_none;
	relation_t *tmp_relation = ROOT;
	int relation_c = 0;



	for(tmp_none=HEAD; tmp_none!=NULL; tmp_none = tmp_none->next_vertex)
		if(tmp_none->next_node!=NULL)
			relation_c++;

	if(relation_c==0)
		printf("none\n");
	print_relations();


}

void print_relations(){
	relation_t *tmp_relation;
	printf("\n");

	for(tmp_relation = ROOT; tmp_relation!=NULL; tmp_relation = tmp_relation->next){
		printf("# %s #", tmp_relation->id_rel);
	}

}

void delete_nodes(char* id_dest, vertex_t *vertex){
	vertex_t *tmp_vertex = vertex;
	node_t *tmp, *tmp_;

	tmp = tmp_vertex->next_node;

	if(tmp_vertex->next_node==NULL)
		return;

	if(tmp->next_node==NULL && strcmp(tmp->id_dest, id_dest)==0){
		tmp_vertex->next_node = NULL;
		free(tmp);
	}

	while(tmp->next_node!=NULL){

		if(strcmp(tmp->id_dest, id_dest)==0){
			tmp_->next_node=tmp->next_node;
			free(tmp);
		}

		tmp_ = tmp;
	}


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