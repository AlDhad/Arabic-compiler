#include "quadruplet.h"

// Function Implementations
qTable* initialiserTQ() {
    qTable* TQ = (qTable*)malloc(sizeof(qTable));
    if (TQ != NULL) {
        TQ->tete = NULL;
        TQ->queue = NULL;
        TQ->nb = 0;
    }
    return TQ;
}

void afficherTQ(qTable *TQ) {
    qNoeud* current = TQ->tete;
    printf("\nLes quadruplets :\n");
    while (current != NULL) {
        afficherQ(current);
        current = current->next;
    }
}

void supprimer_TQ(qTable *TQ) {
    qNoeud* current = TQ->tete;
    while (current != NULL) {
        qNoeud* temp = current;
        current = current->next;
        free(temp);
    }
    TQ->tete = NULL;
    TQ->queue = NULL;
    TQ->nb = 0;
}

void inserer_TQ(qTable *TQ, qNoeud *Q) {
    if (TQ != NULL && Q != NULL) {
        Q->next = NULL;
        if (TQ->queue != NULL) {
            TQ->queue->next = Q;
        } else {
            TQ->tete = Q;
        }
        TQ->queue = Q;
        TQ->nb++;
    }
}

qNoeud* rechercherTQ(qTable *TQ, int address) {
    qNoeud* current = TQ->tete;
    while (current != NULL) {
        if (current->adresse == address) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

qNoeud* creer_Q(char operateur[], char* operande1, char* operande2, char* resultat, int adresse) {
    qNoeud* Q = (qNoeud*)malloc(sizeof(qNoeud));
    if (Q != NULL) {
        strncpy(Q->operateur, operateur, sizeof(Q->operateur) - 1);
        strncpy(Q->operande1, operande1, sizeof(Q->operande1) - 1);
        strncpy(Q->operande2, operande2, sizeof(Q->operande2) - 1);
        strncpy(Q->resultat, resultat, sizeof(Q->resultat) - 1);
        Q->adresse = adresse;
        Q->next = NULL;
    }
    return Q;
}

void afficherQ(qNoeud* Q) {
    if (Q != NULL) {
        printf("Adresse: %d, Operateur: %s, Operande1: %s, Operande2: %s, Resultat: %s\n",
               Q->adresse, Q->operateur, Q->operande1, Q->operande2, Q->resultat);
    } else {
        printf("\n Empty ! \n");
    }
}

void afficherTQDansFichier(qTable *TQ, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "w");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", nomFichier);
        return;
    }

    qNoeud* current = TQ->tete;
    fprintf(fichier, "\nLes quadruplets :\n");

    while (current != NULL) {
        afficherQDansFichier(current, fichier);
        current = current->next;
    }

    fclose(fichier);
}

void afficherQDansFichier(qNoeud* Q, FILE *fichier) {
    if (Q != NULL) {
        fprintf(fichier, "%d -( %s,  %s,  %s,  %s)\n",
                Q->adresse, Q->operateur, Q->operande1, Q->operande2, Q->resultat);
    } else {
        fprintf(fichier, "\n Empty ! \n");
    }
}
void mettreAJourResultat(qTable *TQ, int adresse, const char *nouveauResultat) {
    qNoeud* Q = rechercherTQ(TQ, adresse);
    if (Q != NULL) {
        strncpy(Q->resultat, nouveauResultat, sizeof(Q->resultat) - 1);
        Q->resultat[sizeof(Q->resultat) - 1] = '\0'; // Ensure null-termination
    } else {
        printf("Quadruplet with address %d not found.\n", adresse);
    }
}