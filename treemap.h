#ifndef TREEMAP_h
#define TREEMAP_h

typedef struct TreeMap TreeMap;

typedef struct Pairt {
     void * key;
     void * value;
} Pairt;

TreeMap * createTreeMap(int (*lower_than_int) (void* key1, void* key2));

void insertTreeMap(TreeMap * tree, void* key, void * value);

void eraseTreeMap(TreeMap * tree, void* key);

Pairt * searchTreeMap(TreeMap * tree, void* key);

Pairt * upperBound(TreeMap * tree, void* key);

Pairt * firstTreeMap(TreeMap * tree);

Pairt * nextTreeMap(TreeMap * tree);

#endif /* TREEMAP_h */
