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
} historyNode_t;

typedef struct node {
    char id_dest[MAX];
    char id_rel[MAX];
    bool reusable;
    struct node *next;
} node_t;

typedef struct vertex {
    char id_ent[MAX];
    struct vertex *next;
    bool reusable;
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
historyNode_t *new_historyNode(char *id_rel, historyNode_t *next);
chrono_t *new_chrono(char *id_rel, chrono_t *next);


/* UTILITIES */
bool vertexExists(vertex_t **head, char *id_ent);
bool relExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);
bool historyExists(vertex_t *vertex, char *id_rel);
bool inChronoExists(char *id_rel);
void updateHistory(vertex_t *vertex, char *id_rel, int update);
vertex_t *returnVertex(vertex_t **head, char *id);
node_t *returnNode(vertex_t **head, char *father, char *id);
node_t *returnNodeWithRel(vertex_t **head, char *father, char *id, char *id_rel);
void chronoAdd(char *id_rel);

/* CORE */
void addent(vertex_t **head, char *id_ent);
void delent(vertex_t **head, char *id_ent);
void addrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);
void delrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);
void report(vertex_t **head);


// DEBUG

void print_graph(vertex_t **head);

chrono_t *CHRONO = NULL;

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
vertex_t *new_vertex(char *id_ent, vertex_t *next){
  vertex_t *new_vertex = calloc(1, sizeof(vertex_t));

  strcpy(new_vertex->id_ent, id_ent);
  new_vertex->reusable = false;
  new_vertex->next = next;
  new_vertex->head_node = NULL;
  new_vertex->head_history = NULL;

  return new_vertex;

}

node_t *new_node(char *id_rel, char *id_dest, node_t *next) {
    node_t *new_node = calloc(1, sizeof(node_t));

    strcpy(new_node->id_dest, id_dest);
    strcpy(new_node->id_rel, id_rel);
    new_node->reusable = false;
    new_node->next = next;

    return new_node;
}

historyNode_t *new_historyNode(char *id_rel, historyNode_t *next){
  historyNode_t *new_history = calloc(1, sizeof(historyNode_t));

  strcpy(new_history->id_rel, id_rel);
  new_history->num = 1;
  new_history->next = next;

  return new_history;

}

chrono_t *new_chrono(char *id_rel, chrono_t *next){
  chrono_t *new = calloc(1, sizeof(chrono_t));

  strcpy(new->id_rel, id_rel);
  new->next = next;

  return new;
}


/* UTILITIES */
bool vertexExists(vertex_t **head, char *id_ent){
  vertex_t *current_vertex;

  for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
    if (strcmp(current_vertex->id_ent, id_ent) == 0)
      return !(current_vertex->reusable);

  return false;
}

bool relExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel){
  vertex_t *current_vertex;
  node_t *current_node;

  if(!vertexExists(head, id_orig))
    return false;

  else{

    current_vertex = returnVertex(head, id_orig);

    if (current_vertex == NULL || current_vertex->head_node == NULL)
        return false;

    else {
        for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
            if (strcmp(current_node->id_rel, id_rel) == 0 && strcmp(current_node->id_dest, id_dest) == 0)
                return !(current_node->reusable);

        return false;
    }
  }
}

bool inChronoExists(char *id_rel){
  chrono_t *chrono;

  for (chrono = CHRONO; chrono != NULL; chrono = chrono->next)
    if (strcmp(chrono->id_rel, id_rel) == 0)
      return true;

  return false;

}








void chronoAdd(char *id_rel){

  chrono_t *chrono = CHRONO, *next, *new;

  if (inChronoExists(id_rel))
    return;

  else{
    if (chrono == NULL || strcmp(chrono->id_rel, id_rel) > 0)
      CHRONO = new_chrono(id_rel, chrono);

    else {
      for (chrono = CHRONO; chrono->next != NULL && strcmp(chrono->next->id_rel, id_rel) <= 0; chrono = chrono->next);

      next = chrono->next;
      new = new_chrono(id_rel, next);
      chrono->next = new;
    }
  }
}

bool historyExists(vertex_t *vertex, char *id_rel){
  historyNode_t *history;

  for(history = vertex->head_history; history!=NULL; history = history->next)
    if(strcmp(history->id_rel, id_rel)==0)
      return true;

  return false;

}

void updateHistory(vertex_t *vertex, char *id_rel, int update){
  historyNode_t *current_history = vertex->head_history, *prev;

  if(!historyExists(vertex, id_rel) && update==1){
    if(current_history==NULL || strcmp(current_history->id_rel, id_rel)>0)
      vertex->head_history = new_historyNode(id_rel, current_history);

    else{
      for(current_history = vertex->head_history; current_history->next!=NULL && strcmp(current_history->next->id_rel, id_rel)<=0; current_history = current_history->next);
      current_history->next = new_historyNode(id_rel, current_history->next);
    }
  }

  else{
    for(current_history = vertex->head_history; current_history!=NULL && strcmp(current_history->id_rel, id_rel)!=0; current_history = current_history->next)
      prev = current_history;

    if((current_history->num)+update==0){
      if(current_history == vertex->head_history || current_history->next==NULL)
        vertex->head_history = current_history->next;
      else
        prev->next = current_history->next;

      free(current_history);
    }

    else{
      current_history->num += update; 
    }
  }

}

vertex_t *returnVertex(vertex_t **head, char *id){
  vertex_t *current = NULL;

  for(current = *head; current!=NULL; current = current->next)
    if(strcmp(current->id_ent, id)==0)
      return current;
  return NULL;
}

node_t *returnNode(vertex_t **head, char *father, char *id){
    vertex_t *current = returnVertex(head, father);
    node_t *current_node;

    for(current_node = current->head_node; current_node!=NULL && strcmp(current_node->id_dest, id) != 0; current_node = current_node->next);

    if (current_node==NULL)
      return NULL;
    else
      return current_node;
}

node_t *returnNodeWithRel(vertex_t **head, char *father, char *id, char *id_rel){
    vertex_t *current = returnVertex(head, father);
    node_t *current_node;

    for(current_node = current->head_node; current_node!=NULL; current_node = current_node->next){
      if(strcmp(current_node->id_dest, id) == 0 && strcmp(current_node->id_rel, id_rel) == 0)
        break;
    }

    if (current_node==NULL)
      return NULL;
    else
      return current_node;
}


/* CORE */
void addent(vertex_t **head, char *id_ent){
  vertex_t *current_vertex;

  if (vertexExists(head, id_ent))
        return;

  else{

    if (*head == NULL)
      *head = new_vertex(id_ent, NULL);

    else{

      for(current_vertex = *head; current_vertex->next != NULL && current_vertex->reusable==false; current_vertex = current_vertex->next);

        if(current_vertex->reusable==true){
          strcpy(current_vertex->id_ent, id_ent);
          current_vertex->reusable = false;
        }

        else
          current_vertex->next = new_vertex(id_ent, NULL);
      }
    }
}




void delent(vertex_t **head, char *id_ent) {
    vertex_t *current, *tmp;
    historyNode_t *current_history;

    if (!vertexExists(head, id_ent) || *head == NULL)
        return;

    else{

      current = returnVertex(head, id_ent);
      tmp = current;

      for (current = *head; current!=NULL; current = current->next) {
        if(current->head_history==NULL)
          continue;
        else{
          for (current_history = current->head_history; current_history!=NULL; current_history = current_history->next)
            delrel(head, id_ent, current->id_ent, current_history->id_rel);
        }
      }

      tmp->reusable = true;

      return;
    }
}

void addrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {
    chronoAdd(id_rel);

  if (!vertexExists(head, id_orig) || !vertexExists(head, id_dest) || !(strcmp(id_orig, id_dest)) || relExists(head, id_orig, id_dest, id_rel))
      return;

  vertex_t *current_vertex, *toBeUpdated;

  node_t *current_node, *tmp;

  current_vertex = returnVertex(head, id_orig);
  toBeUpdated = returnVertex(head, id_dest);

  if (current_vertex->head_node == NULL)
      current_vertex->head_node = new_node(id_rel, id_dest, NULL);

  else {
      for (current_node = current_vertex->head_node; current_node->next != NULL && current_node->reusable==false; current_node = current_node->next);

        tmp = current_node;
        if (current_node->reusable==true){
          strcpy(current_node->id_dest, id_dest);
          strcpy(current_node->id_rel, id_rel);
        }

        else
          current_node->next = new_node(id_rel, id_dest, NULL);
     
  }

  updateHistory(toBeUpdated, id_rel, 1);
  tmp->reusable = false;

    return;
}

void delrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {
  if (!relExists(head, id_orig, id_dest, id_rel))
    return;
  if (!vertexExists(head, id_orig))
    return;
  if (!vertexExists(head, id_dest))
    return;

  vertex_t *current, *toBeUpdated;
  node_t *current_node;

  toBeUpdated = returnVertex(head, id_dest);
  current_node = returnNodeWithRel(head, id_orig, id_dest, id_rel);

  updateHistory(toBeUpdated, id_rel, -1);
  current_node->reusable=true;

  return;
}

void report(vertex_t **head) {
    vertex_t *current_vertex;
    historyNode_t *current_history;
    chrono_t *chrono = CHRONO;
    int relation_c = 0;
    int max = 0;

    for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
        if (current_vertex->head_node != NULL)
            relation_c++;

    if (relation_c == 0)
        printf("none\n");



    print_graph(head);

}





// DEBUG

void print_graph(vertex_t **head){
  vertex_t *current_vertex;
  node_t *current_node;
  historyNode_t *current_history;
  chrono_t *chrono;

  printf("###############################################\n");

  if(*head==NULL)
    puts("\nVoid graph\n");

  for(current_vertex=*head; current_vertex!=NULL; current_vertex=current_vertex->next){
    if(current_vertex->reusable==true)
      continue;

    printf("%s: \n", current_vertex->id_ent);

    for(current_history=current_vertex->head_history; current_history!=NULL; current_history=current_history->next){
      printf("- id_rel: %s; num: %d; \n", current_history->id_rel, current_history->num);
    }

  }

}
