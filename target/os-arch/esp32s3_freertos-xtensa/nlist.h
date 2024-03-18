#ifndef _NLIST_H_
#define _NLIST_H_

#include <stdio.h>

struct nlist { /* table entry: */
    struct nlist *next; /* next entry in chain */
    char *name; /* defined name */
    char *defn; /* replacement text */
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* hash: form hash value for string s */
unsigned hash(char *s);

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s);

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn);

//char *strdup(char *s); /* make a duplicate of s */

#endif
