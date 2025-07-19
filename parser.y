%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <math.h>
    #include "symbolTable.h"
    #include "quadruplet.h"
    #include "pile.h"
    #include  <ctype.h>



    //declarations 

    typedef struct {
    int baseType;      // ENTIER, FLOTTANT, STRING, etc.
    bool isArray;      // Whether it's an array
    bool isConst;      // Whether it's a constant
    int arraySize;     // Size if it's an array
    char* structName;  // Name if it's a struct
} TypeInfo;
    TableSymbole *TS;
    qTable *TQ;
    qPile *P;
    

    qNoeud* quad;
    Symbole* node;
    char* currentFunctionCalled=NULL;
    ParametresList* currentParametresList;
    int sauvLabel;
    char* saveFunctionDec=NULL;
    bool itReturn=false;
    int sauvDebut;
    int sauvFin;

    int qC = 0;
    

    extern int colnum ;
    extern int lignenum ;

    extern FILE *yyin;
    int yylex();
    void yyerror(const char *s);
    int line = 1;        // Numéro de ligne courant
    int linecol = 0;         // Numéro de colonne courant
    char *yyin_filename = NULL;
    int sauv = 0;
    int sauvline = 1;

    #define TYPE_ENTIER "ENTIER"
    #define TYPE_FLOTTANT "FLOTTANT"
    #define TYPE_CHAR "CHAR"
    #define TYPE_STRING "STRING"
    #define TYPE_BOOLEAN "BOOLEAN"

// Modified type checking function to work with your symbol table
bool areTypesCompatible(const char* type1, const char* type2) {
    if (strcmp(type1, type2) == 0) {
        return true;
    }
    
    // Allow implicit conversion from int to float
    if (strcmp(type1, TYPE_FLOTTANT) == 0 && strcmp(type2, TYPE_ENTIER) == 0) {
        return true;
    }
    
    return false;
}

// Modified function to check if an identifier is declared
bool isDeclared(const char* id) {
    Symbole* found;
    return rechercherSymbole(TS, id, &found);
}

// Function to get type of an identifier
char* getIdentifierType(const char* id) {
    Symbole* found;
    if (rechercherSymbole(TS, id, &found)) {
        return found->type;
    }
    return NULL;
}

// Function to determine if a literal is valid for a given type
bool isValidLiteralForType(const char* literal, const char* type) {
    if (strcmp(type, TYPE_ENTIER) == 0) {
        // Check if string contains only digits
        for (int i = 0; literal[i] != '\0'; i++) {
            if (!isdigit(literal[i])) return false;
        }
        return true;
    }
    else if (strcmp(type, TYPE_FLOTTANT) == 0) {
        // Check if string is a valid float
        char* endptr;
        strtof(literal, &endptr);
        return *endptr == '\0';
    }
    else if (strcmp(type, TYPE_BOOLEAN) == 0) {
        // Check if string is "true" or "false"
        return (strcmp(literal, "true") == 0 || strcmp(literal, "false") == 0);
    }
    // Add more type checks as needed
    return false;
}
TypeInfo currentType; 
    char currentFunction[256]; 
    bool inLoop = false; 
    int loopNestingLevel = 0;
void semanticError(const char* message, int line) { 
    fprintf(stderr, "Semantic error at line %d: %s\n", line, message); 
    exit(1);
}
ParametresList* creerParametresList() {
    ParametresList* list = (ParametresList*)malloc(sizeof(ParametresList));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

bool ajouterParametreUnion(ParametresList* list, const char* nom, const char* type) {
    // Check if the parameter already exists
    ParametreNode* current = list->head;
    while (current) {
        if (strcmp(current->parametre.nom, nom) == 0) {
            return false; // Parameter already exists
        }
        current = current->next;
    }

    // Add the new parameter
    ParametreNode* newNode = (ParametreNode*)malloc(sizeof(ParametreNode));
    newNode->parametre.nom = strdup(nom);
    newNode->parametre.type = strdup(type);
    newNode->next = NULL;

    if (list->tail) {
        list->tail->next = newNode;
    } else {
        list->head = newNode;
    }
    list->tail = newNode;
    list->count++;
    return true;
}

void libererParametresList(ParametresList* list) {
    ParametreNode* current = list->head;
    while (current) {
        ParametreNode* next = current->next;
        free(current->parametre.nom);
        free(current->parametre.type);
        free(current);
        current = next;
    }
    free(list);
}


%}
%code requires {
    #include "symbolTable.h"  // Include it again here to make sure types are available for the union
}

%union {
    int type;
    char str[255];
    char charv;
    int intv; 
    float flt;
    bool boolean;
    struct {
        char* valeur;
        char* nom;
        int type;
    } structure;
    ParametreUnion parametreUnion;
    ParametreNode* parametreNode;
    ParametresList* parametresList;
}




%token  DEBUT FIN WHILE FOR
%token  PRINT INPUT FONCTION VIDE RETURN
%token  COMMENT COMMENT_PLUS
%token  OR AND NOT
%token  IF ELSE ELIF
%token  DEB_CORPS FIN_CORPS
%token<boolean> TRUE FALSE
%token  PAR_OUV PAR_FERM
%token<charv> CARACTERE
%token<str> CHAINE
%token<intv> INT
%token<flt> FLOAT 
%token  DEB_TABLEAU FIN_TABLEAU TABLE ENREGISTREMENT POINTEUR
%token<type> CONST ENTIER FLOTTANT STRING CHAR BOOLEAN
%token<str> ID
%token INCREM DECREM PLUS DIV MOINS MULT MOD PUISS ASSIGN
%token  SEMICOLON
%token  INF INF_EGAL SUP SUPP_EGAL EQUAL NOT_EQUAL
%token  FROM
%token  TO
%token  VIRGULE 

%type<type> type
%type<structure> valeur
%type<structure> tableau
%type<structure> variable
%type<structure> expression
%type<structure> incrementation
%type<structure> parametreCall
%type<structure> fonction
%type<structure> call
%type<parametreUnion> parametre
%type<parametresList> parametres
%type<parametreUnion> declarationfonction

%right OR
%right AND
%right NOT INCREM DECREM
%right INF INF_EGAL SUP SUPP_EGAL EGALE PASEGALE
%right PLUS MOINS
%right MUL DIV MOD
%left  PUISS 
%right PAR_OUV DEB_TABLEAU DEB_CORPS
%right PAR_FERM FIN_TABLEAU FIN_CORPS


%start programme

%%

programme:
    {
        TS = creerTableSymbole();  
        TQ = initialiserTQ() ;
        P = initialiserP();

    }
    functions DEBUT DEB_CORPS declarations instructions FIN SEMICOLON FIN_CORPS {
        qC++;
        quad = creer_Q("fin", "", "", "", qC);
        inserer_TQ(TQ, quad); 
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);  // afficher TQ pour confirmer
        afficherTQDansFichier(TQ, "output.txt");
        printf("\nProgramme accepte.");
    }
    ;

type:
    ENTIER {
        currentType.baseType = ENTIER;
        currentType.isArray = false;
        currentType.isConst = false;
        $$ = ENTIER;
    }
    | FLOTTANT {
        currentType.baseType = FLOTTANT;
        currentType.isArray = false;
        currentType.isConst = false;
        $$ = FLOTTANT;
    }
    | STRING {
        currentType.baseType = STRING;
        currentType.isArray = false;
        currentType.isConst = false;
        $$ = STRING;
    }
    | CHAR {
        currentType.baseType = CHAR;
        currentType.isArray = false;
        currentType.isConst = false;
        $$ = CHAR;
    }
    | BOOLEAN {
        currentType.baseType = BOOLEAN;
        currentType.isArray = false;
        currentType.isConst = false;
        $$ = BOOLEAN;
    }
    | CONST type {
        currentType.isConst = true;
        $$ = $2;
    }
    ;


tableau :
    TABLE type ID DEB_TABLEAU INT FIN_TABLEAU {
        Symbole* found;
        if (rechercherSymbole(TS, $3, &found)) {
            semanticError("Tableau Deja Déclaré", line);
        }
        
        // Convert the type token to string
        char* typeStr;
        switch($2) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        
        Symbole* sym = creerSymbole(
            VARIABLE,    // category
            $3,         // name
            typeStr,    // type
            "",         // initial value
            line,       // line number
            $5         // memory address
        );
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);
    }
    ;

type_Struct : 
    ENREGISTREMENT ID DEB_CORPS declarations FIN_CORPS
valeur:
    INT { 
        $$.type = ENTIER;
        char buffer[256];
        sprintf(buffer, "%d", $1);
        $$.valeur = strdup(buffer);
    }
    | FLOAT {
        $$.type = FLOTTANT;
        char buffer[256];
        sprintf(buffer, "%f", $1);
        $$.valeur = strdup(buffer);
    }
    | CARACTERE {
        $$.type = CHAR;
        char buffer[256];
        sprintf(buffer, "%c", $1);
        $$.valeur = strdup(buffer);
    }
    | CHAINE {
        $$.type = STRING;
        $$.valeur = strdup($1);
    }
    | TRUE {
        $$.type = BOOLEAN;
        $$.valeur = strdup("true");
    }
    | FALSE {
        $$.type = BOOLEAN;
        $$.valeur = strdup("false");
    }
    ;
variable:
    ID DEB_TABLEAU INT FIN_TABLEAU {
        Symbole* found;
        rechercherSymbole(TS, $1, &found);
        if (found == NULL) {
            semanticError("Tableau non declaree", line);
        }
        else{
            if ($3 > found->taille) {
                
                semanticError("Index hors limites", line);
            }
            else{
                $$.nom = $1;
                switch(found->type[0]) {
                    case 'E': $$.type = ENTIER; break;
                    case 'F': $$.type = FLOTTANT; break;
                    case 'S': $$.type = STRING; break;
                    case 'C': $$.type = CHAR; break;
                    case 'B': $$.type = BOOLEAN; break;
                    default: $$.type = ENTIER;
                }
            }
       }
    }
    | ID POINTEUR ID {
        Symbole* found;
        rechercherSymbole(TS, $1, &found);
        if (found == NULL) {
            semanticError("Structure non declaree", line);
        }
        else{
            rechercherSymbole(TS, $3, &found);
            if (found == NULL) {
                semanticError("Variable non declaree", line);
            }
            else{
                $$.nom = $3;
                switch(found->type[0]) {
                    case 'E': $$.type = ENTIER; break;
                    case 'F': $$.type = FLOTTANT; break;
                    case 'S': $$.type = STRING; break;
                    case 'C': $$.type = CHAR; break;
                    case 'B': $$.type = BOOLEAN; break;
                    default: $$.type = ENTIER;
                }
            }
        }

    }
    | ID {
        $$.nom = strdup($1);
        Symbole* found;
        if (rechercherSymbole(TS, $1, &found)) {
            switch(found->type[0]) {
                case 'E': $$.type = ENTIER; break;
                case 'F': $$.type = FLOTTANT; break;
                case 'S': $$.type = STRING; break;
                case 'C': $$.type = CHAR; break;
                case 'B': $$.type = BOOLEAN; break;
                default: $$.type = ENTIER;
            }
            $$.valeur = strdup(found->valeur);
            }
                else {
            //on check dabord si il ya une fonction en cours de traitement 
            if (saveFunctionDec != NULL) {
                //on check si la variable est un parametre de la fonction
                Parametre *param;
                Symbole* found;
                rechercherSymbole(TS, saveFunctionDec, &found);
                if (rechercherParametre(found, $1, &param)) {
                    char* exprType = NULL;
                    switch(param->type[0]) {
                    case 'E': $$.type = ENTIER; break;
                    case 'F': $$.type = FLOTTANT; break;
                    case 'S': $$.type = STRING; break;
                    case 'C': $$.type = CHAR; break;
                    case 'B': $$.type = BOOLEAN; break;
                    default: $$.type = ENTIER;
                }
                $$.valeur = strdup("0");
                    
                } else {
                    semanticError("Variable non declaree", line);
                }
            } else {
            semanticError("Variable non declaree", line);
        }
    }
            }
        
    ;
expression:
    valeur  
    | variable
    | expression PLUS expression {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;            
        // Perform addition based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) + atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = ENTIER;
        } else if ($1.type == FLOTTANT || $3.type == FLOTTANT) {
            float result = atof(val1) + atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) + atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else {
            semanticError("Invalid types for addition", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("+", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | expression MOINS expression {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;            
        // Perform subtraction based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) - atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = ENTIER;
        } else if ($1.type == FLOTTANT || $3.type == FLOTTANT) {
            float result = atof(val1) - atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) - atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else {
            semanticError("Invalid types for subtraction", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("-", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | expression MULT expression {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;            
        // Perform multiplication based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) * atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = ENTIER;
        } else if ($1.type == FLOTTANT || $3.type == FLOTTANT) {
            float result = atof(val1) * atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) * atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else {
            semanticError("Invalid types for multiplication", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("*", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | expression DIV expression {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;            
        // Perform division based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            if (atoi(val2) == 0) {
                semanticError("Division by zero", line);
            }
            int result = atoi(val1) / atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = ENTIER;
        } else if ($1.type == FLOTTANT || $3.type == FLOTTANT) {
            if (atof(val2) == 0.0) {
                semanticError("Division by zero", line);
            }
            float result = atof(val1) / atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            if (atof(val2) == 0.0) {
                semanticError("Division by zero", line);
            }
            float result = atof(val1) / atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else {
            semanticError("Invalid types for division", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("/", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | expression MOD expression {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;            
        // Perform modulo based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            if (atoi(val2) == 0) {
                semanticError("Modulo by zero", line);
            }
            int result = atoi(val1) % atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = ENTIER;
        } else {
            semanticError("Invalid types for modulo", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("%", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | expression PUISS expression {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;            
        // Perform power based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = pow(atoi(val1), atoi(val2));
            sprintf($$.valeur, "%d", result);
            $$.type = ENTIER;
        } else if ($1.type == FLOTTANT || $3.type == FLOTTANT) {
            float result = pow(atof(val1), atof(val2));
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else {
            semanticError("Invalid types for power", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("^", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | NOT expression {
        printf("i am inside NOT operation\n");
        char bff[255]; 
        Symbole* found1;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get value for operand
        char *val1;
        val1 = $2.valeur;            
        // Perform NOT operation based on type
        if ($2.type == BOOLEAN) {
            int result = !atoi(val1);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid type for NOT operation", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R", qC);
        $$.nom = resultVarName;
        quad = creer_Q("!", 
                      $2.nom ? $2.nom : $2.valeur,
                      "",
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | PAR_OUV expression PAR_FERM
    {
        $$.nom = $2.nom;
        $$.valeur = $2.valeur;
        $$.type = $2.type;
    }

    | expression INF expression {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;   

        // Perform subtraction based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) < atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else if ($1.type == FLOTTANT && $3.type == FLOTTANT) {
            float result = atof(val1) < atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) < atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == STRING && $3.type == STRING) || ($1.type == CHAR && $3.type == CHAR)) {
            int result = strcmp(val1, val2) < 0;
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q(">", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | expression INF_EGAL expression {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;   

        // Perform subtraction based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) <= atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else if ($1.type == FLOTTANT && $3.type == FLOTTANT) {
            float result = atof(val1) <= atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) <= atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == STRING && $3.type == STRING) || ($1.type == CHAR && $3.type == CHAR)) {
            int result = strcmp(val1, val2) <= 0;
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("=>", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");

    }
    | expression SUP expression {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;   

        // Perform subtraction based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) > atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else if ($1.type == FLOTTANT && $3.type == FLOTTANT) {
            float result = atof(val1) > atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) > atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == STRING && $3.type == STRING) || ($1.type == CHAR && $3.type == CHAR)) {
            int result = strcmp(val1, val2) > 0;
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("<", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | expression SUPP_EGAL expression {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;   

        // Perform subtraction based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) >= atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else if ($1.type == FLOTTANT && $3.type == FLOTTANT) {
            float result = atof(val1) >= atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) >= atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == STRING && $3.type == STRING) || ($1.type == CHAR && $3.type == CHAR)) {
            int result = strcmp(val1, val2) >= 0;
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("<=", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    
    | expression EQUAL expression {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;   

        // Perform subtraction based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) == atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else if ($1.type == FLOTTANT && $3.type == FLOTTANT) {
            float result = atof(val1) == atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) == atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == STRING && $3.type == STRING) || ($1.type == CHAR && $3.type == CHAR)) {
            int result = strcmp(val1, val2) == 0;
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        }
        else if($1.type == BOOLEAN && $3.type == BOOLEAN){
            int result = atoi(val1) == atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("==", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    
    | expression NOT_EQUAL expression {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;   

        // Perform subtraction based on types
        if ($1.type == ENTIER && $3.type == ENTIER) {
            int result = atoi(val1) != atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else if ($1.type == FLOTTANT && $3.type == FLOTTANT) {
            float result = atof(val1) != atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == FLOTTANT && $3.type == ENTIER) || ($1.type == ENTIER && $3.type == FLOTTANT)) {
            float result = atof(val1) != atof(val2);
            sprintf($$.valeur, "%f", result);
            $$.type = BOOLEAN;
        } else if (($1.type == STRING && $3.type == STRING) || ($1.type == CHAR && $3.type == CHAR)) {
            int result = strcmp(val1, val2) != 0;
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else if($1.type == BOOLEAN && $3.type == BOOLEAN){
            int result = atoi(val1) != atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("!=", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | expression AND expression {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;   

        // Perform subtraction based on types
        if ($1.type == BOOLEAN && $3.type == BOOLEAN) {
            int result = atoi(val1) && atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("&&", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");

    }
    | expression OR expression {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = $1.valeur;
        val2 = $3.valeur;   

        // Perform subtraction based on types
        if ($1.type == BOOLEAN && $3.type == BOOLEAN) {
            int result = atoi(val1) || atoi(val2);
            sprintf($$.valeur, "%d", result);
            $$.type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("||", 
                      $1.nom ? $1.nom : $1.valeur,
                      $3.nom ? $3.nom : $3.valeur,
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");

    }
    ; 

incrementation:
    variable INCREM {

        char bff[255]; 
        Symbole* found1;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get value for operand
        char *val1;
        val1 = $1.valeur;        
        // Perform addition based on types
        if ($1.type == ENTIER ) {
            int result = atoi(val1) + 1;
            sprintf($$.valeur, "%d", result);
            $$.type = ENTIER;
        } else if ($1.type == FLOTTANT ) {
            float result = atof(val1) + 1;
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else {
            semanticError("Invalid types for addition", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("++", 
                      $1.nom ? $1.nom : $1.valeur,
                      "",
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    | variable DECREM
    {
        char bff[255]; 
        Symbole* found1;
        
        // Initialize result structure
        $$.nom = NULL;
        $$.valeur = malloc(255);
        if ($$.valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get value for operand
        char *val1;
        val1 = $1.valeur;        
        // Perform subtraction based on types
        if ($1.type == ENTIER ) {
            int result = atoi(val1) - 1;
            sprintf($$.valeur, "%d", result);
            $$.type = ENTIER;
        } else if ($1.type == FLOTTANT ) {
            float result = atof(val1) - 1;
            sprintf($$.valeur, "%f", result);
            $$.type = FLOTTANT;
        } else {
            semanticError("Invalid types for subtraction", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        $$.nom=resultVarName;
        quad = creer_Q("--", 
                      $1.nom ? $1.nom : $1.valeur,
                      "",
                      $$.nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
    ;

declarations:
    /* empty */
    | declarations declaration 
    ;

declaration:
    type ID  {
        Symbole* found;
        if (rechercherSymbole(TS, $2, &found)) {
            semanticError("Variable already declared", line);
        }
        
        // Convert the type token to string
        char* typeStr;
        switch($1) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        
        Symbole* sym = creerSymbole(
            VARIABLE,    // category
            $2,         // name
            typeStr,    // type
            "",         // initial value
            line,       // line number
            0          // memory address
        );
        
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);
    } SEMICOLON
    | tableau SEMICOLON 
    | type_Struct SEMICOLON 
    | type ID ASSIGN expression SEMICOLON {
        Symbole* found;
        if (rechercherSymbole(TS, $2, &found)) {
            semanticError("Variable already declared", line);
        }
        
        // Convert the type token to string
        char* typeStr;
        switch($1) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        
        // Check if the expression is valid for the type
        if (!($4.valeur, typeStr)) {
            semanticError("Invalid literal for type", line);
        }
        
        Symbole* sym = creerSymbole(
            VARIABLE,    // category
            $2,         // name
            typeStr,    // type
            $4.valeur,  // initial value
            line,       // line number
            0          // memory address
        );
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);
    }
    
    ;
functions:
    /* empty */
    | functions fonction 
    ;
fonction:
    declarationfonction PAR_OUV parametres PAR_FERM {
        printf("je suis dans fonction1\n");
        printf("Function parameters:\n");
        if ($3->head != NULL) {
            ParametreNode* current = $3->head;
            printf("hilllo");
            Symbole* found;
            //ajouter les parametres de la fonction à la table des symboles
            printf("Parametre: %s %s\n", current->parametre.nom, current->parametre.type);
            printf("le nom de la fonction est : %s\n", $1.nom);
            if(rechercherSymbole(TS, $1.nom, &found)){
            }

            while (current) {            
            ajouterParametre(found, current->parametre.nom, current->parametre.type);
            current = current->next;
            }
            printf("\naffichage des infos de la fonction \n"); 
         afficherInfoFonction(found);
         

        }
        saveFunctionDec=strdup($1.nom);
         
    } corps {
        
        //on check si c'est une fonction (a un type != void ) et ne retourne pas de valeur
        if($1.type[0] != 'V' && !itReturn){
            semanticError("La fonction ne retourne pas de valeur", line);
        }
        printf("fonction correcte syntaxiquement\n"); afficherTableSymbole(TS);   
        saveFunctionDec=NULL;
        }
    ;
declarationfonction :
    type FONCTION ID {        
        Symbole* found;
        if (rechercherSymbole(TS, $3, &found)) {
            semanticError("Function already declared", line);
        }
        
        // Convert the type token to string
        char* typeStr;
        switch($1) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        //création du symbole de la fonction
        Symbole* sym = creerSymbole(
            FUNCTION,    // category
            $3,         // name
            typeStr,    // type
            "",         // initial value
            line,       // line number
            0          // memory address
        );
        $$.nom = strdup($3);
        $$.type = strdup(typeStr);
        
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);

    }
    | FONCTION ID{
        Symbole* found;
        if (rechercherSymbole(TS, $2, &found)) {
            semanticError("Function already declared", line);
        }
        //création du symbole de la fonction
        Symbole* sym = creerSymbole(
            FUNCTION,    // category
            $2,         // name
            "VOID",    // type
            "",         // initial value
            line,       // line number
            0          // memory address
        );
        $$.nom = strdup($2);
        $$.type = strdup("VOID");
        
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);
    }
    ;

parametres:
    /* empty */ {
        $$ = creerParametresList();  // Always create a new list for empty case
    }
    | parametres VIRGULE parametre {
        // Use the existing list from $1
        $$ = $1;  
        // Add the new parameter to the list
        if (!ajouterParametreUnion($$, $3.nom, $3.type)) {
            semanticError("Parametre deja declare", line);
        }
    }
    | parametre {
        // Create new list for single parameter
        $$ = creerParametresList();
        // Add the parameter to the new list
        if (!ajouterParametreUnion($$, $1.nom, $1.type)) {
            semanticError("Parametre deja declare", line);
        }
    }
    ;
    
    
parametre:
    type ID {
        //remplissage des champs du parametre 
        $$.nom = strdup($2);
        char* typeStr;
        switch($1) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        $$.type = strdup(typeStr);

    }
    | TABLE type ID {
    
    }
    | ENREGISTREMENT ID {printf("parametre correcte syntaxiquement\n");}
    ;
instructions:
    /* empty */
    | instructions instruction 
    ;   
instruction:
    declaration
    | assignment 
    | COMMENT
    | COMMENT_PLUS
    | condition
    | loop
    | read
    | write 
    | incrementation SEMICOLON
    | retourner SEMICOLON
    | call SEMICOLON
    ;
read:
    INPUT PAR_OUV ID PAR_FERM SEMICOLON {printf("read correcte syntaxiquement\n");}
    ;

write:
    PRINT PAR_OUV CHAINE VIRGULE ID PAR_FERM SEMICOLON
    |PRINT PAR_OUV ID PAR_FERM SEMICOLON
    |PRINT PAR_OUV CHAINE PAR_FERM SEMICOLON{printf("write correcte syntaxiquement\n");}
    ;

retourner:
    RETURN expression {
        //on check si le type de retour est compatible avec le type de la fonction
        
        Symbole* found;
        printf("voici le nom de la fonction %s\n", saveFunctionDec);
        if (rechercherSymbole(TS, saveFunctionDec, &found)) {
            printf("je suis dans retourner\n");
            if(found->type[0] == 'V'){
                semanticError("La fonction ne retourne pas de valeur", line);
            }
            char* exprType = NULL;
                switch($2.type) {
                    case ENTIER: exprType = TYPE_ENTIER; break;
                    case FLOTTANT: exprType = TYPE_FLOTTANT; break;
                    case CHAR: exprType = TYPE_CHAR; break;
                    case STRING: exprType = TYPE_STRING; break;
                    case BOOLEAN: exprType = TYPE_BOOLEAN; break;
                    default: exprType = TYPE_ENTIER;
                }
            if (!areTypesCompatible(found->type, exprType)) {
                semanticError("Type de retour incompatible avec la fonction", line);
            }
            itReturn = true;
        } else {
          semanticError("Fonction non declaree", line);
        }
        }
    ;

assignment:
    ID ASSIGN expression {
        Symbole* found;
        if (rechercherSymbole(TS, $1, &found)) { // is declared
            if (found->categorie == VARIABLE) {  
                qC++;
                
                // Create a buffer for the value
                char buffer[256];
                sprintf(buffer, "%s", $3.valeur ? $3.valeur : "");
                
                // Update the symbol's value

                SetValueSymbol(found, buffer);
                
                // Create quadruplet
                quad = creer_Q(":=", $1, " ", buffer, qC);
                inserer_TQ(TQ, quad);
                 afficherTQ(TQ);
                
                // Convert expression type to string for comparison
                char* exprType = NULL;
                printf("Type de l'expression: %d\n", $3.type);
                switch($3.type) {
                    case ENTIER: exprType = TYPE_ENTIER; break;
                    case FLOTTANT: exprType = TYPE_FLOTTANT; break;
                    case CHAR: exprType = TYPE_CHAR; break;
                    case STRING: exprType = TYPE_STRING; break;
                    case BOOLEAN: exprType = TYPE_BOOLEAN; break;
                }
                printf("Type de l'expression: %s\n", exprType);
                printf("Type de la variable: %s\n", found->type);
                if (!areTypesCompatible(found->type, exprType)) {
                    semanticError("Type incompatible dans l'affectation.", line);
                }
            } else {
                semanticError("Identifier is not a variable", line);
            }
        } else {
            //on check dabord si il ya une fonction en cours de traitement 
            if (saveFunctionDec != NULL) {
                //on check si la variable est un parametre de la fonction
                Parametre *param;
                Symbole* found;
                rechercherSymbole(TS, saveFunctionDec, &found);
                if (rechercherParametre(found, $1, &param)) {
                    //on check si le type de retour est compatible avec le type de la variable
                    char* exprType = NULL;
                    switch($3.type) {
                        case ENTIER: exprType = TYPE_ENTIER; break;
                        case FLOTTANT: exprType = TYPE_FLOTTANT; break;
                        case CHAR: exprType = TYPE_CHAR; break;
                        case STRING: exprType = TYPE_STRING; break;
                        case BOOLEAN: exprType = TYPE_BOOLEAN; break;
                        default: exprType = TYPE_ENTIER;
                    }
                    if (!areTypesCompatible(exprType, found->type)) {
                        semanticError("Type incompatible dans l'affectation.", line);
                    }
                } else {
                    semanticError("Variable non declaree", line);
                }
            } else {
            semanticError("Variable non declaree", line);
        }
    } }SEMICOLON
    | ID ASSIGN call{
        Symbole* found;
        if (rechercherSymbole(TS, $1, &found)) { // is declared
        //on check si le type de retour est compatible avec le type de la variable
        char* typeStr;
        switch($3.type) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        if (!areTypesCompatible(found->type, typeStr)) {
            semanticError("Type incompatible dans l'affectation.", line);
        }
        qC++;        
        // Create a buffer for the value
        char buffer[256];
        sprintf(buffer, "%s", $3.valeur ? $3.valeur : "");
        // Update the symbol's value
        SetValueSymbol(found, buffer);        
        // Create quadruplet
        quad = creer_Q(":=", $1, " ", buffer, qC);
        inserer_TQ(TQ, quad);
        afficherTQ(TQ);}
        else {
             //on check dabord si il ya une fonction en cours de traitement 
            if (saveFunctionDec != NULL) {
                Symbole* found;
                Parametre *param;
                rechercherSymbole(TS, saveFunctionDec, &found);
                //on check si la variable est un parametre de la fonction
                if (rechercherParametre(found, $1, &param)) {
                    //on check si le type de retour est compatible avec le type de la variable
                    char* exprType = NULL;
                    switch($3.type) {
                        case ENTIER: exprType = TYPE_ENTIER; break;
                        case FLOTTANT: exprType = TYPE_FLOTTANT; break;
                        case CHAR: exprType = TYPE_CHAR; break;
                        case STRING: exprType = TYPE_STRING; break;
                        case BOOLEAN: exprType = TYPE_BOOLEAN; break;
                        default: exprType = TYPE_ENTIER;
                    }
                    if (!areTypesCompatible(exprType, found->type)) {
                        semanticError("Type incompatible dans l'affectation.", line);
                    }
                } else {
                    semanticError("Variable non declaree", line);
                }
            } else {
            semanticError("Variable non declaree", line);
        }
        }
    } SEMICOLON
    ;

condition:
    ifstatement corps elsebloc  
        {
        while (!Pempty(P)) {
            quad = pop(P);
            updateLabel(quad, qC);
        }
        qC++;
        quad = creer_Q("finIf","", " ", "", qC);
        inserer_TQ(TQ, quad);
        
    }
    ;
ifstatement:
    IF PAR_OUV expression PAR_FERM{
        //vérifier si le resultat de l'expression est un boolean
        if($3.type != BOOLEAN){
            semanticError("L'expression doit être un boolean", line);
        }else{
            qC++;
            //branchement en cas de faux
            quad = creer_Q("BZ","temp", " ", $3.valeur, qC);
            inserer_TQ(TQ, quad);
            //sauvegarder qC pour la mise a jour de l'adresse de branchement dans la pile des adresses
            push(P, quad);
            
        }
    }


loop:
    while_partie_une while_partie_deux while_partie_trois
    | for_partie_une for_partie_deux for_partie_trois
    ;

while_partie_une:
    WHILE PAR_OUV {

        printf("I'm inside while\n");

        qC++;
        
        sauvDebut = qC;
    };

while_partie_deux:
    expression{

        printf("I'm inside while\n");

        if($1.type != BOOLEAN) {
            semanticError("Condition de boucle invalide", line);
        }
        // Branchement vers fin si condition n'est pas vrai
        quad = creer_Q("BZ", "fin", " ", $1.valeur, qC);
        inserer_TQ(TQ, quad);
        push(P, quad);
        afficherTQ(TQ);
    };

while_partie_trois:
    PAR_FERM corps {

        printf("I'm inside while\n");

        qC++;
        quad = pop(P); 
        updateLabel(quad, qC+1);
        // Branchement inconditionnel vers Debut
        
        char etiq[255];
        sprintf(etiq, "%d", sauvDebut);
        quad = creer_Q("BR", etiq, "", "", qC);
        inserer_TQ(TQ, quad);
        afficherTQ(TQ);

    };

for_partie_une:
    FOR PAR_OUV {

        printf("I'm inside for\n");
       
        qC++;
        sauvDebut = qC;
    };

for_partie_deux:

    ID FROM INT TO INT PAR_FERM {

        printf("Entrée dans for_partie_deux\n");

        char temp[20]; 
        int tempCounter = 0; 

        sprintf(temp, "T%d", tempCounter++); 

        // Utilisation dans creer_Q
        char str5[20], str3[20];
        sprintf(str5, "%d", $5);
        sprintf(str3, "%d", $3);
        quad = creer_Q("-", str5, str3, temp, qC);

        inserer_TQ(TQ, quad);
        afficherTQ(TQ);

        // Vérifier si le résultat est inférieur à 0
        qC++;
        quad = creer_Q("BZ", "fin", " ", temp, qC); // Sauter si la condition est fausse
        inserer_TQ(TQ, quad);
        push(P, quad);
        afficherTQ(TQ);
    };

for_partie_trois:
    corps {

        printf("Entrée dans for_partie_trois\n");

        qC++;
        quad = pop(P); 
        updateLabel(quad, qC+1);

        // Branchement inconditionnel vers Debut
        char etiq[255];
        sprintf(etiq, "%d", sauvDebut);
        quad = creer_Q("BR", etiq, "", "", qC);
        inserer_TQ(TQ, quad);
        afficherTQ(TQ);
    };

corps :
    DEB_CORPS declarations instructions FIN_CORPS
    | instruction
    ;
elsebloc:
    /* empty */ {
        //maj de l'adresse de branchement dans la pile des adresses
        qC++;
        quad = pop(P);
        updateLabel(quad, qC);
        printf("\nelsebloc correcte syntaxiquement qc = %d\n", qC);
    }
    | elsestatement corps {
        qC++;
        //maj de l'adresse de branchement dans la pile des adresses
        quad = pop(P);
        updateLabel(quad, qC);
        //maj de l'adresse de branchement dans la pile des adresses
       
    }
    | elifstatement corps elsebloc
    
    ;
elsestatement :
    ELSE {
        //maj de l'adresse de branchement dans la pile des adresses
        qC++;
        quad = pop(P);
        updateLabel(quad, qC+1);
        //branchement vers la fin
        
        quad = creer_Q("BR","temp", " ", "", qC);
        inserer_TQ(TQ, quad);
        push(P, quad);
        
    }
elifstatement:
    elifkey PAR_OUV expression PAR_FERM {
        if($3.type != BOOLEAN){
            semanticError("L'expression doit être un boolean", line);
        }else{
            qC++;
            //branchement en cas de faux
            quad = creer_Q("BZ","temp", " ", $3.valeur, qC);
            inserer_TQ(TQ, quad);
            //sauvegarder qC pour la mise a jour de l'adresse de branchement dans la pile des adresses
            push(P, quad);
            
        }
    }
elifkey:
    ELIF{
        //maj de l'adresse de branchement dans la pile des adresses
        qC++;
        quad = pop(P);
        updateLabel(quad, qC+1);
        //branchement vers la fin
        
        quad = creer_Q("BR","temp", " ", "", qC);
        inserer_TQ(TQ, quad);
        push(P, quad);
    }
    ;


call:
    ID 
    {
        //affectation du nom de la fonction courrante 

        currentFunctionCalled = strdup($1);
        //initialisation de la liste des parametres
        currentParametresList = creerParametresList();
        
    }
        PAR_OUV parametresCall PAR_FERM {
        $$.nom = strdup($1);
        //on check si la fonction existe déja dans la table des symboles
        Symbole* found;
        if (rechercherSymbole(TS, $1, &found)) {
            if (found->categorie == FUNCTION) {                
                if (strcmp(found->type, TYPE_ENTIER) == 0) {
                    $$.type = ENTIER;
                } else if (strcmp(found->type, TYPE_FLOTTANT) == 0) {
                    $$.type = FLOTTANT;
                } else if (strcmp(found->type, TYPE_CHAR) == 0) {
                    $$.type = CHAR;
                } else if (strcmp(found->type, TYPE_STRING) == 0) {
                    $$.type = STRING;
                } else if (strcmp(found->type, TYPE_BOOLEAN) == 0) {
                    $$.type = BOOLEAN;
                }
                $$.valeur = strdup(found->valeur);
                //on vérifie le nombre de parametres est le meme 
                if (found->infoFonction->nbParametres != currentParametresList->count) {
                    semanticError("Nombre de parametres incorrect", line);
                }else
                {
                    //on check pour chaque parametre la compatibilité de type 
                    ParametreNode* current = currentParametresList->head;
                    Parametre* currentFonction = found->infoFonction->parametres;
                    while (current) {
                        if (!areTypesCompatible(current->parametre.type, currentFonction->type)) {
                            semanticError("Type incompatible dans l'appel de fonction", line);
                        }
                        current = current->next;
                        currentFonction = currentFonction->suivant;
                    }
                    // on crée le quadruplet pour l'appel de fonction
                    
                    //on fin , on rend la fonction courante et les parametres à NULL
                    currentFunctionCalled = NULL;
                    currentParametresList = NULL;
                    //on push la ligne suivante dans la pile des adresses
                }
            }
            else{
                semanticError("Identifier is not a function", line);
            }
        } else {
            semanticError("Function not declared", line);
        }
        
    }
    
    ;
 
     

parametresCall:
    /* empty */
    | parametresCall VIRGULE parametreCall{
        char* typeStr;
        switch($3.type) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        //ajouter le nouveau parametre à la liste des parametres
        ajouterParametreUnion(currentParametresList, $3.nom ? $3.nom : (char*)$3.valeur, typeStr);
            
    }
    | parametreCall{
        char* typeStr;
        switch($1.type) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        ajouterParametreUnion(currentParametresList, $1.nom ? $1.nom : (char*)$1.valeur, typeStr) ;
    }
    ;
    
parametreCall:
    valeur
    | variable {printf("parametre correcte syntaxiquement\n");}
    ;

%%

void yyerror(const char *s) {
    if (sauvline == line){
        line = line -1 ;
    }
    fprintf(stderr, "File \"%s\", line %d, character %d: %s\n", 
            yyin_filename, line,sauv - linecol, s);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    yyin_filename = argv[1];
    FILE *file = fopen(yyin_filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    yyin = file;
    yyparse();
    fclose(yyin);

    return 0;
}

