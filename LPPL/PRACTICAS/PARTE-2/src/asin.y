%{
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "libtds.h"
%}

%token SEMI_COLON_   
%token BOOL_   INT_    TRUE_  FALSE_
%token IGUAL_   MAS_IGUAL_  MENOS_IGUAL_  POR_IGUAL_  DIVIDIDO_IGUAL_
%token AND_DOBLE_   OR_DOBLE_
%token IGUAL_DOBLE_   DISTINTO_
%token MAYOR_   MENOR_  MAYOR_IGUAL_  MENOR_IGUAL_ 
%token MAS_   MENOS_  POR_  DIVIDIDO_   MODULO_   NEGADO_
%token INCREMENTO_  DECREMENTO_
%token CORCHETE_1_  CORCHETE_2_   PARENTESIS_1_   PARENTESIS_2_   LLAVE_1_  LLAVE_2_
%token FOR_   READ_   PRINT_  WHILE_  IF_   ELSE_ 
%token <cent> CTE_ 
%token <ident> ID_


%type <cent> tipoSimple
%type <cent> constante 
%type <cent> expresionUnaria expresionSufija expresionMultiplicativa 
%type <cent> expresionAditiva expresionRelacional expresionIgualdad expresion expresionOpcional
%type <cent> operadorAsignacion operadorLogico operadorIgualdad operadorRelacional
%type <cent> operadorAditivo operadorMultiplicativo operadorUnario operadorIncremento


/**
%type <operator> operadorAsignacion operadorLogico operadorIgualdad  operadorRelacional
%type <operator> operadorAditivo operadorMultiplicativo operadorUnario operadorIncremento
**/

%union{
    char *ident;        /* Nombre del identificador */
    int cent;           /* Valor de la cte num ́erica entera */
    EXP estruct;        /* Estructura */
}




%%

//REGLAS 


/*************************************************************************************
*******************************INICIO PARTE 1 EDUARDO*********************************
**************************************************************************************/

programa : LLAVE_1_ secuenciaSentencias LLAVE_2_ {mostrarTDS() ;};
secuenciaSentencias : sentencia 
                    | secuenciaSentencias sentencia 
                    ;
sentencia : declaracion 
          | instruccion 
          ;
declaracion : tipoSimple ID_ SEMI_COLON_ /* TO DO FIRST*/
             {
                if (! insTSimpleTDS($2, $1, 0)){ yyerror("DECLARACION REPETIDA"); }
                
             }
            | tipoSimple ID_ IGUAL_ constante SEMI_COLON_
            {
                if($1 != $4){
                    yyerror("No se puede convertir entre tipos ");
                }else if (! insTSimpleTDS($2, $1, 0)){ yyerror("DECLARACION REPETIDA"); }
             }
            | tipoSimple ID_ CORCHETE_1_ CTE_ CORCHETE_2_  SEMI_COLON_
            {   
                if($4 <= 0){
                    yyerror("La talla de un array debe ser un entero positivo");  
                }else{
                    insTVectorTDS($2,T_ARRAY,0,$1,$4);
                }
            }
            ;

tipoSimple  : INT_  {$$ = T_ENTERO;}
            | BOOL_ {$$ = T_LOGICO;}
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
instruccionAsignacion : ID_ operadorAsignacion expresion SEMI_COLON_ {
                            SIMB simb = obtenerTDS($1);
                            if( simb.tipo == T_ERROR){ 
                                yyerror("Identificador no declarado");
                            }else if(simb.tipo != $3 && $1!=T_ERROR && $3!=T_ERROR){
                                yyerror("Los tipos no son compatibles");
                            }
                      }
                      | ID_ CORCHETE_1_ expresion CORCHETE_2_ operadorAsignacion expresion SEMI_COLON_ {
                            SIMB simb = obtenerTDS($1);
                            if( simb.tipo == T_ERROR){ 
                                yyerror("Identificador no declarado");
                            }else if(simb.tipo != T_ARRAY){
                                yyerror("El identificador especificado no es de tipo array");
                            }else if(simb.telem != $6 && $6!=T_ERROR){
                                yyerror("No se puede asignar elementes de un tipo distinto a los elementos del array");
                            }else if($3 != T_ENTERO){
                                yyerror("Las posiciones del array deben de ser de tipo entero");
                            }
                      }
                      ;
/*************************************************************************************
*******************************FIN PARTE 1 EDUARDO************************************
**************************************************************************************/






/*************************************************************************************
*******************************INICIO PARTE JOSE *************************************
**************************************************************************************/
instruccionEntradaSalida  : READ_ PARENTESIS_1_ ID_ PARENTESIS_2_ SEMI_COLON_ {
                                SIMB simb = obtenerTDS($3);
                                if( simb.tipo == T_ERROR){ 
                                    yyerror("Identificador no declarado");
                                }else if(simb.tipo != T_ENTERO){
                                    yyerror("Read solo toma enteros como parametro");
                                }
                            }
                          | PRINT_ PARENTESIS_1_ expresion PARENTESIS_2_ SEMI_COLON_{
                                if($3 != T_ENTERO){
                                    yyerror("La funcion print solo toma parametros de tipo entero");
                                }
                                
                          }
                          ;
instruccionSeleccion  : IF_ PARENTESIS_1_ expresion PARENTESIS_2_ instruccion ELSE_ instruccion{
                                if($3!=T_LOGICO && $3!=T_ERROR){
                                    yyerror("La clausula del if debe ser de tipo logico");
                                }
                            }
instruccionIteracion  : FOR_ PARENTESIS_1_ expresionOpcional SEMI_COLON_ expresion{
                                                                                    if($5 != T_LOGICO){
                                                                                        yyerror("El bucle for necesita una clausula booleana");
                                                                                    }
                            } SEMI_COLON_ expresionOpcional PARENTESIS_2_ instruccion{
                            
                            }
expresionOpcional : expresion {$$ = $1;} 
                  | ID_ IGUAL_ expresion {
                        $$ = T_ERROR;
                        SIMB simb = obtenerTDS($1);
                        if( simb.tipo == T_ERROR){ 
                            yyerror("Identificador no declarado");
                        }else if(simb.tipo != $3 && $1!=T_ERROR && $3!=T_ERROR){
                            yyerror("Los tipos no son compatibles");
                        }else{
                            $$ = simb.tipo;
                        }
                  }
                  | {$$ = T_VACIO;}
                  ;
expresion : expresionIgualdad {$$ = /*expresionIgualdad*/$1;}
          | expresion operadorLogico expresionIgualdad{
                $$ = T_ERROR;
                if(/*expresion*/$1!=T_LOGICO || /*expresionIgualdad*/$3!=T_LOGICO ){
                    yyerror("No se pueden aplicar operadores logicos a tipos distintos del logico");
                }else {
                    $$ = /*expresion*/$1;
                }
          }
          ;
/*************************************************************************************
******************************* FIN PARTE JOSE ***************************************
**************************************************************************************/





/*************************************************************************************
*******************************INICIO PARTE 2 EDUARDO*********************************
**************************************************************************************/

expresionIgualdad : expresionRelacional {$$ = $1;}
                  | expresionIgualdad operadorIgualdad expresionRelacional{
                        $$ = T_ERROR;
                        if( /*expresionIgualdad*/$1 != $3/*expresionRelacional*/){
                            yyerror("No se pueden comparar tipos distintos");
                        }else if(!(/*expresionIgualdad*/$1==T_ERROR || /*expresionRelacional*/$3==T_ERROR)){
                            $$ = $1/*expresionIgualdad*/;   
                        }
                    }
                  
                  ;
expresionRelacional : expresionAditiva {$$ = $1;}
                    | expresionRelacional operadorRelacional expresionAditiva{
                        $$ = T_ERROR;
                        if(/*expresionRelacional*/$1!=T_ENTERO ||  /*expresionAditiva*/$3!=T_ENTERO ){
                            yyerror("No se pueden ordenar tipos distintos de entero");
                        }else {
                            $$ = T_LOGICO/*expresionRelacional*/;   
                        }
                    }
                    ;

/*************************************************************************************
*******************************FIN PARTE 2 EDUARDO************************************
**************************************************************************************/








/*************************************************************************************
*******************************INICIO PARTE ANGEL*************************************
**************************************************************************************/
expresionAditiva  : expresionMultiplicativa {$$ = $1;}
                  | expresionAditiva operadorAditivo expresionMultiplicativa{
                        $$ = T_ERROR;
                        if(/*expresionAditiva*/$1 != $3/*expresionMultiplicativa*/ ){
                            yyerror("No se pueden multiplicar tipos distintos");
                        }else if($1!=T_ENTERO){
                            yyerror("La expresion aditiva solo es aplicable al tipo entero");
                        }else {
                            $$ = $1/*expresionAditiva*/;
                        }
                  }
                  ;
expresionMultiplicativa : expresionUnaria {$$ = $1/*expresionUnaria*/;}
                        | expresionMultiplicativa operadorMultiplicativo expresionUnaria{
                            $$ = T_ERROR;
                            if(/*expresionMultiplicativa*/ $1 != $3 /*expresionUnaria*/){
                                yyerror("No se pueden multiplicar tipos distintos");
                            }else if(/*expresionMultiplicativa*/$1!=T_ENTERO){
                                yyerror("La expresion multiplicativa solo es aplicable al tipo entero");
                            }else {
                                $$ = $1 /*expresionMultiplicativa*/;
                            }
                        }
                        ;

expresionUnaria : expresionSufija {$$ = $1/*expresionSufija*/;}
                | operadorUnario expresionUnaria { $$ = $2; /*expresionUnaria*/}
                | operadorIncremento ID_ {
                    $$ = T_ERROR;
                    SIMB simb = obtenerTDS($1);
                    if( simb.tipo == T_ERROR){ 
                        yyerror("Identificador no declarado");
                    }else if(simb.tipo != T_ENTERO){
                        yyerror("operador solo aplicable a tipo entero");
                    }else{
                        $$ = simb.tipo;
                    }
                   
                }
                ;

expresionSufija : PARENTESIS_1_ expresion PARENTESIS_2_{
                    $$ = $2/*expresion*/;
                }

                | ID_ operadorIncremento {
                    $$ = T_ERROR;
                    SIMB simb = obtenerTDS($1);
                    if( simb.tipo == T_ERROR){ 
                        yyerror("Identificador no declarado");
                    }else if(simb.tipo != T_ENTERO){
                        yyerror("operador solo aplicable a tipo entero");
                    }else{
                        $$ = simb.tipo;
                    }
                    
                }
                | ID_ CORCHETE_1_ expresion CORCHETE_2_
                {
                    $$ = T_ERROR;
                    SIMB simb = obtenerTDS($1);
                    if( simb.tipo == T_ERROR){ 
                        yyerror("Identificador no declarado");
                    }else if(/*expresion*/$3 != T_ENTERO){
                        yyerror("Los indices de un array deben ser enteros");
                    }else{
                        $$=simb.telem;
                    }
                }
                | ID_ 
                {
                    $$ = T_ERROR;
                    SIMB simb = obtenerTDS($1);
                    if( simb.tipo == T_ERROR){ 
                        yyerror("Identificador no declarado");
                    }
                    $$ = simb.tipo;
                 }
                | constante {$$=$1;}
                ;

/*TO DO eliminar valor */
constante : CTE_    { $$ = T_ENTERO; }
          | TRUE_   { $$ = T_LOGICO; }
          | FALSE_  { $$ = T_LOGICO; }
          ;

operadorAsignacion  : IGUAL_            { $$ = OP_ASI_IGUAL; }
                    | MAS_IGUAL_        { $$ = OP_ASI_MAS_IGUAL; }
                    | MENOS_IGUAL_      { $$ = OP_ASI_MENOS_IGUAL; }
                    | POR_IGUAL_        { $$ = OP_ASI_POR_IGUAL; }
                    | DIVIDIDO_IGUAL_   { $$ = OP_ASI_DIV_IGUAL; }
                    ;

operadorLogico  : AND_DOBLE_        { $$ = OP_LOG_AND; }
                | OR_DOBLE_         { $$ = OP_LOG_OR; }
                ;

operadorIgualdad  : IGUAL_DOBLE_    { $$ = OP_ADI_IGUAL_DOBLE; }
                  | DISTINTO_       { $$ = OP_IGU_DIST; }
                  ;

operadorRelacional  : MAYOR_          { $$ = OP_REL_MAYOR; }
                    | MENOR_          { $$ = OP_REL_MENOR; }
                    | MAYOR_IGUAL_    { $$ = OP_REL_MAYOR_IG; }  
                    | MENOR_IGUAL_    { $$ = OP_REL_MENOR_IG; }
                    ;

operadorAditivo : MAS_      { $$ = OP_ADI_MAS; }
                | MENOS_    { $$ = OP_ADI_MENOS; }
                ;

operadorMultiplicativo  : POR_      { $$ = OP_MUL_POR; }
                        | DIVIDIDO_ { $$ = OP_MUL_DIV; }
                        | MODULO_   { $$ = OP_MUL_MOD; }
                        ;

operadorUnario  : MAS_      { $$ = OP_UNA_MAS; }
                | MENOS_    { $$ = OP_UNA_MENOS; }
                | NEGADO_   { $$ = OP_UNA_NEG; }
                ;

operadorIncremento  : INCREMENTO_   { $$ = OP_INC_INC; }
                    | DECREMENTO_   { $$ = OP_INC_DEC; }
                    ;

/* ANOTACION $$ -> parte izquierda regla*/

/*************************************************************************************
******************************* FIN PARTE ANGEL  *************************************
**************************************************************************************/


%%
/*****************************************************************************/
/*****************************************************************************/