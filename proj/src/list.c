#include "list.h"


Node* node(void * d){

  Node * n = malloc(sizeof(Node));
  n->data = d;
  return n;
}

List listInit(){

  struct List l;
  l.head = NULL;
  return l;
}

void freeList(struct List *l){

  removeAll(l);
}

void insertFront(struct List * l , void * d){

  struct Node *n = node(d);
  struct Node *prim = l->head;
  l->head = n;
  n->ptNext = prim;
}

//allocate elements on l1, while consuming those elements from l2

void insertList(struct List *l1, struct List *l2){

  if(l1 == l2)
    return;
  while(!isEmpty(l2)){
    insertFront(l1,popFront(l2));
  }
  freeList(l2);
}

bool isEmpty(struct List *l){

  return l->head == NULL;
}

bool contain(struct List *l, void * d , size_t size){

  struct Node * pt = l->head;
  while(pt && !memcmp(pt->data,d,size))
    pt = pt -> ptNext;
  return pt && !memcmp(pt->data,d,size);
}

void * popFront(struct List *l){

  if(l->head == NULL)
    return NULL;

  void * data = l->head->data;
  struct Node * n = l->head;
  l->head = l->head->ptNext;
  free(n);
  return data;
}

void * getHead(struct List *l){

  if(l->head == NULL)
    return NULL;
  return l->head->data;
}

void removeAll(struct List *l){

  while(!isEmpty(l))
    popFront(l);
}

void * get(struct List *l, size_t i){

  struct Node * pt = l->head;
  size_t n = 0;

  while(pt && n != i){
    pt = pt->ptNext;
    ++n;
  }
  if(pt == NULL)
    return NULL;
  return pt->data;
}
