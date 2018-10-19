%{
#include <stdio.h>
#include <string.h>
#include "header.h"
%}

%token SEMI_COLON_   
%token BOOL_   INT_    TRUE_  FALSE_
%token IGUAL_   MAS_IGUAL_  MENOS_IGUAL_  POR_IGUAL_  DIVIDIDO_IGUAL_
%token AND_DOBLE_   OR_DOBLE_
%token IGUAL_DOBLE_   DISTINTO_
%token MAYOR_   MENOR_  MAYOR_IGUAL_  MENOR_IGUAL 
%token MAS_   MENOS_  POR_  DIVIDIDO_   MODULO_   NEGADO_
%token INCREMENTO_  DECREMENTO_
%token CORCHETE_1_  CORCHETE_2_   PARENTESIS_1_   PARENTESIS_2_   LLAVE_1_  LLAVE_2_
%token FOR_   READ_   PRINT_  WHILE_  IF_   ELSE_ 
%token CTE_ 
%token ID_





%%

//REGLAS 


programa : LLAVE_1_ secuenciaSentencias LLAVE_2_;
secuenciaSentencias : sentencia 
                    | secuenciaSentencias sentencia 
                    ;
sentencia : declaracion 
          | instruccion
          ;
declaracion : tipoSimple ID_ SEMI_COLON_
            | tipoSimple ID_ IGUAL_ constante SEMI_COLON_
            | tipoSimple ID_ CORCHETE_1_ CTE_ CORCHETE_2_  SEMI_COLON_
            ;
tipoSimple  : INT_
            | BOOL_
            ;
instruccion : LLAVE_1_ listaInstrucciones LLAVE_2_
            | instruccionEntradaSalida
            | instruccionAsignacion
            | instruccionSeleccion
            | instruccionIteracion
            ;
listaInstrucciones  : listaInstrucciones instruccion
                    | 
                    ;
instruccionAsignacion : ID_ operadorAsignacion expresion SEMI_COLON_
                      | ID_ CORCHETE_1_ expresion CORCHETE_2_ operadorAsignacion expresion SEMI_COLON_
                      ;
instruccionEntradaSalida  : READ_ PARENTESIS_1_ ID_ PARENTESIS_2_ SEMI_COLON_
                          | PRINT_ PARENTESIS_1_ expresion PARENTESIS_2_ SEMI_COLON_
                          ;
instruccionSeleccion  : IF_ PARENTESIS_1_ expresion PARENTESIS_2_ instruccion ELSE_ instruccion;
instruccionIteracion  : FOR_ PARENTESIS_1_ expresionOpcional SEMI_COLON_ expresion SEMI_COLON_ expresionOpcional PARENTESIS_2_ instruccion;
expresionOpcional : expresion 
                  | ID_ IGUAL_ expresion
                  |
                  ;
expresion : expresionIgualdad
          | expresion operadorLogico expresionIgualdad
          ;
expresionIgualdad : expresionRelacional
                  | expresionIgualdad operadorIgualdad expresionRelacional
                  ;
expresionRelacional : expresionAditiva
                    | expresionRelacional operadorRelacional expresionAditiva
                    ;
expresionAditiva  : expresionMultiplicativa
                  | expresionAditiva operadorAditivo expresionMultiplicativa
                  ;
expresionMultiplicativa : expresionUnaria
                        | expresionMultiplicativa operadorMultiplicativo expresionUnaria
                        ;
expresionUnaria : expresionSufija 
                | operadorUnario expresionUnaria
                | operadorIncremento ID_
                ;
expresionSufija : PARENTESIS_1_ expresion PARENTESIS_2_
                | ID_ operadorIncremento
                | ID_ CORCHETE_1_ expresion CORCHETE_2_
                | ID_
                | constante
                ;
constante : CTE_
          | TRUE_
          | FALSE_
          ;
operadorAsignacion  : IGUAL_
                    | MAS_IGUAL_
                    | MENOS_IGUAL_
                    | POR_IGUAL_
                    | DIVIDIDO_IGUAL_
                    ;
operadorLogico  : AND_DOBLE_ 
                | OR_DOBLE_
                ;
operadorIgualdad  : IGUAL_DOBLE_
                  | DISTINTO_
                  ;
operadorRelacional  : MAYOR_
                    | MENOR_
                    | MAYOR_IGUAL_
                    | MENOR_IGUAL
                    ;
operadorAditivo : MAS_
                | MENOS_
                ;
operadorMultiplicativo  : POR_
                        | DIVIDIDO_
                        | MODULO_
                        ;
operadorUnario  : MAS_
                | MENOS_
                | NEGADO_
                ;
operadorIncremento  : INCREMENTO_
                    | DECREMENTO_
                    ;

%%
/*****************************************************************************/
/*****************************************************************************/