#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255
#define CMD_MAX 7

typedef struct history{
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
    struct vertex *next;
    bool valid;
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
bool validVertexExists(vertex_t **head, char *id_ent);
bool validRelExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel);
bool validHistoryExists(vertex_t *vertex, char *id_rel);
bool inChronoExists(char *id_rel);
void updateHistory(vertex_t *vertex, char *id_rel, int update);
vertex_t *returnValidVertex(vertex_t **head, char *id);
node_t *returnValidNode(vertex_t **head, char *father, char *id);
node_t *returnValidNodeWithRel(vertex_t **head, char *father, char *id, char *id_rel);
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
  new_vertex->valid = true;
  new_vertex->next = next;
  new_vertex->head_node = NULL;
  new_vertex->head_history = NULL;

  return new_vertex;

}

node_t *new_node(char *id_rel, char *id_dest, node_t *next) {
    node_t *new_node = calloc(1, sizeof(node_t));

    strcpy(new_node->id_dest, id_dest);
    strcpy(new_node->id_rel, id_rel);
    new_node->valid = true;
    new_node->next = next;

    return new_node;
}

historyNode_t *new_historyNode(char *id_rel, historyNode_t *next){
  historyNode_t *new_history = calloc(1, sizeof(historyNode_t));

  strcpy(new_history->id_rel, id_rel);
  new_history->num = 1;
  new_history->next = next;
  new_history->valid = true;

  return new_history;

}

chrono_t *new_chrono(char *id_rel, chrono_t *next){
  chrono_t *new = calloc(1, sizeof(chrono_t));

  strcpy(new->id_rel, id_rel);
  new->next = next;

  return new;
}


/* UTILITIES */
bool validVertexExists(vertex_t **head, char *id_ent){
  vertex_t *current_vertex;

  for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
    if (strcmp(current_vertex->id_ent, id_ent) == 0)
      return current_vertex->valid;

  return false;
}

bool validRelExists(vertex_t **head, char *id_orig, char *id_dest, char *id_rel){
  vertex_t *current_vertex;
  node_t *current_node;

  if(!validVertexExists(head, id_orig))
    return false;

  else{

    current_vertex = returnValidVertex(head, id_orig);

    if (current_vertex == NULL || current_vertex->head_node == NULL)
        return false;

    else {
        for (current_node = current_vertex->head_node; current_node != NULL; current_node = current_node->next)
            if (strcmp(current_node->id_rel, id_rel) == 0 && strcmp(current_node->id_dest, id_dest) == 0)
                return current_node->valid;

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

bool validHistoryExists(vertex_t *vertex, char *id_rel){
  historyNode_t *history;

  for(history = vertex->head_history; history!=NULL; history = history->next)
    if(strcmp(history->id_rel, id_rel)==0 && history->valid)
      return true;

  return false;

}

void updateHistory(vertex_t *vertex, char *id_rel, int update){
  historyNode_t *current_history = vertex->head_history;

  if(validHistoryExists(vertex, id_rel)){
    for(current_history = vertex->head_history; current_history!=NULL && strcmp(current_history->id_rel, id_rel)!=0; current_history = current_history->next);

    if(((current_history->num)+update)<=0)
      current_history->valid = false;

    else
      current_history->num += update;
      
  }
  
  else{
    if(vertex->head_history==NULL)
      vertex->head_history = new_historyNode(id_rel, NULL);

    else{
      for(current_history = vertex->head_history; current_history->next!=NULL && current_history->valid; current_history = current_history->next);

      if(current_history->next==NULL)
        current_history->next = new_historyNode(id_rel, NULL);

      else if(!current_history->valid){
        strcpy(current_history->id_rel, id_rel);
        current_history->num = update;
        current_history->valid = true;

      }
      
    }

  }






  /* OLD
  historyNode_t *current_history = vertex->head_history, *prev;

  if(!validHistoryExists(vertex, id_rel) && update==1){
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
*/
}

vertex_t *returnValidVertex(vertex_t **head, char *id){
  vertex_t *current = NULL;

  for(current = *head; current!=NULL; current = current->next)
    if(strcmp(current->id_ent, id)==0 && current->valid)
      return current;
  
  return NULL;
}

node_t *returnValidNode(vertex_t **head, char *father, char *id){
    vertex_t *current = returnValidVertex(head, father);
    node_t *current_node;

    for(current_node = current->head_node; current_node!=NULL && strcmp(current_node->id_dest, id) != 0; current_node = current_node->next);

    if (current_node==NULL || !current_node->valid)
      return NULL;
    else
      return current_node;
}

node_t *returnValidNodeWithRel(vertex_t **head, char *father, char *id, char *id_rel){
    vertex_t *current = returnValidVertex(head, father);
    node_t *current_node;

    for(current_node = current->head_node; current_node!=NULL; current_node = current_node->next){
      if(strcmp(current_node->id_dest, id) == 0 && strcmp(current_node->id_rel, id_rel) == 0)
        break;
    }

    if (current_node==NULL || !current_node->valid)
      return NULL;
    else
      return current_node;
}


/* CORE */
void addent(vertex_t **head, char *id_ent){
  vertex_t *current_vertex;

  if (validVertexExists(head, id_ent))
        return;

  else{

    if (*head == NULL)
      *head = new_vertex(id_ent, NULL);

    else{

      for(current_vertex = *head; current_vertex->next != NULL && current_vertex->valid==true; current_vertex = current_vertex->next);

      if(current_vertex->valid==false){
        strcpy(current_vertex->id_ent, id_ent);
        current_vertex->valid = true;
      }

      else
        current_vertex->next = new_vertex(id_ent, NULL);
      }
    }
}




void delent(vertex_t **head, char *id_ent) {
    vertex_t *current, *tmp;
    historyNode_t *current_history;

    if (!validVertexExists(head, id_ent) || *head == NULL)
        return;

    else{

      current = returnValidVertex(head, id_ent);
      tmp = current;

      for (current = *head; current!=NULL; current = current->next) {
        if(current->head_history==NULL)
          continue;
        else{
          for (current_history = current->head_history; current_history!=NULL; current_history = current_history->next)
            delrel(head, id_ent, current->id_ent, current_history->id_rel);
        }
      }

      tmp->valid = false;

      return;
    }
}

void addrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {
    chronoAdd(id_rel);

  if (!validVertexExists(head, id_orig) || !validVertexExists(head, id_dest) || !(strcmp(id_orig, id_dest)) || validRelExists(head, id_orig, id_dest, id_rel))
      return;

  vertex_t *current_vertex, *toBeUpdated;

  node_t *current_node, *tmp;

  current_vertex = returnValidVertex(head, id_orig);
  toBeUpdated = returnValidVertex(head, id_dest);

  if (current_vertex->head_node == NULL)
      current_vertex->head_node = new_node(id_rel, id_dest, NULL);

  else {
      for (current_node = current_vertex->head_node; current_node->next != NULL && current_node->valid==true; current_node = current_node->next);

        tmp = current_node;
        if (current_node->valid==false){
          strcpy(current_node->id_dest, id_dest);
          strcpy(current_node->id_rel, id_rel);
          current_node->valid=true;
        }

        else
          current_node->next = new_node(id_rel, id_dest, NULL);
     
  }

  updateHistory(toBeUpdated, id_rel, 1);

  return;
}

void delrel(vertex_t **head, char *id_orig, char *id_dest, char *id_rel) {
  if (!validRelExists(head, id_orig, id_dest, id_rel))
    return;
  if (!validVertexExists(head, id_orig))
    return;
  if (!validVertexExists(head, id_dest))
    return;

  vertex_t *current, *toBeUpdated;
  node_t *current_node;

  toBeUpdated = returnValidVertex(head, id_dest);
  current_node = returnValidNodeWithRel(head, id_orig, id_dest, id_rel);

  updateHistory(toBeUpdated, id_rel, -1);
  current_node->valid=false;

  return;
}

void report(vertex_t **head) {
    vertex_t *current_vertex;
    historyNode_t *current_history;
    chrono_t *chrono = CHRONO;
    int relation_c = 0;
    int max = 0;
/*
    for (current_vertex = *head; current_vertex != NULL; current_vertex = current_vertex->next)
        if (current_vertex->head_node != NULL && current_vertex->head_node->valid && current_vertex->valid)
            relation_c++;

    if (relation_c == 0)
        printf("none\n");
*/


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
    if(current_vertex->valid==false)
      continue;

    printf("%s: \n", current_vertex->id_ent);

    for(current_history=current_vertex->head_history; current_history!=NULL; current_history=current_history->next){
      if(current_history->valid)
        printf("- id_rel: %s; num: %d; \n", current_history->id_rel, current_history->num);
    }

  }

}
