#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define MAX 255

typedef struct vertex{
	char id_ent[255];
	struct node *next_node;
	struct vertex *next;
	struct vertex *prev;
} vertex_t;

typedef struct node{
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

vertex_t *new_vertex(char *id_ent, vertex_t *next, vertex_t *prev);
node_t *new_node(char *id_rel, char *id_dest, node_t *next, node_t *prev);

bool vertexExists(char* id_ent);
bool relExists(char* id_orig, char* id_dest, char* id_rel);
bool relationExistsInHistory(char *id_rel);
void addrel_to_history(char *id_rel);
int numRelPerEntities(char *id_dest, char * id_rel);
void sortHistory();






//DEBUG
void print_graph();
void print_relations();




vertex_t *HEAD = NULL;
char relationsHistory[MAX][MAX];
int relationCounter = 0;

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

vertex_t *new_vertex(char *id_ent, vertex_t *next, vertex_t *prev){										// DO NOT TOUCH
	vertex_t *new_vertex = malloc(sizeof(vertex_t));

	strcpy(new_vertex->id_ent, id_ent);
	new_vertex->next_node = NULL;
	new_vertex->next = next;
	new_vertex->prev = prev;

	return new_vertex;
}

node_t *new_node(char *id_rel, char *id_dest, node_t *next, node_t *prev){										// DO NOT TOUCH
	node_t *new_node = malloc(sizeof(node_t));

	strcpy(new_node->id_dest, id_dest);
	strcpy(new_node->id_rel, id_rel);
	new_node->next_node = next;
	new_node->prev_node = prev;

	return new_node;
}

void addent(char* id_ent){				// DO NOT TOUCH

	vertex_t *current;

	if(vertexExists(id_ent))
		return;

	if(HEAD==NULL)
		HEAD = new_vertex(id_ent, NULL, NULL);

	else{

		for(current = HEAD; current->next!=NULL; current = current->next);
		current->next = new_vertex(id_ent, NULL, current);
	}
}

bool vertexExists(char* id_ent){				// DO NOT TOUCH
	vertex_t *current;

	for(current = HEAD; current!=NULL; current = current->next)
		if(strcmp(current->id_ent, id_ent)==0)
			return true;

	return false;
}

bool relExists(char* id_orig, char* id_dest, char* id_rel){				// DO NOT TOUCH
	vertex_t *current;
	node_t *current_node;

	for(current = HEAD; current!=NULL; current = current->next){
		if(strcmp(current->id_ent, id_orig)==0)
			break;
	}

	if(current==NULL)
		return false;

	current_node = current->next_node;

	if(current_node==NULL)
		return false;

	else{
		for(current_node = current->next_node; current_node!=NULL; current_node = current_node->next_node)
			if(strcmp(current_node->id_rel, id_rel)==0 && strcmp(current_node->id_dest, id_dest)==0)
				return true;

		return false;
	}
}

void addrel_to_history(char *id_rel){									// DO NOT TOUCH

	strcpy(relationsHistory[relationCounter], id_rel);
	relationCounter++;

	sortHistory();

	return;

}

void sortHistory(){														// DO NOT TOUCH
	char tmp[MAX];
	int i, j;

	for(i=0; i<relationCounter; i++){
		for(j=0; j<relationCounter-i-1; j++){
			if(strcmp(relationsHistory[j], relationsHistory[j+1])>=0){
				strcpy(tmp, relationsHistory[j]);
				strcpy(relationsHistory[j], relationsHistory[j+1]);
				strcpy(relationsHistory[j+1], tmp);

			}

		}
	}

}

bool relationExistsInHistory(char *id_rel){								// DO NOT TOUCH
	int i;

	for(i=0; i<relationCounter; i++)
		if(strcmp(relationsHistory[i], id_rel)==0)
			return true;

	return false;

}

void addrel(char* id_orig, char* id_dest, char* id_rel){				// DO NOT TOUCH

	if(!vertexExists(id_orig)){
		return;
	}

	if(!vertexExists(id_dest)){
		return;
	}

	if(relExists(id_orig, id_dest, id_rel))
		return;

	if(!relationExistsInHistory(id_rel))
		addrel_to_history(id_rel);

	vertex_t *current_vertex;

	node_t *current_node;

	for(current_vertex = HEAD; strcmp(current_vertex->id_ent, id_orig)!=0; current_vertex = current_vertex->next);

	if(current_vertex->next_node==NULL){
		current_vertex->next_node = new_node(id_rel, id_dest, NULL, NULL);

		return;
	}

	else{
		for(current_node = current_vertex->next_node; current_node->next_node!=NULL; current_node = current_node->next_node);

		current_node->next_node = new_node(id_rel, id_dest, NULL, current_node);

		return;
	}

}

void delrel(char* id_orig, char* id_dest, char* id_rel){				// DO NOT TOUCH
	if(!relExists(id_orig, id_dest, id_rel)){
		return;
	}

	else{
		vertex_t *current_vertex;
		node_t *current_node, *tmp;

		for(current_vertex = HEAD; strcmp(current_vertex->id_ent, id_orig)!=0; current_vertex = current_vertex->next);

		current_node = current_vertex->next_node;

		if(current_node->next_node==NULL && current_node->prev_node==NULL && strcmp(current_node->id_dest, id_dest)==0 && strcmp(current_node->id_rel, id_rel)==0){
			free(current_node);
			current_vertex->next_node = NULL;

			return;
		}

		else{

			for(current_node = current_vertex->next_node; current_node!=NULL; current_node = current_node->next_node){
				if(strcmp(current_node->id_dest, id_dest)==0 && strcmp(current_node->id_rel, id_rel)==0)
					break;
			}

			tmp = current_node;
			if(current_node->prev_node==NULL){
				current_vertex->next_node = current_node->next_node;
				(current_node->next_node)->prev_node = NULL;

				free(tmp);
				return;
			}

			else{

				if(current_node->next_node==NULL){
					(current_node->prev_node)->next_node = NULL;
					free(tmp);

					return;

				}

				else{

				(current_node->prev_node)->next_node = tmp->next_node;
				(current_node->next_node)->prev_node = tmp->prev_node;
				free(tmp);

				return;
			}
			}

		}


	}

}

void delent(char* id_ent){								// DO NOT TOUCH
	vertex_t *current = HEAD, *tmp;
	int i;

	if(!vertexExists(id_ent))
		return;

	for(current = HEAD; current!=NULL && strcmp(current->id_ent, id_ent)!=0; current = current->next);

	if(current==HEAD){
		HEAD = current->next;
		HEAD->prev = NULL;

		free(current);

	}

	else if(current==HEAD && current->next==NULL){
		free(current);
		HEAD = NULL;

	}

	else if(current->next==NULL){
		tmp = current;
		current->prev->next = NULL;

		free(tmp);

	}

	else{
		tmp = current;
		current->prev->next = current->next;
		current->next->prev = current->prev;

		free(tmp);

	}

	for(current = HEAD; current!=NULL; current = current->next){
		for(i=0; i<relationCounter; i++){
			delrel(current->id_ent, id_ent, relationsHistory[i]);
		}
	}

}

int numRelPerEntities(char *id_dest, char * id_rel){
	vertex_t *current_vertex;
	node_t *current_node;
	int counter = 0;

	for(current_vertex = HEAD; current_vertex!=NULL; current_vertex = current_vertex->next){
		for(current_node = current_vertex->next_node; current_node!=NULL; current_node = current_node->next_node){
			if(strcmp(current_node->id_dest, id_dest)==0 && strcmp(current_node->id_rel, id_rel)==0)
				counter++;
		}

	}

	return counter;

}

void report(){
	vertex_t *tmp_none, *current_vertex;
	int relation_c = 0, max = 0, tmp;
	char id_rel[255];
	char entities[MAX][MAX];
	int i, j, m;

	for(tmp_none=HEAD; tmp_none!=NULL; tmp_none = tmp_none->next)
		if(tmp_none->next_node!=NULL)
			relation_c++;

	if(relation_c==0)
		printf("none");

	for(i=0; i<relationCounter; i++){
		strcpy(id_rel, relationsHistory[i]);

		for(current_vertex = HEAD; current_vertex!=NULL; current_vertex = current_vertex->next){
			tmp = numRelPerEntities(current_vertex->id_ent, relationsHistory[i]);
			if(tmp>=max)
				max = tmp;

		}


		for(current_vertex = HEAD; current_vertex!=NULL; current_vertex = current_vertex->next){
			tmp = numRelPerEntities(current_vertex->id_ent, relationsHistory[i]);
			if(tmp==max && tmp!=0){
				strcpy(entities[j], current_vertex->id_ent);
				j++;
			}
		}

		if(max == 0)
			goto RESET;

		printf("%s ", id_rel);
		for(m=0; m<j; m++)
			printf("%s ", entities[m]);
		printf("%d; ", max);

RESET:


		max = 0;
		tmp = max;
		j=0;

		int k;
		for(k=0; k<MAX; k++)
			strcpy(entities[k], "");


	}
	printf("\n");




	//print_relations();

	//print_graph();


}

void print_relations(){
	int i;
	printf("\n##########\n");
	printf("Relations History: \n");

	for(i=0; i<relationCounter; i++)
		printf("%s, ", *(relationsHistory+i));

}

void print_graph(){
	vertex_t *tmp;
	node_t *tmp_node;

	printf("##########\n");

	for(tmp = HEAD; tmp!=NULL; tmp = tmp->next){
		printf("%s: \n", tmp->id_ent);
		for(tmp_node = tmp->next_node; tmp_node != NULL; tmp_node = tmp_node->next_node)
			printf("	id_rel: %s; id_dest: %s; \n", tmp_node->id_rel, tmp_node->id_dest);

	}

	printf("##########\n");

	return;
}
