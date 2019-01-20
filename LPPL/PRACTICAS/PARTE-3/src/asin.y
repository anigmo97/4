%{
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "libtds.h"

%}

%union{
    int cent;
    EXP tipoExpresion; //expresion
// instruccion if = expresion +  ( int puntero_a_else, int puntero_a_fin)
    INST_IF tipoIf; 
//for = expresion +( int posicion_condicion , int posicion_incremento,int puntero_a_fin,int puntero_a_cuerpo);    
    INST_FOR tipoFor; 
    int tipoOperador;
    char* nomVar; 
}
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
%token <nomVar> ID_

%type <tipoExpresion> tipoSimple constante expresion expresionOpcional expresionIgualdad 
%type <tipoExpresion> expresionRelacional expresionAditiva expresionMultiplicativa expresionUnaria expresionSufija
//%type <tipoOperador> operadorAsignacion operadorUnario


/*AÑADIDOS */
%type <tipoExpresion> instruccionAsignacion
%type <cent> operadorAditivo operadorAsignacion operadorIgualdad operadorMultiplicativo
%type <cent> operadorIncremento operadorUnario operadorRelacional operadorLogico
%type <cent> TRUE_ FALSE_
%type <tipoIf> instruccionSeleccion

%%
programa : LLAVE_1_ {numErrores=0;dvar=0;}secuenciaSentencias LLAVE_2_
            {
                if(verbosidad){
                    mostrarTDS();
                }
                emite(FIN, crArgNul(), crArgNul(), crArgNul());
            }
    ;
secuenciaSentencias : sentencia 
                    | secuenciaSentencias sentencia 
                    ;
sentencia : declaracion 
          | instruccion
          ;
declaracion : tipoSimple ID_ SEMI_COLON_
            {
                if(insTSimpleTDS($2,$1.tipo,dvar)==FALSE){  //La variable está ya declarada?
                    yyerror(VAR_DECLARADA);
                }
				else{
					dvar=dvar+TALLA_TIPO_SIMPLE;
				}
            }
            | tipoSimple ID_ IGUAL_ constante SEMI_COLON_
            {
                if(insTSimpleTDS($2,$1.tipo,dvar)==FALSE){ //La variable está ya declarada?
                    yyerror(VAR_DECLARADA);
                }
                else if($1.tipo!=$4.tipo){     //El r-value y el l-value son del mismo tipo?
                    yyerror(TIPOS_INCOMPATIBLES);
                }
                else{ // si todo va bien
                    dvar=dvar+TALLA_TIPO_SIMPLE;
                    SIMB simb = obtenerTDS($2);
                    emite(EASIG,$4.arg,crArgNul(),crArgPos(simb.desp));
                }
            }

            | tipoSimple ID_ CORCHETE_1_ CTE_ CORCHETE_2_  SEMI_COLON_
            {
                if($4==0){    //El nº de elementos está bien?
                    yyerror(ELEMENTOS_ARRAY);
                }

                else if(insTVectorTDS($2,T_ARRAY,dvar,$1.tipo,$4)==FALSE){   //La variable está ya declarada?
                    yyerror(VAR_DECLARADA);
                }
                else{
                    dvar= dvar+  TALLA_TIPO_SIMPLE * $4;
                    //REVISAR ESTA PARTE FALTA ACABAR
                }
            }
            ;

tipoSimple  : INT_{$$.tipo = T_ENTERO;} //REVISAR TIPO SIMPLE PODRIA SER INT
            | BOOL_{$$.tipo = T_LOGICO;}
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
                        {
                            SIMB simb;
                            simb=obtenerTDS($1);
                            if(simb.tipo==T_ERROR){ //La variable está declarada?
                                yyerror(VAR_NO_DECLARADA);
                            }

                            else if(simb.tipo==T_ARRAY){ //La variable es simple?
                                yyerror(VAR_TIPO_ARRAY);
                            }
                                       
		                    else if(simb.tipo!=$3.tipo && $3.tipo!=T_ERROR){ //El r-value y el l-value son del mismo tipo?
		                        yyerror(TIPOS_INCOMPATIBLES);
		                    }else if($2!=EASIG && (simb.tipo==T_LOGICO || $3.tipo ==T_LOGICO)){
								yyerror(OPERADOR_INCOMPATIBLE);							
							}else{ //si todo va bien
                                int id_pos = simb.desp;
                                $$.arg = crArgPos(id_pos);
                                if($2 == ESIG || $2 == EASIG){
                                    emite($2,$3.arg,crArgNul(),$$.arg); //puede ver valor o posicion $3 o $3.arg??
                                }else{
                                    emite($2,crArgPos(id_pos),$3.arg,$$.arg);
                                } 
                            }                        
                        }

                      | ID_ CORCHETE_1_ expresion CORCHETE_2_ operadorAsignacion expresion SEMI_COLON_
                        {
                            SIMB simb;
                            simb=obtenerTDS($1);
                            if(simb.tipo==T_ERROR){ //La variable está declarada?
                                yyerror(VAR_NO_DECLARADA);
                            }
                            else if(simb.tipo!=T_ARRAY){ //La variable es un array?
                                yyerror(VAR_TIPO_SIMPLE);
                            }

                            else if($3.tipo != T_ENTERO){ //La expresion del indice es un entero?
                                yyerror(INDICE_ARRAY);
                            }

                            else if(simb.telem!=$6.tipo && $6.tipo !=T_ERROR){  //El r-value es del tipo de la variable?
                                yyerror(TIPOS_INCOMPATIBLES);
                            }
                            // REVISAR , OP_ENTERO ESTA OBSOLETO?
                            else if($5!=EASIG && (simb.telem==T_LOGICO || $6.tipo==T_LOGICO) ) {   //El operador es compatible con los tipos?
                                yyerror(OPERADOR_INCOMPATIBLE);
                            }else{ // si todo va bien
                                //COMPROBAR QUE OPERADORES COGE, DEBERIA COGER TODOS
                                $$.arg = crArgPos(creaVarTemp());
                                if($5 == EASIG ){ // si =
                                    // REVISAR PONER VALOR EN $$.arg
                                    emite(EVA,crArgPos(simb.desp),$3.arg,$6.arg); // id[$3] := $6
                                }else{ // si +=  -=  *=  /=
                                    emite(EAV,crArgPos(simb.desp),$3.arg,$$.arg); // $$.arg := id[$3]
                                    emite($5,$$.arg,$6.arg,$$.arg); // $$.arg := $$.arg OP $6
                                    emite(EVA,crArgPos(simb.desp),$3.arg,$$.arg);
                                }
                                
                            }
                        }
                      ;

instruccionEntradaSalida  : READ_ PARENTESIS_1_ ID_ PARENTESIS_2_ SEMI_COLON_
                            {
                                SIMB simb;
                                simb=obtenerTDS($3);
                                if(simb.tipo==T_ERROR){ //La variable está declarada?
                                    yyerror(VAR_NO_DECLARADA);
                                }
								else if(simb.tipo!=T_ENTERO){
									yyerror(EXPRESION_READ);								
								}else{// si todo va bien
                                    emite(EREAD,crArgNul(),crArgNul(),crArgPos(simb.desp));
                                }
                            }
                          | PRINT_ PARENTESIS_1_ expresion PARENTESIS_2_ SEMI_COLON_
                          {
                            if ($3.tipo!=T_ENTERO){ 
							    yyerror(EXPRESION_PRINT);
							}else{ // si todo va bien
                                emite(EWRITE,crArgNul(),crArgNul(),$3.arg);
                            }
                          }
                          ;
/* pongo <tipoIf> porque sino, no se sabe si tipo a mitad de regla*/
instruccionSeleccion  : IF_ PARENTESIS_1_ expresion PARENTESIS_2_{   
                            if($3.tipo!=T_LOGICO && $3.tipo!=T_ERROR){  //La primera expresión es de tipo lógico?
                                yyerror(EXPRESION_IF);
                            }/*else{ //si todo va bien
                                
                            }*/
                            $<tipoIf>1.puntero_a_else = creaLans(si);
                            emite(EIGUAL,$3.arg,crArgEnt(FALSE),crArgEtq($<tipoIf>1.puntero_a_else));
                        }
                         instruccion ELSE_{
                             $<tipoIf>1.puntero_a_fin = creaLans(si);
                             emite(GOTOS,crArgNul(),crArgNul(),crArgEtq($<tipoIf>1.puntero_a_fin));
                             completaLans($<tipoIf>1.puntero_a_else,crArgEtq(si));
                         } 
                         instruccion{
                            completaLans($<tipoIf>1.puntero_a_fin,crArgEtq(si));
                        }
                        ;
instruccionIteracion  : FOR_ PARENTESIS_1_ expresionOpcional SEMI_COLON_ {
                            $<tipoFor>1.posicion_condicion = si;  // condicion for
                        }
                        expresion SEMI_COLON_
  						{
                            if($6.tipo!=T_LOGICO && $6.tipo!=T_ERROR){
                                yyerror(EXPRESION_FOR);
                            }else{ // si todo va bien
                                $<tipoFor>1.puntero_a_fin = creaLans(si); // $$ =expresion final
                                emite(EIGUAL,$6.arg,crArgEnt(FALSE),crArgEtq($<tipoFor>1.puntero_a_fin)); //si false salta a final
                                $<tipoFor>1.puntero_a_cuerpo = creaLans(si);  // $2 -> SEMI_COLON_ // puntero a cuerpo
                                emite(GOTOS,crArgNul(),crArgNul(),crArgEtq($<tipoFor>1.puntero_a_cuerpo)); // si sigue aqui va al cuerpo
                                $<tipoFor>1.posicion_incremento = si;
                            }
                        } 
						 expresionOpcional PARENTESIS_2_{
                            emite(GOTOS,crArgNul(),crArgNul(),crArgEtq($<tipoFor>1.posicion_condicion)); // este debe ir a la condicion
                            completaLans($<tipoFor>1.puntero_a_cuerpo,crArgEtq(si)); // este completa el puntero_al_cuerpo
                         }
                         instruccion {
                             emite(GOTOS,crArgNul(),crArgNul(),crArgEtq($<tipoFor>1.posicion_incremento)); // debe ir a incremento
                             completaLans($<tipoFor>1.puntero_a_fin,crArgEtq(si)); // este completa el puntero a fin 
                         }
                     
    ;

expresionOpcional : expresion {$$.tipo=$1.tipo;$$.arg = $1.arg;}  
                  | ID_ IGUAL_ expresion
                  {
                      $$.tipo=T_ERROR;
                      SIMB simb;
                      simb=obtenerTDS($1);
                      if(simb.tipo==T_ERROR){   //La variable está declarada?
                          yyerror(VAR_NO_DECLARADA);
                      }
                      if(simb.tipo==T_ARRAY){   //La variable es de tipo simple?
                        yyerror(VAR_TIPO_ARRAY);
                      }
                      else if(simb.tipo!=$3.tipo && $3.tipo!=T_ERROR){
                          yyerror(TIPOS_INCOMPATIBLES);
                      }
					  else{
						$$.tipo=simb.tipo;
                        $$.arg = crArgPos(simb.desp);
                        emite(EASIG,$3.arg,crArgNul(),crArgPos(simb.desp));
					 }
                  }
                  |{$$.tipo=T_VACIO;}
                  ;

expresion : expresionIgualdad {$$.tipo=$1.tipo;$$.arg = $1.arg;}
          | expresion operadorLogico expresionIgualdad
          {
              $$.tipo=T_ERROR;
              if($1.tipo!=T_ERROR && $3.tipo!=T_ERROR){   //Las expresiones tienen un valor correcto?
                  if($1.tipo==T_LOGICO && $3.tipo==T_LOGICO){ //Las expresiones son de tipo lógico?
                    $$.tipo=T_LOGICO;
                    $$.arg = crArgPos(creaVarTemp());
                    if ($2 == AND_DOBLE_){
                        emite(EASIG,crArgEnt(TRUE),crArgNul(),$$.arg); // lo pongo a true
                        emite(EIGUAL,$1.arg,crArgEnt(FALSE),crArgEtq(si+3)); // si false salto a poner a false
                        emite(EIGUAL,$3.arg,crArgEnt(FALSE),crArgEtq(si+2)); // si false salto a poner a false
                        emite(GOTOS,crArgNul(),crArgNul(),crArgEtq(si+2)); // si he llegado es que ambas eran true
                        emite(EASIG,crArgEnt(FALSE),crArgNul(),$$.arg); // lo pongo a false
                    }else { // OR
                        emite(EASIG,crArgEnt(TRUE),crArgNul(),$$.arg); // lo pongo a true
                        emite(EIGUAL,$1.arg,crArgEnt(TRUE),crArgEtq(si+3)); // si true salto a fuera
                        emite(EIGUAL,$3.arg,crArgEnt(TRUE),crArgEtq(si+2)); // si true salto
                        emite(EASIG,crArgEnt(FALSE),crArgNul(),$$.arg); // lo pongo a false
                    }
                  }
                  else{
                      yyerror(OPERADOR_INCOMPATIBLE);
                  }
              }
          }
          ;

expresionIgualdad : expresionRelacional{$$.tipo=$1.tipo;$$.arg = $1.arg;} //HACER TEST CUANDO HAYA IF
                  | expresionIgualdad operadorIgualdad expresionRelacional
                  { 
                      $$.tipo=T_ERROR;
                      if($1.tipo!=T_ERROR && $3.tipo!=T_ERROR){ //Las expresiones son correctas?
                          if($1.tipo==$3.tipo){
                              $$.tipo=T_LOGICO;
                              $$.arg = crArgPos(creaVarTemp());
                              emite(EASIG,crArgEnt(TRUE),crArgNul(),$$.arg); // lo pongo a true
                              emite($2,$1.arg,$3.arg,crArgEtq(si+2)); // si se cumple me salto la instruccion siguiente
                              emite(EASIG,crArgEnt(FALSE),crArgNul(),$$.arg); // lo pongo a false
                          }
                          else{
                              yyerror(TIPOS_INCOMPATIBLES);
                          }
                      }
                  }
                  ;
expresionRelacional : expresionAditiva{$$.tipo=$1.tipo;$$.arg = $1.arg;}
                    | expresionRelacional operadorRelacional expresionAditiva //REVISAR Y MEJORAR TEST
                    {
                        $$.tipo=T_ERROR;
                        if($1.tipo!=T_ERROR && $3.tipo!=T_ERROR){
                            if($1.tipo==T_ENTERO && $3.tipo==T_ENTERO){
                                $$.tipo=T_LOGICO;
                                $$.arg = crArgPos(creaVarTemp());
                                emite(EASIG,crArgEnt(TRUE),crArgNul(),$$.arg); // lo pongo a true
                                emite($2,$1.arg,$3.arg,crArgEtq(si+2)); // si se cumple me salto la instruccion siguiente
                                emite(EASIG,crArgEnt(FALSE),crArgNul(),$$.arg);
                            }
                            else{
                                yyerror(TIPOS_INCOMPATIBLES);
                            }
                        }
                    }
                    ;
expresionAditiva  : expresionMultiplicativa {$$.tipo=$1.tipo;$$.arg = $1.arg;}
                  | expresionAditiva operadorAditivo expresionMultiplicativa
                  {
                      $$.tipo=T_ERROR;
                      if($1.tipo!=T_ERROR && $3.tipo!=T_ERROR){
                          if($1.tipo==T_ENTERO && $3.tipo==T_ENTERO){
                              $$.tipo=T_ENTERO;
                              $$.arg = crArgPos(creaVarTemp());
                              emite($2,$1.arg,$3.arg,$$.arg);
                          }
                          else{
                              yyerror(TIPOS_INCOMPATIBLES);
                          }
                      }
                  }
                  ;
expresionMultiplicativa : expresionUnaria{$$.tipo=$1.tipo;$$.arg = $1.arg;}
                        | expresionMultiplicativa operadorMultiplicativo expresionUnaria
                        {
                            $$.tipo=T_ERROR;
                            if($1.tipo!=T_ERROR && $3.tipo!=T_ERROR){
                                if($1.tipo==T_ENTERO && $3.tipo==T_ENTERO){
                                    $$.tipo=T_ENTERO;
                                    $$.arg = crArgPos(creaVarTemp());
                                    emite($2,$1.arg,$3.arg,$$.arg);
                                }
                                else{
                                    yyerror(TIPOS_INCOMPATIBLES);
                                }
                            }

                        }
                        ;
expresionUnaria : expresionSufija {$$.tipo=$1.tipo;$$.arg = $1.arg;}
                | operadorUnario expresionUnaria
                {
                    $$.tipo=T_ERROR;
                    if($2.tipo!=T_ERROR){
                        if($2.tipo==T_LOGICO && $1==OP_LOGICO){ // CASO !
                            $$.tipo=$2.tipo;
                            $$.arg = crArgPos(creaVarTemp());
                            emite(EASIG,crArgEnt(TRUE),crArgNul(),$$.arg);
                            emite(EIGUAL,$2.arg,crArgEnt(FALSE),crArgEnt(si + 2)); //REVISAR
                            emite(EASIG,crArgEnt(FALSE),crArgNul(),$$.arg);
                        }else if($2.tipo==T_ENTERO && ($1==EASIG || $1==ESIG)){ //CASO + -
                            $$.tipo=$2.tipo;
                            $$.arg = crArgPos(creaVarTemp());
                            emite($1,$2.arg,crArgNul(),$$.arg);
                        }else{
                            yyerror(OPERADOR_INCOMPATIBLE);
                        }
                    }
                }
                | operadorIncremento ID_
                {
                    $$.tipo=T_ERROR;
                    SIMB simb;
                    simb=obtenerTDS($2);
                    if(simb.tipo==T_ERROR){
                        yyerror(VAR_NO_DECLARADA);
                    }else if (simb.tipo==T_ENTERO){
                        $$.tipo=T_ENTERO;
                        $$.arg = crArgPos(simb.desp);
                        emite($1,crArgPos(simb.desp),crArgEnt(1),$$.arg);
                    }else{ 
                        yyerror(OPERADOR_INCOMPATIBLE);
                    }
                }
                ;
expresionSufija : PARENTESIS_1_ expresion PARENTESIS_2_{$$=$2;}
                | ID_ operadorIncremento
                {
                    $$.tipo=T_ERROR;
                    SIMB simb;
                    simb=obtenerTDS($1);
                    if(simb.tipo==T_ERROR){
                        yyerror(VAR_NO_DECLARADA);
					}else if(simb.tipo==T_ARRAY){
						yyerror(VAR_TIPO_ARRAY);
					}
                    else if (simb.tipo==T_ENTERO){ //SI TODO VA BIEN
                        $$.tipo=T_ENTERO;
                        $$.arg= crArgPos(creaVarTemp());
                        // como es x++ o x-- primero se copia
                        emite(EASIG, crArgPos(simb.desp), crArgNul(), $$.arg);
                        emite($2, crArgPos(simb.desp), crArgEnt(1), crArgPos(simb.desp));
                    }else{
                        yyerror(OPERADOR_INCOMPATIBLE);
                    }
                }
                | ID_ CORCHETE_1_ expresion CORCHETE_2_
                {
                    $$.tipo=T_ERROR;
                    SIMB simb;
                    simb=obtenerTDS($1);
                    if(simb.tipo==T_ERROR){
                        yyerror(VAR_NO_DECLARADA);
                    }else if(simb.tipo!=T_ARRAY){
                        yyerror(VAR_TIPO_SIMPLE);
                    }else if($3.tipo==T_ENTERO){ //SE PODRÍA CAMBIAR LA ESTRUCTURA
                        //FALTA COMPROBAR QUE EL INDICE ES MENOR QUE EL NUMERO DE ELEMENTOS
                        $$.tipo=simb.telem;
                        $$.arg = crArgPos(creaVarTemp());
                        emite(EAV,crArgPos(simb.desp),$3.arg,$$.arg);
                    }else if($3.tipo!=T_ERROR){
                        yyerror(INDICE_ARRAY);
                    }    
                }
                | ID_ 
                {
                    $$.tipo=T_ERROR;
                    SIMB simb;
                    simb=obtenerTDS($1);
                    if(simb.tipo==T_ERROR){
                        yyerror(VAR_NO_DECLARADA);
                    }else if(simb.tipo==T_ARRAY){
						yyerror(VAR_TIPO_ARRAY);					
					}else{// si todo va bien
						$$.tipo=simb.tipo;
                        $$.arg = crArgPos(simb.desp);					
					}
                }
                | constante{$$=$1;}
                ;
constante : CTE_ {$$.tipo=T_ENTERO;
                  int pos = creaVarTemp();
                  $$.arg = crArgPos(pos);
                  emite(EASIG,crArgEnt($1),crArgNul(),$$.arg);
                  }
          | TRUE_{$$.tipo=T_LOGICO;
                  int pos = creaVarTemp();
                  $$.arg = crArgPos(pos);
                  emite(EASIG,crArgEnt(TRUE),crArgNul(),$$.arg);
                 }
          | FALSE_{ $$.tipo=T_LOGICO;
                   int pos = creaVarTemp();
                   $$.arg = crArgPos(pos);
                   emite(EASIG,crArgEnt(FALSE),crArgNul(),$$.arg);
                  }
          ;
operadorAsignacion  : IGUAL_ {$$=EASIG;}
                    | MAS_IGUAL_ {$$=ESUM;} 
                    | MENOS_IGUAL_ {$$=EDIF;}
                    | POR_IGUAL_ {$$=EMULT;}
                    | DIVIDIDO_IGUAL_ {$$=EDIVI;}
                    ;
operadorLogico  : AND_DOBLE_ {$$ = AND_DOBLE_;}
                | OR_DOBLE_  {$$ = OR_DOBLE_;}
                ;
operadorIgualdad  : IGUAL_DOBLE_ {$$=EIGUAL;}
                  | DISTINTO_ {$$=EDIST;}
                  ;
operadorRelacional  : MAYOR_ {$$=EMAY;} 
                    | MENOR_  {$$=EMEN;}
                    | MAYOR_IGUAL_ {$$=EMAYEQ;}
                    | MENOR_IGUAL_ {$$=EMENEQ;}
                    ;
operadorAditivo : MAS_ { $$ = ESUM; }
                | MENOS_ { $$ = EDIF; }
                ;
operadorMultiplicativo  : POR_  { $$ = EMULT; }
                        | DIVIDIDO_ { $$ = EDIVI; }
                        | MODULO_ { $$ = RESTO; }
                        ;
operadorUnario  : MAS_{$$=EASIG;}
                | MENOS_ {$$=ESIG;}
                | NEGADO_ {$$=OP_LOGICO;}
                ;
operadorIncremento  : INCREMENTO_ { $$ = ESUM; }
                    | DECREMENTO_ { $$ = EDIF; }
                    ;

%%
/*****************************************************************************/
/*****************************************************************************/
