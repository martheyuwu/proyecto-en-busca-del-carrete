#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashmap.h"
#include "hashmap.h"
#include "list.h"
#include "treemap.h"
typedef struct Node Node;

struct Node {
    void * data;
    Node * next;
    Node * prev;
};

struct List {
    Node * head;
    Node * tail;
    Node * current;
};

typedef List List;

Node * createNode(void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

List * createList() {
  List* lista = (List*) malloc(sizeof(List));
  assert(lista != NULL);
  lista->head = NULL;
  lista->tail = NULL;
  lista->current = NULL;

  return lista;
}

void * firstList(List * list) {
  if( !list->head ) return NULL; //Si la cabeza no existe retorno nulo
  list->current = list->head;
  return list->current->data;
}

void * nextList(List * list) {
  if( !list->current ) return NULL; //Si el nodo actual no existe retorno nulo
  if( !list->current->next ) return NULL; //Si el siguiente al actual no existe retorno nulo
  list->current = list->current->next;
  return list->current->data;
}

void * lastList(List * list) {
  if( !list->tail ) return NULL; // Si la cola no existe retorno nulo
  list->current = list->tail;
  return list->current->data;
}

void * prevList(List * list) {
  if( !list->current ) return NULL; //Si el nodo actual no existe retorno nulo
  if( !list->current->prev ) return NULL; //Si el anterior al actual no existe retorno nulo
  list->current = list->current->prev;
  return list->current->data;
}

void pushFront(List * list, void * data) {
  Node * nodoNuevo = createNode(data);

  nodoNuevo->next = list->head;
  if( list->head ){ //Si el head existe, conecto el prev del head con el nuevo nodo
    list->head->prev = nodoNuevo;
  }
  else{ //Si el head no existe, la cola de la lista será el nuevo nodo
    list->tail = nodoNuevo;
  }
  list->head = nodoNuevo;
}

void pushBack(List * list, void * data) {
    list->current = list->tail;
    pushCurrent(list,data);
}

void pushCurrent(List * list, void * data) {
  Node * nodoNuevo = createNode(data);

  if( !list->current ){ //Si el nodo actual no existe, agregar el elemento al frente
      pushFront(list,data);
  }
  else{
      nodoNuevo->next = list->current->next; //Que el "next" del nuevo nodo apunte al siguiente del actual
      nodoNuevo->prev = list->current;       //Que el "prev" del nuevo nodo apunte al actual
      list->current->next = nodoNuevo;       //Que el "next" del nodo actual apunte al nuevo
      list->current = nodoNuevo;             //Que el nodo actual sea el nodo nuevo
      if( !list->current->next ){ //Si el nodo actual era la cola, entonces asignarle nuevamente la cola
        list->tail = nodoNuevo;              //Que la cola sea el nuevo
      }
      else{//Si existía uno nodo depués del actual, el prev de ese nodo que apunte al nuevo
        nodoNuevo->next->prev = nodoNuevo;   //Que el "prev" del siguiente nodo del actual apunte al nuevo
      }

  }

}

void * popFront(List * list) {
    list->current = list->head;
    return popCurrent(list);
}

void * popBack(List * list) {
    list->current = list->tail;
    return popCurrent(list);
}

void * popCurrent(List * list) {
    if( !list->current ) return NULL; //Si no existe nodo actual

    void * aux = list->current->data;
    if(list->tail == list->head){ //Si solo existe un nodo
        free( list->current );
        list->current = NULL;
        list->head = NULL;
        list->tail = NULL;
    }
    else{
        if( list->current == list->head ){ //Si el nodo actual es la cabeza
            list->current = list->head->next;
            list->head = list->current;
            free( list->head->prev );
            list->head->prev = NULL;
        }
        else{
            if( list->current == list->tail){ //Si el nodo actual es la cola
                list->current = list->tail->prev;
                list->tail = list->current;
                free( list->tail->next );
                list->tail->next = NULL;
            }
            else{ //Si el nodo actual se encuentra en medio de la lista
                list->current = list->current->next;
                list->current->prev = list->current->prev->prev;
                free( list->current->prev->next );
                list->current->prev->next = list->current;
            }
        }
    }
    return aux;

}

void cleanList(List * list) {
    while (list->head != NULL) {
        popFront(list);
    }
}
