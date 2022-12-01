#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heap.h"
#include "hashmap.h"
#include "list.h"
#include "treemap.h"
typedef struct nodo{
   void* data;
   int priority;
}heapElem;

typedef struct Heap{
  heapElem* heapArray;
  int size;
  int capac;
} Heap;


void* heap_top(Heap* pq){
   if (pq->size == 0) return NULL;
   return pq->heapArray[0].data;

}



void heap_push(Heap* pq, void* data, int priority){
    heapElem aux;
   int pos = pq->size;
   if (pq->size==pq->capac){
        pq->heapArray=realloc(pq->heapArray,(pq->capac*2+1) *sizeof(heapElem));
        pq->capac=pq->capac*2+1;
    }
   int pAux;
	pq->heapArray[pos].data = data;
	pq->heapArray[pos].priority = priority;

	while( pos > 0 ){
      pAux = round(pos/2);
	   if(pq->heapArray[pos].priority > pq->heapArray[pAux].priority){
		aux = pq->heapArray[pos];
		pq->heapArray[pos] =  pq->heapArray[pAux];
      pq->heapArray[pAux] =  aux;
	   }
	   pos = pAux;
	}
	pq->size++;

}


void heap_pop(Heap* pq){


   int aux=0;
   while(aux!=pq->size){

      pq->heapArray[aux].data=pq->heapArray[aux+1].data;
      pq->heapArray[aux].priority=pq->heapArray[aux+1].priority;
      aux++;
   }
   int pAux;
   heapElem aux1;
	while( aux > 0 ){
      pAux = round(aux/2);
	   if ( pq->heapArray[aux].priority > pq->heapArray[pAux].priority){
		aux1 = pq->heapArray[aux];
		pq->heapArray[aux] =  pq->heapArray[pAux];
      pq->heapArray[pAux] =  aux1;
	   }
	   aux = pAux;
	}
   pq->size--;

}

Heap* createHeap(){
  Heap *crear=(Heap*)malloc(sizeof(Heap));
  crear->heapArray=(heapElem*)calloc(3,sizeof(heapElem));
  crear->size=0;
  crear->capac=3;
  return crear;
}
