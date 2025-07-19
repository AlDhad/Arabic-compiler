#include "quadruplet.h"
#include "pile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void updateLabel(qNoeud *Q, int adresse){
    char new[20];
    sprintf(new, "%d", adresse);
    strcpy(Q->operande1, new);
    return;
}


qPile* initialiserP() {
    qPile *pile = malloc(sizeof(qPile));
    pile->sommet = -1;
    return pile;
}
int Pempty(qPile *P){
    if(P->sommet == -1){
        return 1; //it is empty
    } 
    return 0;
}

int Pfull(qPile *P){
    if(P->sommet ==  99){
        return 1;
    }
    return 0;
}

void push(qPile *P, qNoeud *Q){
    //add to stack 
    if(!Pfull(P)){
        //if stack isnt full
        P->sommet++;
        P->items[P->sommet] = Q;
        //printf("Q is inserted no worries <3 \n \n ");
    } 
    else {
        printf("cant pop, is full \n");
    }
}

qNoeud* pop(qPile *P){
        //add to stack 
    if(!Pempty(P)){
        qNoeud *Q = P->items[P->sommet];
        //printf("POPED");
        P->sommet--;
        return Q;
    }
    printf("cant pop, is empty \n");
    return NULL;
}

void afficherP(qPile* P){
    printf("QUADRUPLET STACK : \n");
    if(!Pempty(P)){
        for(int i = 0; i <= P->sommet; i++){
            afficherQ(P->items[i]);
        }
    } else {
        printf("STACK EMPTY \n");
    }

}