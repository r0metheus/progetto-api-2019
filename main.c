#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255
#define CMD_MAX 7

typedef struct history {
    char id_rel[MAX];
    int num;
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
    bool valid;
    struct vertex *next;
    struct node *head_node;
    struct history *head_history;
} vertex_t;

typedef struct chrono {
    char id_rel[MAX];
    struct chrono *next;
} chrono_t;

/* COSTRUTTORI */
vertex_t *new_vertex(char *id_ent, vertex_t *next);

node_t *new_node(char *id_rel, char *id_dest, node_t *next);

historyNode_t *new_historyNode(char *id_rel, historyNode_t *next, int num);

chrono_t *new_chrono(char *id_rel, chrono_t *next);

/* UTILITIES */
bool vertexExists(vertex_t **head, char *id_ent);

bool validRelExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);

bool validHistoryExists(vertex_t *vertex, char *id_rel);

bool inChronoExists(char *id_rel);

void updateHistory(vertex_t *vertex, char *id_rel, int update);

vertex_t *returnVertex(vertex_t **head, char *id_ent);

node_t *returnValidNode(vertex_t **head, char *father, char *id, char *id_rel);

historyNode_t *returnValidHistory(vertex_t *vertex, char *id_rel);

void addInChrono(char *id_rel);

/* CORE */
void addent(vertex_t **head, char *id_ent);

void delent(vertex_t **head, char *id_ent);

void addrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);

void delrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);

void report(vertex_t **head);

// DEBUG

void print_graph(vertex_t **head);

int no_report = 0;

chrono_t *chrono_head = NULL;

int main(int argc, char **argv) {
    char cmd[CMD_MAX];
    char arg0[MAX];
    char arg1[MAX];
    char arg2[MAX];
    vertex_t *HEAD = NULL;

    for (;;) {
        scanf("%s", cmd);

        if (strncmp("report", cmd, CMD_MAX - 1) == 0)
            report(&HEAD);

        if (strncmp("end", cmd, CMD_MAX - 1) == 0) {
            break;
        }

        if (strncmp("addent", cmd, CMD_MAX - 1) == 0) {
            scanf("%s", arg0);
            addent(&HEAD, arg0);
        }

        if (strncmp("delent", cmd, CMD_MAX - 1) == 0) {
            scanf("%s", arg0);
            delent(&HEAD, arg0);
        }

        if (strncmp("addrel", cmd, CMD_MAX - 1) == 0) {
            scanf("%s %s %s", arg0, arg1, arg2);
            addrel(&HEAD, arg0, arg1, arg2);
        }

        if (strncmp("delrel", cmd, CMD_MAX - 1) == 0) {
            scanf("%s %s %s", arg0, arg1, arg2);
            delrel(&HEAD, arg0, arg1, arg2);
        }

        strncpy(cmd, "", CMD_MAX);
        strncpy(arg0, "", MAX);
        strncpy(arg1, "", MAX);
        strncpy(arg2, "", MAX);

    }

    return 0;
}

/* COSTRUTTORI */
vertex_t *new_vertex(char *id_ent, vertex_t *next) {
    vertex_t *new = calloc(1, sizeof(vertex_t));

    strcpy(new->id_ent, id_ent);
    new->next = next;
    new->valid = true;
    new->head_node = NULL;
    new->head_history = NULL;

    return new;

}

node_t *new_node(char *id_rel, char *id_dest, node_t *next) {
    node_t *new = calloc(1, sizeof(node_t));

    strcpy(new->id_dest, id_dest);
    strcpy(new->id_rel, id_rel);
    new->valid = true;
    new->next = next;

    return new;
}

historyNode_t *new_historyNode(char *id_rel, historyNode_t *next, int num) {
    historyNode_t *new = calloc(1, sizeof(historyNode_t));

    strcpy(new->id_rel, id_rel);
    new->num = num;
    new->valid = true;
    new->next = next;

    return new;

}

chrono_t *new_chrono(char *id_rel, chrono_t *next) {
    chrono_t *new = calloc(1, sizeof(chrono_t));

    strcpy(new->id_rel, id_rel);
    new->next = next;

    return new;
}











/* UTILITIES */
bool vertexExists(vertex_t **head, char *id_ent) {
    vertex_t *current_vertex;

    for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
        if (strcmp(current_vertex->id_ent, id_ent) == 0 && current_vertex->valid)
            return true;

    return false;
}

vertex_t *returnVertex(vertex_t **head, char *id_ent) {
    vertex_t *current_vertex;

    for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
        if (strcmp(current_vertex->id_ent, id_ent) == 0 && current_vertex->valid)
            return current_vertex;

    return NULL;
}

node_t *returnValidNode(vertex_t **head, char *father, char *id, char *id_rel) {
    vertex_t *current_vertex = returnVertex(head, father);
    node_t *current_node;

    if (current_vertex == NULL)
        return NULL;

    else {
        for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
            if (strcmp(current_node->id_dest, id) == 0 && strcmp(current_node->id_rel, id_rel) == 0)
                break;

        if (current_node == NULL || current_node->valid == false)
            return NULL;
        else
            return current_node;

    }
}

bool validRelExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {
    vertex_t *current_vertex;
    node_t *current_node;

    current_vertex = returnVertex(head, id_orig);

    if (current_vertex == NULL || current_vertex->head_node == NULL)
        return false;

    else {
        for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
            if (strcmp(current_node->id_rel, id_rel) == 0 && strcmp(current_node->id_dest, id_dest) == 0)
                return current_node->valid;

        return false;
    }

}

bool inChronoExists(char *id_rel) {
    chrono_t *chrono;

    for (chrono = chrono_head; chrono != NULL; chrono = chrono->next)
        if (strcmp(chrono->id_rel, id_rel) == 0)
            return true;

    return false;

}

void addInChrono(char *id_rel) {

    chrono_t *current_chrono = chrono_head, *new;

    if (inChronoExists(id_rel))
        return;

    else {
        if (current_chrono == NULL || strcmp(current_chrono->id_rel, id_rel) > 0)
            chrono_head = new_chrono(id_rel, current_chrono);

        else {
            for (current_chrono = chrono_head; current_chrono->next != NULL && strcmp(current_chrono->next->id_rel, id_rel)<=0; current_chrono = current_chrono->next);

            new = new_chrono(id_rel, current_chrono->next);
            current_chrono->next = new;
        }
    }
}

bool validHistoryExists(vertex_t *vertex, char *id_rel) {
    historyNode_t *current_history;

    for (current_history = vertex->head_history; current_history != NULL; current_history = current_history->next)
        if (strcmp(current_history->id_rel, id_rel) == 0 && current_history->valid && current_history->num>0)
            return true;

    return false;

}

historyNode_t *returnValidHistory(vertex_t *vertex, char *id_rel) {
    historyNode_t *current_history;

    for (current_history = vertex->head_history; current_history != NULL; current_history = current_history->next)
        if (strcmp(current_history->id_rel, id_rel) == 0 && current_history->valid && current_history->num>0)
            break;

    if (current_history == NULL)
        return NULL;
    else
        return current_history;

}











void updateHistory(vertex_t *vertex, char *id_rel, int update) {
    historyNode_t *current_history;

    if (validHistoryExists(vertex, id_rel)) {
        current_history = returnValidHistory(vertex, id_rel);

        if (update == 1) {
            current_history->num += update;
            return;
        }

        if (update == -1) {
            if (current_history->num == 1) {
                current_history->num += update;
                current_history->valid = false;
                return;
            }

            else {
                current_history->num += update;
                return;
            }
        }
    } else {
        if (update == -1)
            return;

        else if (update==1){
            if (vertex->head_history == NULL) {
                vertex->head_history = new_historyNode(id_rel, NULL, update);

                return;
            }

            for (current_history = vertex->head_history; current_history->next != NULL && current_history->valid; current_history = current_history->next);

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
    vertex_t *current_vertex = *head, *next, *new;

    if (vertexExists(head, id_ent))
        return;

    else {
        if (*head == NULL || strcmp((*head)->id_ent, id_ent) > 0)
            *head = new_vertex(id_ent, current_vertex);

        else {
            for (current_vertex = *head; current_vertex->next != NULL && strcmp(current_vertex->next->id_ent, id_ent) <=0; current_vertex = current_vertex->next);

            new = new_vertex(id_ent, current_vertex->next);
            current_vertex->next = new;

        }
    }
}

void delent(vertex_t **head, char *id_ent) {
    vertex_t *current_vertex, *toDelete, *toDelete_prev;
    historyNode_t *current_history;
    node_t *current_node;

    if (*head == NULL || !vertexExists(head, id_ent))
        return;

    else{
        toDelete = returnVertex(head, id_ent);

        // ELIMINO IN OGNI VERTICE LE RELAZIONI CHE HANNO COME DESTINAZIONE IL VERTICE DA ELIMINARE
        for(current_vertex = *head; current_vertex!=NULL; current_vertex = current_vertex->next)
            for(current_node = current_vertex->head_node; current_node!=NULL; current_node = current_node->next)
                if(strcmp(current_node->id_dest, id_ent)==0)
                    current_node->valid = false;


        // ELIMINO OGNI RELAZIONE CHE ESCE DAL VERTICE DA ELIMINARE

        for(current_vertex = *head; current_vertex!=NULL; current_vertex = current_vertex->next)
            for(current_history = current_vertex->head_history; current_history!=NULL; current_history = current_history->next)
                delrel(head, toDelete->id_ent, current_vertex->id_ent, current_history->id_rel);


        // ELIMINO IL VERTICE INCRIMINATO

        for(current_vertex = *head; current_vertex!=NULL && strcmp(current_vertex->id_ent, id_ent)!=0; current_vertex = current_vertex->next)
            toDelete_prev = current_vertex;

        if (toDelete == *head) {
            *head = toDelete->next;
            free(toDelete);

            return;
        }

        toDelete_prev->next = toDelete->next;

        free(toDelete);

    }

}

void addrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {

    if (!vertexExists(head, id_orig))
        return;
    else if (!vertexExists(head, id_dest))
        return;
    else if (validRelExists(head, id_orig, id_dest, id_rel))
        return;

    addInChrono(id_rel);

    vertex_t *current_vertex, *toBeUpdated;
    node_t *current_node;

    current_vertex = returnVertex(head, id_orig);
    toBeUpdated = returnVertex(head, id_dest);

    updateHistory(toBeUpdated, id_rel, 1);

    if (current_vertex->head_node == NULL) {
        current_vertex->head_node = new_node(id_rel, id_dest, NULL);
    } else {
        for (current_node = current_vertex->head_node; current_node->next != NULL && current_node->valid; current_node = current_node->next);

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
    if (!validRelExists(head, id_orig, id_dest, id_rel))
        return;
    else if (!vertexExists(head, id_orig))
        return;
    else if (!vertexExists(head, id_dest))
        return;
    else if (*head == NULL || id_orig == NULL || id_dest == NULL || id_rel == NULL)
        return;

    vertex_t *toBeUpdated = returnVertex(head, id_dest);;
    node_t *current_node = returnValidNode(head, id_orig, id_dest, id_rel);

    updateHistory(toBeUpdated, id_rel, -1);

    current_node->valid = false;

}

void report(vertex_t **head) {
    vertex_t *current_vertex;
    historyNode_t *current_history;
    chrono_t *current_chrono;
    int relation_c = 0, max = 0;

    for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
        if (current_vertex->head_node != NULL && current_vertex->head_node->valid)
            relation_c++;

    if (relation_c == 0) {
        printf("none\n");

        return;
    }

    for (current_chrono = chrono_head; current_chrono != NULL; current_chrono = current_chrono->next) {

        for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
            for (current_history = current_vertex->head_history; current_history != NULL; current_history = current_history->next)
                if (strcmp(current_history->id_rel, current_chrono->id_rel) == 0 && current_history->num > max && current_history->valid)
                    max = current_history->num;

        if (max == 0)
            continue;
        printf("%s ", current_chrono->id_rel);
        for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
            for (current_history = current_vertex->head_history; current_history != NULL; current_history = current_history->next)
                if (strcmp(current_history->id_rel, current_chrono->id_rel) == 0 && current_history->num == max && current_history->valid)
                    printf("%s ", current_vertex->id_ent);

        printf("%d; ", max);

        max = 0;
    }

    printf("\n");


    //print_graph(head);

}





// DEBUG

void print_graph(vertex_t **head) {
    vertex_t *current_vertex;
    node_t *current_node;
    historyNode_t *current_history;
    chrono_t *current_chrono;

    no_report += 1;
    printf("report no. %d ##############################################################################################################\n", no_report);

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
        for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
            printf("        - id_rel: %s; id_dest: %s; valid: %d; \n", current_node->id_rel, current_node->id_dest,
                   current_node->valid);

    }

}
