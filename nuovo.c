#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255
#define CMD_MAX 7

typedef struct history{
    char id_rel[MAX];
    int num;
    struct history *next;
    struct history *prev;
} historyNode_t;

typedef struct node {
    char id_dest[MAX];
    char id_rel[MAX];
    struct node *next;
    struct node *prev;
} node_t;

typedef struct vertex {
    char id_ent[MAX];
    struct vertex *next;
    struct vertex *prev;
    struct node *head_node;
    struct history *head_history;
} vertex_t;

/* COSTRUTTORI */
vertex_t *new_vertex(char *id_ent, vertex_t *next, vertex_t *prev);
node_t *new_node(char *id_rel, char *id_dest, node_t *next, node_t *prev);
historyNode_t *new_historyNode(char *id_rel, int num, historyNode_t *next, historyNode_t *prev);

/* UTILITIES */
bool vertexExists(vertex_t **head, char *id_ent);
bool relExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);
void updateHistory(vertex_t *id_ent, char *id_rel, int update);

/* CORE */
void addent(vertex_t **head, char *id_ent);
void delent(vertex_t **head, char *id_ent);
void addrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);
void delrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);
void report(vertex_t **head);


// DEBUG

void print_graph(vertex_t **head);

int main(int argc, char **argv) {
    char cmd[CMD_MAX];
    char arg0[MAX];
    char arg1[MAX];
    char arg2[MAX];
    vertex_t *HEAD = NULL;

    for (;;) {
        scanf("%s", cmd);

        if (strncmp("report", cmd, CMD_MAX-1) == 0)
            report(&HEAD);

        if (strncmp("end", cmd, CMD_MAX-1) == 0) {
            break;
        }

        if (strncmp("addent", cmd, CMD_MAX-1) == 0) {
            scanf("%s", arg0);
            addent(&HEAD, arg0);
        }

        if (strncmp("delent", cmd, CMD_MAX-1) == 0) {
            scanf("%s", arg0);
            delent(&HEAD, arg0);
        }

        if (strncmp("addrel", cmd, CMD_MAX-1) == 0) {
            scanf("%s %s %s", arg0, arg1, arg2);
            addrel(&HEAD, arg0, arg1, arg2);
        }

        if (strncmp("delrel", cmd, CMD_MAX-1) == 0) {
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
vertex_t *new_vertex(char *id_ent, vertex_t *next, vertex_t *prev){
  vertex_t *new_vertex = malloc(sizeof(vertex_t));

  strcpy(new_vertex->id_ent, id_ent);
  new_vertex->next = next;
  new_vertex->prev = prev;
  new_vertex->head_node = NULL;
  new_vertex->head_history = NULL;

  return new_vertex;

}
node_t *new_node(char *id_rel, char *id_dest, node_t *next, node_t *prev) {
    node_t *new_node = malloc(sizeof(node_t));

    strcpy(new_node->id_dest, id_dest);
    strcpy(new_node->id_rel, id_rel);
    new_node->next = next;
    new_node->prev = prev;

    return new_node;
}
historyNode_t *new_historyNode(char *id_rel, int num, historyNode_t *next, historyNode_t *prev){
  historyNode_t *new_history = malloc(sizeof(historyNode_t));

  strcpy(new_history->id_rel, id_rel);
  new_history->num = num;
  new_history->next = next;
  new_history->next = prev;

  return new_history;

}

/* UTILITIES */
bool vertexExists(vertex_t **head, char *id_ent){
  vertex_t *current_vertex;

  for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
    if (strcmp(current_vertex->id_ent, id_ent) == 0)
      return true;

  return false;
}
bool relExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel){
  vertex_t *current_vertex;
  node_t *current_node;

  for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
    if (strcmp(current_vertex->id_ent, id_orig) == 0)
      break;

    if (current_vertex == NULL || current_vertex->head_node == NULL)
        return false;

    else {
        for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
            if (strcmp(current_node->id_rel, id_rel) == 0 && strcmp(current_node->id_dest, id_dest) == 0)
                return true;

        return false;
    }
}
void updateHistory(vertex_t *id_ent, char *id_rel, int update){
  // to do
}


/* CORE */
void addent(vertex_t **head, char *id_ent){
  vertex_t *current_vertex = *head, *next, *new;

  if (vertexExists(head, id_ent))
        return;

  else{
    if (current_vertex == NULL || strcmp(current_vertex->id_ent, id_ent) > 0)
      *head = new_vertex(id_ent, current_vertex, NULL);

    else {
      for (current_vertex = *head; current_vertex->next != NULL && strcmp(current_vertex->next->id_ent, id_ent) <= 0; current_vertex = current_vertex->next);

      next = current_vertex->next;
      new = new_vertex(id_ent, next, current_vertex);
      current_vertex->next = new;

      if (next != NULL)
        next->prev = new;
    }
  }
}

void delent(vertex_t **head, char *id_ent) {
    vertex_t *current = *head, *tmp_vertex;
    historyNode_t *current_history;

    if (!vertexExists(head, id_ent) || *head == NULL)
        return;

    else{

      for (current = *head; current!=NULL && strcmp(current->id_ent, id_ent) != 0; current = current->next);
      tmp_vertex = current;

/*    To do: delete all relation that have id_ent as destination, from all the other nodes
      for (current = *head; current!=NULL; current = current->next) {
        if(current->head_history==NULL)
          continue;
        else{
          for (current_history = current->head_history; current_history!=NULL; current_history = current_history->next)
            delrel(head, id_ent, current->id_ent, current_history->id_rel);
        }
      }
*/

      if (*head == tmp_vertex)
          *head = tmp_vertex->next;
      if (tmp_vertex->next != NULL)
          tmp_vertex->next->prev = tmp_vertex->prev;
      if (tmp_vertex->prev != NULL)
          tmp_vertex->prev->next = tmp_vertex->next;

      free(tmp_vertex);

      return;

    }

}

void addrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {

  if (!vertexExists(head, id_orig) || !vertexExists(head, id_dest) || !(strcmp(id_orig, id_dest)) || relExists(head, id_orig, id_dest, id_rel))
      return;

  vertex_t *current_vertex;

  node_t *current_node, *next, *new;

  for (current_vertex = *head; current_vertex!=NULL && strcmp(current_vertex->id_ent, id_orig) != 0; current_vertex = current_vertex->next);

  if (current_vertex->head_node == NULL || strcmp(current_vertex->head_node->id_dest, id_dest) > 0)
      current_vertex->head_node = new_node(id_rel, id_dest, NULL, NULL);

  else {
      for (current_node = current_vertex->head_node; current_node->next != NULL && strcmp(current_node->next->id_dest, id_dest) <= 0; current_node = current_node->next);
      next = current_node->next;
      new = new_node(id_rel, id_dest, next, current_node);

      current_node->next = new;
      if(next!=NULL)
          next->prev = new;
  }

    return;
}

void delrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {
  if (!relExists(head, id_orig, id_dest, id_rel) || !vertexExists(head, id_orig) || !vertexExists(head, id_dest))
      return;

  vertex_t *current = *head;
  node_t *current_node, *tmp_node;


  for (current = *head; current != NULL && strcmp(current->id_ent, id_orig) != 0; current = current->next);

  for (current_node = current->head_node; current_node != NULL && strcmp(current_node->id_dest, id_dest) != 0 && strcmp(current_node->id_rel, id_rel) != 0; current_node = current_node->next);

  tmp_node = current_node;

  if(current->head_node == tmp_node)
    current->head_node = tmp_node->next;
  if (tmp_node->next!=NULL)
    tmp_node->next->prev = tmp_node->prev;
  if(tmp_node->prev != NULL)
    tmp_node->prev->next = tmp_node->next;


  free(tmp_node);

  return;
}

void report(vertex_t **head) {
    vertex_t *current_vertex;
    int relation_c = 0;

/*
    for (current_vertex = HEAD; current_vertex != NULL; current_vertex = current_vertex->next)
        if (current_vertex->head_node != NULL)
            relation_c++;

    if (relation_c == 0)
        printf("none");
*/

    print_graph(head);


}





// DEBUG

void print_graph(vertex_t **head){
  vertex_t *current_vertex, *tail_vertex;
  node_t *current_node;

  printf("#####\n");

  if(*head==NULL)
    puts("\nVoid graph\n");

  for(current_vertex=*head; current_vertex!=NULL; current_vertex=current_vertex->next){
    printf("%s: \n", current_vertex->id_ent);

    for(current_node=current_vertex->head_node; current_node!=NULL; current_node=current_node->next){
      printf("  id_dest: %s; id_rel: %s; \n", current_node->id_dest, current_node->id_rel);
    }
  }

  printf("\n#####\n");

}
