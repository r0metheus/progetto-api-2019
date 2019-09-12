#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX     64
#define CMD_MAX 7

typedef struct history {
		unsigned int num;
		char id_rel[MAX];
		bool valid;
		struct history *next;
} historyNode_t;

typedef struct node {
		char id_dest[MAX];
		char id_rel[MAX];
		bool valid;
		struct node *next;
} node_t;

typedef struct vertex {
		char id_ent[MAX];
		struct vertex *next;
		struct node *head_node;
		struct history *head_history;
} vertex_t;

typedef struct chrono {
		char id_rel[MAX];
		struct chrono *next;
} chrono_t;

/* CONSTRUCTORS */
vertex_t *new_vertex(char *id_ent, vertex_t *next);                                     // Allocates memory and returns pointer to a new vertex

node_t *new_node(char *id_rel, char *id_dest, node_t *next);                            // Allocates memory and returns pointer to a new relation node

historyNode_t *new_historyNode(char *id_rel, historyNode_t *next, unsigned int num);    // Allocates memory and returns pointer to a new history node

chrono_t *new_chrono(char *id_rel, chrono_t *next);                                     // Allocates memory and returns pointer to a new chrono item


/* UTILITIES */
bool vertexExists(vertex_t **head, char *id_ent);                                       // True if id_ent already exists, false if not

vertex_t *returnVertex(vertex_t **head, char *id_ent);                                  // Returns pointer to id_ent vertex, NULL if id_ent doesn't exist

bool validRelationExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);  // True if a valid relation already exists, false if not

node_t *returnValidRelation(vertex_t **head, char *father, char *id, char *id_rel);     // Returns pointer to a valid node (relation), NULL if that relation doesn't exist

bool inChronoExists(chrono_t **chrono_head, char *id_rel);                              // True if a relation has been inserted before

void addInChrono(chrono_t **chrono_head, char *id_rel);                                 // Adds id_rel in chrono structure iff id_rel doesn't exist there

historyNode_t *returnValidHistory(vertex_t *vertex, char *id_rel);                      // Returns pointer to a valid history, NULL if that relation doesn't exist

void updateHistory(vertex_t *vertex, char *id_rel, unsigned int update);                // Updates history of a vertex after addrel/delrel


/* CORE */
void addent(vertex_t **head, char *id_ent);                                             // Adds a vertex to the graph

void delent(vertex_t **head, char *id_ent);                                             // Deletes a vertex from the graph

void addrel(vertex_t **head, chrono_t **chrono_head, char *id_orig,
            char *id_dest, char *id_rel);                                               // Adds an edge between two vertex

void delrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);               // Deletes an edge between two vertex (source and destination)

void report(vertex_t **head, chrono_t **chrono_head);                                   // Does a report

// DEBUG
//void print_graph(vertex_t **head);

int main(int argc, char **argv) {
	char cmd[CMD_MAX];                                                                    // command arg
	char arg0[MAX];                                                                       // input args
	char arg1[MAX];
	char arg2[MAX];
	vertex_t *HEAD = NULL;
	chrono_t *chrono_head = NULL;

	for (;;) {
		scanf("%s", cmd);

		if (strcmp("report", cmd) == 0)
			report(&HEAD, &chrono_head);

		if (strcmp("end", cmd) == 0) {
			break;
		}

		if (strcmp("addent", cmd) == 0) {
			scanf("%s", arg0);
			addent(&HEAD, arg0);
		}

		if (strcmp("delent", cmd) == 0) {
			scanf("%s", arg0);
			delent(&HEAD, arg0);
		}

		if (strcmp("addrel", cmd) == 0) {
			scanf("%s %s %s", arg0, arg1, arg2);
			addrel(&HEAD, &chrono_head, arg0, arg1, arg2);
		}

		if (strcmp("delrel", cmd) == 0) {
			scanf("%s %s %s", arg0, arg1, arg2);
			delrel(&HEAD, arg0, arg1, arg2);
		}

		*cmd = '\0';
		*arg0 = '\0';
		*arg1 = '\0';
		*arg2 = '\0';
	}

	return 0;
}

/* CONSTRUCTORS */
vertex_t *new_vertex(char *id_ent, vertex_t *next) {
	vertex_t *new = malloc(sizeof(vertex_t));

	strcpy(new->id_ent, id_ent);
	new->next = next;
	new->head_node = NULL;
	new->head_history = NULL;

	return new;
}

node_t *new_node(char *id_rel, char *id_dest, node_t *next) {
	node_t *new = malloc(sizeof(node_t));

	strcpy(new->id_dest, id_dest);
	strcpy(new->id_rel, id_rel);
	new->valid = true;
	new->next = next;

	return new;
}

historyNode_t *new_historyNode(char *id_rel, historyNode_t *next, unsigned int num) {
	historyNode_t *new = malloc(sizeof(historyNode_t));

	strcpy(new->id_rel, id_rel);
	new->num = num;
	new->valid = true;
	new->next = next;

	return new;
}

chrono_t *new_chrono(char *id_rel, chrono_t *next) {
	chrono_t *new = malloc(sizeof(chrono_t));

	strcpy(new->id_rel, id_rel);
	new->next = next;

	return new;
}

/* UTILITIES */
bool vertexExists(vertex_t **head, char *id_ent) {
	vertex_t *current_vertex;

	for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
		if (strcmp(current_vertex->id_ent, id_ent) == 0)
			return true;

	return false;
}

vertex_t *returnVertex(vertex_t **head, char *id_ent) {
	vertex_t *current_vertex;

	for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
		if (strcmp(current_vertex->id_ent, id_ent) == 0)
			return current_vertex;

	return NULL;
}

node_t *returnValidRelation(vertex_t **head, char *father, char *id, char *id_rel) {
	vertex_t *current_vertex = returnVertex(head, father);
	node_t *current_node;

	if (current_vertex == NULL)
		return NULL;

	else {
		for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
			if (current_node->valid && strcmp(current_node->id_dest, id) == 0 && strcmp(current_node->id_rel, id_rel) == 0)
				return current_node;

		return NULL;
	}
}

bool validRelationExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {
	vertex_t *current_vertex;
	node_t *current_node;

	current_vertex = returnVertex(head, id_orig);

	if (current_vertex == NULL || current_vertex->head_node == NULL)
		return false;

	else {
		for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
			if (current_node->valid && strcmp(current_node->id_rel, id_rel) == 0 && strcmp(current_node->id_dest, id_dest) == 0)
				return true;

		return false;
	}
}

bool inChronoExists(chrono_t **chrono_head, char *id_rel) {
	chrono_t *chrono;

	for (chrono = *chrono_head; chrono != NULL; chrono = chrono->next)
		if (strcmp(chrono->id_rel, id_rel) == 0)
			return true;

	return false;
}

void addInChrono(chrono_t **chrono_head, char *id_rel) {
	chrono_t *current_chrono = *chrono_head, *new;

	if (inChronoExists(chrono_head, id_rel))
		return;

	else {
		if (current_chrono == NULL || strcmp(current_chrono->id_rel, id_rel) > 0)
			*chrono_head = new_chrono(id_rel, current_chrono);

		else {
			for (current_chrono = *chrono_head;
			     current_chrono->next != NULL &&
			     strcmp(current_chrono->next->id_rel, id_rel) <= 0;
			     current_chrono = current_chrono->next);

			new = new_chrono(id_rel, current_chrono->next);
			current_chrono->next = new;
		}
	}
}

historyNode_t *returnValidHistory(vertex_t *vertex, char *id_rel) {
	historyNode_t *current_history;

	for (current_history = vertex->head_history; current_history != NULL; current_history = current_history->next)
		if (current_history->valid && current_history->num > 0 && strcmp(current_history->id_rel, id_rel) == 0)
			return current_history;

	return NULL;
}

void updateHistory(vertex_t *vertex, char *id_rel, unsigned int update) {
	historyNode_t *current_history = returnValidHistory(vertex, id_rel);

	if (current_history != NULL) {
		if (update == 1) {
			current_history->num++;
			return;
		}

		if (update == 0) {
			if (current_history->num == 1) {
				current_history->valid = false;
				return;
			} else {
				current_history->num--;
				return;
			}
		}
	} else {
		if (update == 0)
			return;

		else if (update == 1) {
			if (vertex->head_history == NULL) {
				vertex->head_history = new_historyNode(id_rel, NULL, update);

				return;
			}

			for (current_history = vertex->head_history;
			     current_history->next != NULL &&
			     current_history->valid;
			     current_history = current_history->next);

			if (current_history->next == NULL) {
				current_history->next = new_historyNode(id_rel, NULL, update);

				return;
			} else if (current_history->valid == false) {
				strcpy(current_history->id_rel, id_rel);
				current_history->num = update;
				current_history->valid = true;

				return;
			}
		}
	}
}

/* CORE */
void addent(vertex_t **head, char *id_ent) {
	vertex_t *current_vertex = *head, *new;

	if (vertexExists(head, id_ent))
		return;

	else {
		if (*head == NULL || strcmp((*head)->id_ent, id_ent) > 0)
			*head = new_vertex(id_ent, current_vertex);

		else {
			for (current_vertex = *head;
			     current_vertex->next != NULL &&
			     strcmp(current_vertex->next->id_ent, id_ent) <= 0;
			     current_vertex = current_vertex->next);

			new = new_vertex(id_ent, current_vertex->next);
			current_vertex->next = new;
		}
	}
}

void delent(vertex_t **head, char *id_ent) {
	vertex_t *current_vertex, *toDelete, *toDelete_prev;
	historyNode_t *current_history;
	node_t *current_node;
	toDelete = returnVertex(head, id_ent);

	if (*head == NULL || toDelete == NULL)
		return;

	else {
		// Deletes every edge towards the vertex to be deleted
		for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
			for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
				if (strcmp(current_node->id_dest, id_ent) == 0)
					current_node->valid = false;

		// Deletes every edge from the vertex to be deleted
		for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
			for (current_history = current_vertex->head_history; current_history != NULL; current_history = current_history->next)
				delrel(head, toDelete->id_ent, current_vertex->id_ent, current_history->id_rel);

		// Deletes vertex
		for (current_vertex = *head; current_vertex != NULL && strcmp(current_vertex->id_ent, id_ent) != 0; current_vertex = current_vertex->next)
			toDelete_prev = current_vertex;

		if (*head == toDelete)
			*head = toDelete->next;

		else
			toDelete_prev->next = toDelete->next;

		free(toDelete);
	}
}

void addrel(vertex_t **head, chrono_t **chrono_head, char *id_orig, char *id_dest, char *id_rel) {
	if (validRelationExists(head, id_orig, id_dest, id_rel))
		return;

	vertex_t *current_vertex, *toBeUpdated;
	node_t *current_node;

	current_vertex = returnVertex(head, id_orig);
	toBeUpdated = returnVertex(head, id_dest);

	if (current_vertex == NULL || toBeUpdated == NULL)
		return;

	addInChrono(chrono_head, id_rel);

	updateHistory(toBeUpdated, id_rel, 1);

	if (current_vertex->head_node == NULL) {
		current_vertex->head_node = new_node(id_rel, id_dest, NULL);
	}

	else {
		for (current_node = current_vertex->head_node;
		     current_node->next != NULL && current_node->valid;
		     current_node = current_node->next);

		if (current_node->next == NULL && current_node->valid) {
			current_node->next = new_node(id_rel, id_dest, NULL);

			return;
		}

		else if (!current_node->valid) {
			strcpy(current_node->id_dest, id_dest);
			strcpy(current_node->id_rel, id_rel);
			current_node->valid = true;

			return;
		}
	}
}

void delrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {
	if (!validRelationExists(head, id_orig, id_dest, id_rel))
		return;

	vertex_t *toBeUpdated = returnVertex(head, id_dest);;
	node_t *current_node = returnValidRelation(head, id_orig, id_dest, id_rel);

	if (toBeUpdated == NULL || current_node == NULL)
		return;

	updateHistory(toBeUpdated, id_rel, 0);
	current_node->valid = false;
}

void report(vertex_t **head, chrono_t **chrono_head) {
	vertex_t *current_vertex;
	historyNode_t *current_history;
	chrono_t *current_chrono;
	register unsigned int relation_c = 0;
	register unsigned int max = 0;

	if (*chrono_head != NULL) {
		for (current_vertex = *head; current_vertex != NULL && relation_c == 0; current_vertex = current_vertex->next)
			for (current_history = current_vertex->head_history; current_history != NULL; current_history = current_history->next)
				if (current_history->valid)
					relation_c++;

		if (relation_c == 0) {
			fputs("none\n", stdout);

			return;
		}

		for (current_chrono = *chrono_head; current_chrono != NULL; current_chrono = current_chrono->next) {

			for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
				for (current_history = current_vertex->head_history; current_history != NULL; current_history = current_history->next)
					if (current_history->num > max && current_history->valid && strcmp(current_history->id_rel, current_chrono->id_rel) == 0)
						max = current_history->num;

			if (max == 0)
				continue;

			printf("%s ", current_chrono->id_rel);
			for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
				for (current_history = current_vertex->head_history; current_history != NULL; current_history = current_history->next)
					if (current_history->num == max && current_history->valid && strcmp(current_history->id_rel, current_chrono->id_rel) == 0)
						printf("%s ", current_vertex->id_ent);

			printf("%d; ", max);

			max = 0;
		}

		puts("");

	} else
		fputs("none\n", stdout);

}

// DEBUG

/*void print_graph(vertex_t **head) {
	vertex_t *current_vertex;
	node_t *current_node;
	historyNode_t *current_history;

	if (*head == NULL)
		puts("\nVoid graph\n");

	for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next) {
		printf("%s:\n", current_vertex->id_ent);

		printf("    history for %s\n", current_vertex->id_ent);
		for (current_history = current_vertex->head_history;
		     current_history != NULL; current_history = current_history->next)
			printf("        - id_rel: %s; num: %d; valid: %d;\n", current_history->id_rel, current_history->num,
			       current_history->valid);

		printf("    nodes for %s\n", current_vertex->id_ent);
		for (current_node = current_vertex->head_node;
		     current_node != NULL; current_node = current_node->next)
			printf("        - id_rel: %s; id_dest: %s; valid: %d; \n", current_node->id_rel, current_node->id_dest,
			       current_node->valid);
	}
}*/
