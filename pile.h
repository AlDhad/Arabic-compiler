#ifndef pile_H
#define pile_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quadruplet.h"

typedef struct qPile 
{
    int sommet;
    qNoeud* items[100];
} qPile;

void updateLabel(qNoeud *Q, int adresse);
qPile* initialiserP();
void afficherP(qPile* P);
int Pempty(qPile *P);
int Pfull(qPile *P);
void push(qPile *P, qNoeud *Q);
qNoeud* pop(qPile *P);



#endif