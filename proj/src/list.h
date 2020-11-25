#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct Node{

  void * data;
  struct Node * ptNext;

}Node;

//lista simplesmente ligada

typedef struct List{
  struct Node * head;

}List;

//Metodos de Node
Node* node(void * d); //alocar memoria para um novo Node, com data d

//Metodos de List
List listInit(); //construtor de um nova lista
void freeList(struct List *l); // liberta toda a memoria alocada pela lista legal
void insertFront(struct List * l , void * d); // insere um novo elemento na lista l com data d
void insertList(struct List *l1, struct List *l2); //aloca na lista l1 todos os elemetos de l2, enquanto os consome
bool isEmpty(struct List *l); // returna se a lista l esta vazia
bool contain(struct List *l, void * d , size_t size); //retorna se a lista l contem algum elemento com data d, de tamanho (bytes) especificado por size (por ser template)
void * popFront(struct List *l); // elimina o primeiro valor da lista l e retorna um apontador esse valor
void * getHead(struct List *l);
void removeAll(struct List *l); //  remove todos os elementos da lista l;
void * get(struct List *l, size_t i); // retorna apontador para elemento na posicao i;
