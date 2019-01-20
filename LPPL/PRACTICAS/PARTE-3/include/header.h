/*****************************************************************************/
/**   Ejemplo de un posible fichero de cabeceras ("header.h") donde situar  **/
/** las definiciones de constantes, variables y estructuras para MenosC.18  **/
/** Los alumos deberan adaptarlo al desarrollo de su propio compilador.     **/ 
/*****************************************************************************/
#ifndef _HEADER_H
#define _HEADER_H
#include <stdio.h>
#include <string.h>

/****************************************************** Constantes generales */
#define TRUE  1
#define FALSE 0
#define TALLA_TIPO_SIMPLE 1 

/********************************************** Compatibilidad de operadores */
#define OP_ENTERO 0
#define OP_LOGICO 1
#define OP_AMBOS 2

//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
// NUEVO PARTE 3 TODO REORDENAR
// typedef struct exp {
//     int valor;
//     int tipo;
//     int pos;
// } EXP;
#include <libgci.h>
//extern struct TIPO_ARG;

typedef struct exp{
    int tipo; //  T_VACIO, T_ENTERO ,T_LOGICO ,T_ARRAY ,T_ERROR 
    TIPO_ARG arg; //tipo y  val
} EXP;

typedef struct inst_if{
    int tipo;
    TIPO_ARG arg;
    int puntero_a_else;
    int puntero_a_fin;
}INST_IF;

typedef struct inst_for{
    int tipo;
    TIPO_ARG arg;
    int posicion_condicion;
    int posicion_incremento;
    int puntero_a_fin;
    int puntero_a_cuerpo;
}INST_FOR;

// TIPO_ARG.tipo -> ARG_ENTERO, ARG_POSICION ,ARG_ETIQUETA ,ARG_NULO
// TIPO_ARG.val   -> ARG_ENTERO, ARG_POSICION ,ARG_ETIQUETA ,ARG_NULO

//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************

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


/********************************************************* Mensajes de error */
#define VAR_DECLARADA "Error: Variable ya declarada"
#define VAR_NO_DECLARADA "Error: Variable no declarada"
#define VAR_TIPO_SIMPLE "Error: La variable es de tipo simple"
#define VAR_TIPO_ARRAY "Error: La variable es de tipo array"

#define OPERADOR_INCOMPATIBLE "Error: Operador incompatible con tipo(s)"
#define TIPOS_INCOMPATIBLES "Error: Incompatibilidad de tipos"
#define ELEMENTOS_ARRAY "Error: El nº de elementos de un array debe ser mayor a 0"
#define INDICE_ARRAY "Error: El índice de un array debe ser de tipo entero"

#define EXPRESION_ERRONEA "Error: Expresión errónea"
#define EXPRESION_IF "Error: La condición del if debe ser una expresión de tipo lógico"
#define EXPRESION_FOR "Error: La expresión del bucle for debe ser de tipo lógico"
#define EXPRESION_PRINT "Error: La operación print solo admite argumento de tipo entero"
#define EXPRESION_READ "Error: La operaciónr read solo admite argumento de tipo entero"

#endif  /* _HEADER_H */
/*****************************************************************************/
