#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255

typedef struct history{
    char id_rel[MAX];
    int num;
    struct history *next_history;
    struct history *prev_history;
} historyNode_t;

typedef struct node {
    char id_dest[MAX];
    char id_rel[MAX];
    struct node *next_node;
    struct node *prev_node;
} node_t;

typedef struct vertex {
    char id_ent[MAX];
    struct vertex *next_vertex;
    struct vertex *prev_vertex;
    struct node *head_node;
    struct history *head_history;
} vertex_t;

/* UTILITIES */

vertex_t *new_vertex(char *id_ent, vertex_t *next, vertex_t *prev);
node_t *new_node(char *id_rel, char *id_dest, node_t *next, node_t *prev);
historyNode_t *new_historyNode(char *id_rel, int num, historyNode_t *next_history, historyNode_t *prev_history);



bool vertexExists(char *id_ent);
bool relExists(char *id_orig, char *id_dest, char *id_rel);
bool relationExistsInHistory(char *id_rel);

/* CORE */
void addent(char *id_ent);
void delent(char *id_ent);
void addrel(char *id_orig, char *id_dest, char *id_rel);
void delrel(char *id_orig, char *id_dest, char *id_rel);
void report();


void updateHistory(vertex_t *id, char *id_rel, int update);

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



/* UTILITIES */

vertex_t *new_vertex(char *id_ent, vertex_t *next, vertex_t *prev) {
    vertex_t *new_vertex = malloc(sizeof(vertex_t));

    strcpy(new_vertex->id_ent, id_ent);
    new_vertex->head_node = NULL;
    new_vertex->head_history = NULL;
    new_vertex->next_vertex = next;
    new_vertex->prev_vertex = prev;

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

historyNode_t *new_historyNode(char *id_rel, int num, historyNode_t *next_history, historyNode_t *prev_history){
  historyNode_t *new_history = malloc(sizeof(historyNode_t));

  strcpy(new_history->id_rel, id_rel);
  new_history->num = num;
  new_history->next_history = next_history;
  new_history->prev_history = prev_history;

  return new_history;

}

bool vertexExists(char *id_ent) {
    vertex_t *current_vertex;

    for (current_vertex = HEAD; current_vertex != NULL; current_vertex = current_vertex->next_vertex)
        if (strcmp(current_vertex->id_ent, id_ent) == 0)
            return true;

    return false;
}

bool relExists(char *id_orig, char *id_dest, char *id_rel) {
    vertex_t *current_vertex;
    node_t *current_node;

    for (current_vertex = HEAD; current_vertex != NULL; current_vertex = current_vertex->next_vertex) {
        if (strcmp(current_vertex->id_ent, id_orig) == 0)
            break;
    }

    if (current_vertex == NULL)
        return false;

    if (current_vertex->head_node == NULL)
        return false;

    else {
        for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next_node)
            if (strcmp(current_node->id_rel, id_rel) == 0 && strcmp(current_node->id_dest, id_dest) == 0)
                return true;

        return false;
    }
}










/* CORE */

void addent(char *id_ent) {

    vertex_t *current_vertex = HEAD, *next, *new;

    if (vertexExists(id_ent))
        return;

    else {

        if (current_vertex == NULL || strcmp(current_vertex->id_ent, id_ent) > 0)
            HEAD = new_vertex(id_ent, current_vertex, NULL);

        else {

            for (current_vertex = HEAD; current_vertex->next_vertex != NULL && strcmp(current_vertex->next_vertex->id_ent, id_ent) <= 0; current_vertex = current_vertex->next_vertex);
            next = current_vertex->next_vertex;

            new = new_vertex(id_ent, next, current_vertex);
            current_vertex->next_vertex = new;
            if (next != NULL)
                next->prev_vertex = new;
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

    if(!(strcmp(id_orig, id_dest)))
        return;

    if (relExists(id_orig, id_dest, id_rel))
        return;

    vertex_t *current_vertex;

    node_t *current_node, *next, *new;

    for (current_vertex = HEAD; current_vertex!=NULL && strcmp(current_vertex->id_ent, id_orig) != 0; current_vertex = current_vertex->next_vertex);

    if (current_vertex->head_node == NULL || strcmp(current_vertex->head_node->id_dest, id_dest) > 0)
        current_vertex->head_node = new_node(id_rel, id_dest, NULL, NULL);

    else {
        for (current_node = current_vertex->head_node; current_node->next_node != NULL && strcmp(current_node->next_node->id_dest, id_dest) <= 0; current_node = current_node->next_node);
        next = current_node->next_node;
        new = new_node(id_rel, id_dest, next, current_node);

        current_node->next_node = new;
        if(next!=NULL)
            next->prev_node = new;
    }

    updateHistory(current_vertex, id_rel, 1);

}

void delrel(char *id_orig, char *id_dest, char *id_rel) {

    if (!relExists(id_orig, id_dest, id_rel)) {
        return;
    }

    vertex_t *current_vertex;
    node_t *current_node, *tmp;

    for (current_vertex = HEAD; strcmp(current_vertex->id_ent, id_orig) != 0; current_vertex = current_vertex->next_vertex);

    current_node = current_vertex->head_node;

    if (current_node->next_node == NULL && current_node->prev_node == NULL && strcmp(current_node->id_dest, id_dest) == 0 && strcmp(current_node->id_rel, id_rel) == 0) {
        free(current_node);
        current_vertex->head_node = NULL;

    }

    else {
            for (current_node = current_vertex->head_node;
                 current_node != NULL; current_node = current_node->next_node) {
                if (strcmp(current_node->id_dest, id_dest) == 0 && strcmp(current_node->id_rel, id_rel) == 0)
                    break;
            }

            tmp = current_node;

            if (current_node->prev_node == NULL) {
                current_vertex->head_node = current_node->next_node;
                (current_node->next_node)->prev_node = NULL;

                free(tmp);

            } else {

                if (current_node->next_node == NULL) {
                    (current_node->prev_node)->next_node = NULL;
                    free(tmp);

                } else {

                    (current_node->prev_node)->next_node = tmp->next_node;
                    (current_node->next_node)->prev_node = tmp->prev_node;
                    free(tmp);
                }
            }

        }

    updateHistory(current_vertex, id_rel, -1);

}

void updateHistory(vertex_t *id, char *id_rel, int update){
    vertex_t *current_vertex = id;
    historyNode_t *current_history = id->head_history, *prev;
    historyNode_t *new_history;

    if (update==1){
      if(current_history==NULL)
        current_history = new_historyNode(id_rel, update, NULL, NULL);

      else{

    for(current_history = id->head_history; current_history!=NULL && strcmp(id_rel, current_history->id_rel)!=0; current_history = current_history->next_history)
      prev = current_history;

        if(current_history==NULL)
          current_history = new_historyNode(id_rel, update, NULL, prev);
        else
            current_history->num += update;
      }


    if (update==-1){
      if(current_history==NULL)
        return;

      else{
        for(current_history = id->head_history; current_history!=NULL && strcmp(id_rel, current_history->id_rel)!=0; current_history = current_history->next_history)
          prev = current_history;

        if(current_history->num + update == 0){
          current_history->prev_history->next_history = current_history->next_history;
          current_history->next_history->prev_history = prev;
        }
        else{
          current_history->num += update;
        }
      }



    }

}



void report() {
    vertex_t *tmp_none, *current;
    int relation_c = 0, max = 0, tmp;
    char entities[MAX][MAX];
    int j, m;


    for (tmp_none = HEAD; tmp_none != NULL; tmp_none = tmp_none->next)
        if (tmp_none->next_node != NULL)
            relation_c++;

    if (relation_c == 0)
        printf("none");






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
