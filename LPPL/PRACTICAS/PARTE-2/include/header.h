#ifndef _HEADER_H
#define _HEADER_H

/****************************************************** Constantes generales */
#define TRUE  1
#define FALSE 0
#define TALLA_TIPO_SIMPLE 1 //AÑADIDO

/*operadorAsignacion*/
#define OP_ASI_IGUAL 0
#define OP_ASI_MAS_IGUAL 1
#define OP_ASI_MENOS_IGUAL 2
#define OP_ASI_POR_IGUAL 3
#define OP_ASI_DIV_IGUAL 4

/*operadorLogico*/  
#define OP_LOG_AND 0
#define OP_LOG_OR 1

/*operadorIgualdad*/
#define OP_ADI_IGUAL_DOBLE 0
#define OP_IGU_DIST 1


/*operadorRelacional*/
#define OP_REL_MAYOR 0
#define OP_REL_MENOR 1
#define OP_REL_MAYOR_IG 2
#define OP_REL_MENOR_IG 3

/*operadorAditivo*/
#define OP_ADI_MAS 0
#define OP_ADI_MENOS 1

/*operadorMultiplicativo*/ 
#define OP_MUL_POR 0 
#define OP_MUL_DIV 1
#define OP_MUL_MOD 2

/*operadorUnario*/
#define OP_UNA_MAS 0
#define OP_UNA_MENOS 1
#define OP_UNA_NEG 2


/*operadorIncremento*/
#define OP_INC_INC 0 
#define OP_INC_DEC 1

/************************************************ Struct para las expresions */
typedef struct exp {
    int valor;
    int tipo;
} EXP;

/************************************* Variables externas definidas en el AL */
extern int yylex();
extern int yyparse();

extern FILE *yyin;
extern int   yylineno;
extern char *yytext;
/********************* Variables externas definidas en el Programa Principal */
extern void yyerror(const char * msg) ;   /* Tratamiento de errores          */

extern int verbosidad;              /* Flag para saber si se desea una traza */
extern int numErrores;              /* Contador del numero de errores        */

#endif  /* _HEADER_H */
/*****************************************************************************/
