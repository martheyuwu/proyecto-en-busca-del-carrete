#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include "list.h"
#include "treemap.h"
#include "heap.h"
typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equalmap(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    Pair *newpair=createPair(key,value);
    long pos=hash(key,map->capacity);
    if (map->buckets[pos]!=NULL){
      while(1){
        if (pos==map->capacity)
          pos=0;
        if (map->buckets[pos]==NULL){
            map->buckets[pos]=newpair;
            map->current=pos;
            map->size++;
            break;
        }
        pos++;
      }
    }
    else{
      map->buckets[pos]=newpair;
      map->current=pos;
      map->size++;
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    long capacity=map->capacity;
    Pair **aux=(Pair**)calloc(map->capacity,sizeof(Pair*));
    aux=map->buckets;
    map->capacity*=2;
    map->buckets=(Pair**)calloc(map->capacity,sizeof(Pair*));
    map->size=0;
    long pos=0;
    while (pos!=capacity){
      if (aux[pos]!=NULL){
        if (aux[pos]->key!=NULL)
          insertMap(map,aux[pos]->key,aux[pos]->value);
      }
      pos++;
    }
}


HashMap * createMap(long capacity) {
    HashMap *hash=(HashMap*)malloc(sizeof(HashMap));
    hash->buckets=(Pair**)calloc(capacity,sizeof(Pair*));
    hash->size=0;
    hash->capacity=capacity;
    hash->current=-1;
    return hash;
}

void eraseMap(HashMap * map,  char * key) {
  long pos=hash(key,map->capacity);
  long aux=pos;
  while (1){
    if (pos==map->capacity)
      pos=0;
    if (map->buckets[pos]==NULL)
      break;
    if (is_equalmap(map->buckets[pos]->key,key)==1){
      map->buckets[pos]->key=NULL;
      map->size--;
      break;
    }
    pos++;
    if (pos==aux)
      break;
  }
}

Pair * searchMap(HashMap * map,  char * key) {
    long pos=hash(key,map->capacity);
    while (1){
      if (pos==map->capacity)
        pos=0;
      if (map->buckets[pos]==NULL)
        return NULL;
      if (is_equalmap(map->buckets[pos]->key,key)==1){
        map->current=pos;
        return map->buckets[pos];
      }
      pos++;
    }
}

Pair * firstMap(HashMap * map) {
    long pos=0;
    while (1){
      if (map->buckets[pos]!=NULL){
        if (map->buckets[pos]->key!=NULL){
          map->current=pos;
          return map->buckets[pos];
        }
      }
      pos++;
    }
}

Pair * nextMap(HashMap * map) {
    long pos=map->current+1;
    while (1){
      if (pos==map->capacity)
        return NULL;
      if (map->buckets[pos]!=NULL){
        map->current=pos;
        return map->buckets[pos];
      }
      pos++;
    }
}
