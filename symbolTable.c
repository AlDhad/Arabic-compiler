#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include <stdbool.h>


// Création de la table des symboles

TableSymbole *creerTableSymbole()
{
    TableSymbole *table = (TableSymbole *)malloc(sizeof(TableSymbole));
    if (table == NULL)
    {
        fprintf(stderr, "Erreur: impossible d'allouer la mémoire pour la table des symboles\n");
        exit(1);
    }

    table->debut = NULL;
    table->fin = NULL;
    table->taille = 0;

    return table;
}

// Création d'un symbole
Symbole *creerSymbole(CategorieSymbole categorie, char *nom, char *type, char *valeur, int numLigne, int taille)

{
    Symbole *symbole = (Symbole *)malloc(sizeof(Symbole));
    if (symbole == NULL)
    {
        fprintf(stderr, "Erreur: impossible d'allouer la mémoire pour le symbole\n");
        exit(1);
    }

    symbole->categorie = categorie;
    symbole->nom = strdup(nom);
    symbole->type = strdup(type);
    symbole->valeur = strdup(valeur);
    symbole->nnumLigne = numLigne;
    symbole->taille = taille;

    if (categorie == FUNCTION)
    {
        symbole->infoFonction = (InfoFonction *)malloc(sizeof(InfoFonction));
        if (symbole->infoFonction == NULL)
        {
            fprintf(stderr, "Erreur: impossible d'allouer la mémoire pour les informations de la fonction\n");
            exit(1);
        }

        symbole->infoFonction->nbParametres = 0;
        symbole->infoFonction->parametres = NULL;
    }
    else // Pour le cas où le symbole est une variable ou une constante
    {
        symbole->infoFonction = NULL;
    }
    return symbole;
}
//fonction d'insertion d'un symbole dans la table des symboles
bool rechercherParametre(Symbole *fonction, const char *nomParam, Parametre **parametreTrouve) {
    if (fonction == NULL || fonction->categorie != FUNCTION || fonction->infoFonction == NULL) {
        *parametreTrouve = NULL;
        return false;
    }

    Parametre *current = fonction->infoFonction->parametres;
    while (current != NULL) {
        if (strcmp(current->nom, nomParam) == 0) {
            *parametreTrouve = current;
            return true;
        }
        current = current->suivant;
    }

    *parametreTrouve = NULL;
    return false;
}
void insererSymbole(TableSymbole *table, Symbole *symbole) {
    if (table->debut == NULL) {
        // La table est vide, insérer au début
        table->debut = symbole;
        table->fin = symbole;
        symbole->precedent = NULL;
        symbole->suivant = NULL;
    } else {
        // La table n'est pas vide, insérer à la fin
        table->fin->suivant = symbole;
        symbole->precedent = table->fin;
        symbole->suivant = NULL;
        table->fin = symbole;
    }
    table->taille++;
}

//fonction d'affichage de la table des symboles
void afficherTableSymbole(TableSymbole *table) {
    Symbole *current = table->debut;
    printf("Table des symboles (taille: %zu):\n", table->taille);
    printf("---------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-20s | %-10s | %-10s | %-15s | %-15s | %-30s |\n", "Nom", "Type", "Catégorie", "Adresse mémoire", "Valeur Init", "Infos Fonction");
    printf("---------------------------------------------------------------------------------------------------------------------------\n");
    while (current != NULL) {
        char infosFonction[256] = "";
        if (current->categorie == FUNCTION) {
            snprintf(infosFonction, sizeof(infosFonction), "Nb Params: %d", current->infoFonction->nbParametres);
            Parametre *param = current->infoFonction->parametres;
            while (param != NULL) {
                char paramInfo[64];
                snprintf(paramInfo, sizeof(paramInfo), ", %s: %s", param->nom, param->type);
                strncat(infosFonction, paramInfo, sizeof(infosFonction) - strlen(infosFonction) - 1);
                param = param->suivant;
            }
        }
        printf("| %-20s | %-10s | %-10s | %-15d | %-15s | %-30s |\n", current->nom, current->type, categorieToString(current->categorie), current->taille, current->valeur, infosFonction);
        current = current->suivant;
    }
    printf("---------------------------------------------------------------------------------------------------------------------------\n");
}

// Fonction pour convertir la catégorie en chaîne de caractères
const char* categorieToString(CategorieSymbole categorie) {
    switch (categorie) {
        case VARIABLE: return "VARIABLE";
        case CONSTANTE: return "CONSTANTE";
        case FUNCTION: return "FUNCTION";
        default: return "UNKNOWN";
    }
}

//fonction de recherche d'un symbole
//utilisée surtout avant la création d'un symbole pour vérifier si un symbole avec le même nom existe déjà, pour éviter les doublons
bool rechercherSymbole(TableSymbole *table, const char *nom, Symbole **symboleTrouve) {
    Symbole *current = table->debut;
    while (current != NULL) {
        if (strcmp(current->nom, nom) == 0) {
            *symboleTrouve = current;
            return true;
        }
        current = current->suivant;
    }
    *symboleTrouve = NULL;
    return false;
}
//fonction de suppression d'un symbole
void supprimerSymbole(TableSymbole *table, const char *nom) {
    Symbole *symboleASupprimer;
    if (rechercherSymbole(table, nom, &symboleASupprimer)) {
        if (symboleASupprimer->precedent != NULL) {
            symboleASupprimer->precedent->suivant = symboleASupprimer->suivant;
        } else {
            table->debut = symboleASupprimer->suivant;
        }

        if (symboleASupprimer->suivant != NULL) {
            symboleASupprimer->suivant->precedent = symboleASupprimer->precedent;
        } else {
            table->fin = symboleASupprimer->precedent;
        }

        free(symboleASupprimer->nom);
        free(symboleASupprimer->type);
        free(symboleASupprimer->valeur);
        if (symboleASupprimer->infoFonction != NULL) {
            free(symboleASupprimer->infoFonction);
        }
        free(symboleASupprimer);

        table->taille--;
    } else {
        printf("Symbole non trouvé: %s\n", nom);
    }
}

void ajouterParametre(Symbole *fonction, const char *nom, const char *type) {
    if (fonction == NULL) {
        fprintf(stderr, "Erreur: pointeur de fonction NULL\n");
        return;
    }

    if (fonction->categorie != FUNCTION) {
        fprintf(stderr, "Erreur: le symbole n'est pas une fonction\n");
        return;
    }

    if (fonction->infoFonction == NULL) {
        fprintf(stderr, "Erreur: infoFonction non initialisé\n");
        fonction->infoFonction = (InfoFonction *)malloc(sizeof(InfoFonction));
        if (fonction->infoFonction == NULL) {
            fprintf(stderr, "Erreur: impossible d'allouer la mémoire pour infoFonction\n");
            exit(1);
        }
        fonction->infoFonction->nbParametres = 0;
        fonction->infoFonction->parametres = NULL;
    }

    Parametre *param = (Parametre *)malloc(sizeof(Parametre));
    if (param == NULL) {
        fprintf(stderr, "Erreur: impossible d'allouer la mémoire pour le paramètre\n");
        exit(1);
    }
    param->nom = strdup(nom);
    param->type = strdup(type);
    param->suivant = NULL;

    if (fonction->infoFonction->parametres == NULL) {
        fonction->infoFonction->parametres = param;
    } else {
        Parametre *current = fonction->infoFonction->parametres;
        while (current->suivant != NULL) {
            current = current->suivant;
        }
        current->suivant = param;
    }
    fonction->infoFonction->nbParametres++;
}
//fonction pour afficher les informations d'une fonction
void afficherInfoFonction(Symbole *symbole) {
    InfoFonction *info = symbole->infoFonction;
    printf("Nom de la fonction: %s\n", symbole->nom);
    printf("Type de retour: %s\n", symbole->type);
    printf("Adresse mémoire: %d\n", symbole->taille);
    printf("Nombre de paramètres: %d\n", info->nbParametres);
    printf("Liste des paramètres:\n");
    Parametre *current = info->parametres;
    while (current != NULL) {
        printf("Paramètre: %s, Type: %s\n", current->nom, current->type);
        current = current->suivant;
    }
}

// int main() {
//     Symbole *symbole = creerSymbole(FUNCTION, "main", "void", "", 1, 0);
//     Symbole *symbole2 = creerSymbole(VARIABLE, "x", "int", "8", 4, 0);
    
//     TableSymbole *table = creerTableSymbole();
//     if (table != NULL) {
//         printf("Table des symboles créée\n");
//     }
//     insererSymbole(table, symbole);
//     insererSymbole(table, symbole2);
//     afficherTableSymbole(table);
//     ajouterParametre(symbole->infoFonction, "a", "int");
//     afficherTableSymbole(table);
//     printf("affihcage des informations de la fonction\n");
//     afficherInfoFonction(symbole);
//     return 0;
// }
int isIntOrfloat(int x){
    int result = 0 ; 
    if (x == 1 || x == 2){
        result = 1 ; 
    }
    return result ; 
}

int isInt(int x){
    int result = 0 ; 
    if (x == 1){
        result = 1 ; 
    }
    return result ; 
}


int isFlt(int x){
    int result = 0 ; 
    if (x == 2){
        result = 1 ; 
    }
    return result ; 
}
void SetTypeSymbol(Symbole* symbole, char* type) {
    if (symbole != NULL) {
        symbole->type = type;
    }
}

void SetNomSymbol(Symbole* symbole, char* nom) {
    if (symbole != NULL) {
        free(symbole->nom);
        symbole->nom = strdup(nom);
    }
}

void SetValueSymbol(Symbole* symbole, char* value) {
    if (symbole != NULL) {
        free(symbole->valeur);
        symbole->valeur = strdup(value);
    }
}
