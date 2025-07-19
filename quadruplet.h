#ifndef quadruplet_H
#define quadruplet_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct qNoeud
{
    char operateur[30];
    char operande1[255];
    char operande2[255];
    char resultat[255];

    int adresse;

    struct qNoeud *next;

} qNoeud;

typedef struct qTable
{
    struct qNoeud *tete;
    struct qNoeud *queue;
    int nb;

} qTable;



qTable* initialiserTQ();
void afficherTQ(qTable *TQ);
void supprimer_TQ(qTable *TQ);
void inserer_TQ(qTable *TQ, qNoeud *Q);
qNoeud* rechercherTQ(qTable *TQ,  int address);
qNoeud* creer_Q(char operateur[], char* operande1, char* operande2, char* resultat, int adresse);
void afficherQ(qNoeud* Q);

void afficherTQDansFichier(qTable *TQ, const char *nomFichier);
void afficherQDansFichier(qNoeud* Q, FILE *fichier) ;

void mettreAJourResultat(qTable *TQ, int adresse, const char *nouveauResultat);

#endif