#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255

typedef struct relHistory{
    char id_rel[MAX];
    int num;
    struct relHistory *next;
} relHistory_t;

typedef struct vertex {
    char id_ent[MAX];
    struct node *next_node;
    struct vertex *next;
    struct vertex *prev;
    struct relHistory *head;
} vertex_t;

typedef struct node {
    char id_dest[MAX];
    char id_rel[MAX];
    struct node *next_node;
    struct node *prev_node;
} node_t;

/* UTILITIES */

vertex_t *new_vertex(char *id_ent, vertex_t *next, vertex_t *prev);
node_t *new_node(char *id_rel, char *id_dest, node_t *next, node_t *prev);
bool vertexExists(char *id_ent);
bool relExists(char *id_orig, char *id_dest, char *id_rel);
bool relationExistsInHistory(char *id_rel);

/* CORE */
void addent(char *id_ent);
void delent(char *id_ent);
void addrel(char *id_orig, char *id_dest, char *id_rel);
void delrel(char *id_orig, char *id_dest, char *id_rel);
void report();


void updateHistory(char *id_ent, char *id_rel, int update);

int numRelPerEntities(char *id_dest, char *id_rel);


//DEBUG
void print_graph();

vertex_t *HEAD = NULL;

/* MAIN */
int main(int argc, char **argv) {
    char cmd[7];
    char arg0[MAX];
    char arg1[MAX];
    char arg2[MAX];

    for (;;) {
        scanf("%s", cmd);

        if (strncmp("report", cmd, 6) == 0)
            report();

        if (strncmp("end", cmd, 6) == 0) {
            break;
        }

        if (strncmp("addent", cmd, 6) == 0) {
            scanf("%s", arg0);
            addent(arg0);
        }

        if (strncmp("delent", cmd, 6) == 0) {
            scanf("%s", arg0);
            delent(arg0);
        }

        if (strncmp("addrel", cmd, 6) == 0) {
            scanf("%s %s %s", arg0, arg1, arg2);
            addrel(arg0, arg1, arg2);
        }

        if (strncmp("delrel", cmd, 6) == 0) {
            scanf("%s %s %s", arg0, arg1, arg2);
            delrel(arg0, arg1, arg2);
        }

        strncpy(cmd, "", 7);
        strncpy(arg0, "", MAX);
        strncpy(arg1, "", MAX);
        strncpy(arg2, "", MAX);

    }

    return 0;
}

vertex_t *new_vertex(char *id_ent, vertex_t *next, vertex_t *prev) {
    vertex_t *new_vertex = malloc(sizeof(vertex_t));

    strcpy(new_vertex->id_ent, id_ent);
    new_vertex->next_node = NULL;
    new_vertex->head = NULL;
    new_vertex->next = next;
    new_vertex->prev = prev;


    return new_vertex;
}

node_t *new_node(char *id_rel, char *id_dest, node_t *next, node_t *prev) {
    node_t *new_node = malloc(sizeof(node_t));

    strcpy(new_node->id_dest, id_dest);
    strcpy(new_node->id_rel, id_rel);
    new_node->next_node = next;
    new_node->prev_node = prev;

    return new_node;
}

bool vertexExists(char *id_ent) {
    vertex_t *current;

    for (current = HEAD; current != NULL; current = current->next)
        if (strcmp(current->id_ent, id_ent) == 0)
            return true;

    return false;
}

bool relExists(char *id_orig, char *id_dest, char *id_rel) {
    vertex_t *current;
    node_t *current_node;

    for (current = HEAD; current != NULL; current = current->next) {
        if (strcmp(current->id_ent, id_orig) == 0)
            break;
    }

    if (current == NULL)
        return false;

    current_node = current->next_node;

    if (current_node == NULL)
        return false;

    else {
        for (current_node = current->next_node; current_node != NULL; current_node = current_node->next_node)
            if (strcmp(current_node->id_rel, id_rel) == 0 && strcmp(current_node->id_dest, id_dest) == 0)
                return true;

        return false;
    }
}


/* CORE */

void addent(char *id_ent) {

    vertex_t *current = HEAD, *next, *new;

    if (vertexExists(id_ent))
        return;

    else {

        if (HEAD == NULL || strcmp(HEAD->id_ent, id_ent) > 0)
            HEAD = new_vertex(id_ent, current, NULL);

        else {

            for (current = HEAD; current->next != NULL && strcmp(current->next->id_ent, id_ent) <= 0; current = current->next);
            next = current->next;

            new = new_vertex(id_ent, next, current);
            current->next = new;
            if (next != NULL)
                next->prev = new;
        }

    }

}
void delent(char *id_ent) {
    vertex_t *current = HEAD, *tmp_vertex;
    relHistory_t *tmp_history;

    if (!vertexExists(id_ent))
        return;

    for (current = HEAD; current != NULL && strcmp(current->id_ent, id_ent) != 0; current = current->next);
    tmp_vertex = current;
    
    for (current = HEAD; current!=NULL; current = current->next) {
        for (tmp_history = current->head; tmp_history!=NULL; tmp_history = tmp_history->next) {
            delrel(id_ent, current->id_ent, tmp_history->id_rel);
        }
    }

    if (tmp_vertex == HEAD)
        HEAD = tmp_vertex->next;
    if (tmp_vertex->next != NULL)
        tmp_vertex->next->prev = tmp_vertex->prev;
    if (tmp_vertex->prev != NULL)
        tmp_vertex->prev->next = tmp_vertex->next;

    free(tmp_vertex);

}

void addrel(char *id_orig, char *id_dest, char *id_rel) {

    if (!vertexExists(id_orig)) {
        return;
    }

    if (!vertexExists(id_dest)) {
        return;
    }

    if(strcmp(id_orig, id_dest)==0)
        return;

    if (relExists(id_orig, id_dest, id_rel))
        return;

    vertex_t *current_vertex;

    node_t *current_node, *next, *new;

    for (current_vertex = HEAD; current_vertex!=NULL && strcmp(current_vertex->id_ent, id_orig) != 0; current_vertex = current_vertex->next);

    if (current_vertex->next_node == NULL || strcmp(current_vertex->next_node->id_dest, id_dest) > 0)
        current_vertex->next_node = new_node(id_rel, id_dest, NULL, NULL);

    else {
        for (current_node = current_vertex->next_node; current_node->next_node != NULL && strcmp(current_node->next_node->id_dest, id_dest) <= 0; current_node = current_node->next_node);
        next = current_node->next_node;
        new = new_node(id_rel, id_dest, next, current_node);

        current_node->next_node = new;
        if(next!=NULL)
            next->prev_node = new;
    }

    updateHistory(id_dest, id_rel, 1);

}

void delrel(char *id_orig, char *id_dest, char *id_rel) {

    if (!relExists(id_orig, id_dest, id_rel)) {
        return;
    } 

    vertex_t *current_vertex;
    node_t *current_node, *tmp;

    for (current_vertex = HEAD; strcmp(current_vertex->id_ent, id_orig) != 0; current_vertex = current_vertex->next);

    current_node = current_vertex->next_node;

    if (current_node->next_node == NULL && current_node->prev_node == NULL && strcmp(current_node->id_dest, id_dest) == 0 && strcmp(current_node->id_rel, id_rel) == 0) {
        free(current_node);
        current_vertex->next_node = NULL;

    } 

    else {
            for (current_node = current_vertex->next_node;
                 current_node != NULL; current_node = current_node->next_node) {
                if (strcmp(current_node->id_dest, id_dest) == 0 && strcmp(current_node->id_rel, id_rel) == 0)
                    break;
            }

            tmp = current_node;
            if (current_node->prev_node == NULL) {
                current_vertex->next_node = current_node->next_node;
                (current_node->next_node)->prev_node = NULL;

                free(tmp);
                return;
            } else {

                if (current_node->next_node == NULL) {
                    (current_node->prev_node)->next_node = NULL;
                    free(tmp);

                    return;

                } else {

                    (current_node->prev_node)->next_node = tmp->next_node;
                    (current_node->next_node)->prev_node = tmp->prev_node;
                    free(tmp);

                    return;
                }
            }

        }

    updateHistory(id_dest, id_rel, -1);

}

void updateHistory(char *id_ent, char *id_rel, int update){
    vertex_t *tmp_vertex;
    relHistory_t *tmp_relation = tmp_vertex->head;
    relHistory_t *new_relation = NULL;

    if (update==1){
        for(tmp_vertex = HEAD; tmp_vertex!=NULL; tmp_vertex = tmp_vertex->next)
            for(tmp_relation = tmp_vertex->head; tmp_relation!=NULL && strcmp(id_rel, tmp_relation->id_rel)!=0; tmp_relation = tmp_relation->next);
        
        
        if(tmp_relation==NULL){
            new_relation = malloc(sizeof(relHistory_t));
            strcpy(tmp_relation->id_rel, id_rel);
            tmp_relation->num = 1;
            new_relation->next = NULL;
            tmp_vertex->head = new_relation;

            return; 
        }
        else{
            tmp_relation->num += 1;

            return;
        }


    if (update==-1){

    }

    else
        return;




}


int numRelPerEntities(char *id_dest, char *id_rel) {
    vertex_t *current_vertex;
    node_t *current_node;
    int counter = 0;

    for (current_vertex = HEAD; current_vertex != NULL; current_vertex = current_vertex->next) {
        for (current_node = current_vertex->next_node; current_node != NULL; current_node = current_node->next_node) {
            if (strcmp(current_node->id_dest, id_dest) == 0 && strcmp(current_node->id_rel, id_rel) == 0)
                counter++;
        }
    }

    return counter;

}

void report() {
    vertex_t *tmp_none, *current;
    int relation_c = 0, max = 0, tmp;
    char entities[MAX][MAX];
    int j, m;
/*
    for (tmp_none = HEAD; tmp_none != NULL; tmp_none = tmp_none->next)
        if (tmp_none->next_node != NULL)
            relation_c++;

    if (relation_c == 0)
        printf("none");

    for (current_relation = REL_HEAD; current_relation != NULL; current_relation = current_relation->next) {

        for (current = HEAD; current != NULL; current = current->next) {
            tmp = numRelPerEntities(current->id_ent, current_relation->id_rel);
            if (tmp >= max)
                max = tmp;

        }

        for (current = HEAD; current != NULL; current = current->next) {
            tmp = numRelPerEntities(current->id_ent, current_relation->id_rel);
            if (tmp == max && tmp != 0) {
                strcpy(entities[j], current->id_ent);
                j++;
            }
        }

        if (max == 0)
            goto RESET;

        printf("%s ", current_relation->id_rel);
        for (m = 0; m < j; m++)
            printf("%s ", entities[m]);
        printf("%d; ", max);

        RESET:

        max = 0;
        tmp = max;
        j = 0;

        int k;
        for (k = 0; k < MAX; k++)
            strcpy(entities[k], "");


    }
    printf("\n");
*/

    print_graph();


}














// DEBUG

void print_graph() {
    vertex_t *tmp;
    node_t *tmp_node;
    int i;

    printf("##########\n");

    if (HEAD == NULL)
        printf("Void graph.\n");

    for (tmp = HEAD; tmp != NULL; tmp = tmp->next) {
        printf("%s: \n", tmp->id_ent);
        for (tmp_node = tmp->next_node; tmp_node != NULL; tmp_node = tmp_node->next_node)
            printf("	id_rel: %s; id_dest: %s; \n", tmp_node->id_rel, tmp_node->id_dest);

    }

    for(tmp = HEAD; tmp!=NULL; tmp = tmp->next){
        for(i=0; i< tmp->h_length; i++)
            printf("ent: %s, rel: %s, num: %d\n", tmp->id_ent, tmp->history[i].id_rel, tmp->history[i].num);
    }

    printf("##########\n");

    return;
}
