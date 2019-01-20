#!/bin/bash

ROJO="\e[0;31m"
NC="\e[0m"
printf '\e[3;0;0t'
PATH_TESTS_PROFE="tmp/"
PATH_TESTS_ANGEL="tmp/test-angel/"
INSTRUCCION_PRUEBA="$1"


clear
make
if [ $? -ne 0 ]
then 
    echo -e "${ROJO} ERRORES EN MAKE ${NC}"
    exit 1
fi

if [[ $INSTRUCCION_PRUEBA == *.c* ]]
then
  echo -e "${ROJO}NO ESCRIBA LA EXTENSION, SE BUSCA .c por defecto${NC}";
  exit 1
fi



if [ -f "tmp/$INSTRUCCION_PRUEBA.c" ]
then 
	PATH_RELATIVO="tmp"
	rm "tmp/$INSTRUCCION_PRUEBA.c3d"
else 
    if [ -f "tmp/test-angel/$INSTRUCCION_PRUEBA.c" ]
    then 
		PATH_RELATIVO="tmp/test-angel"
		rm "tmp/test-angel/$INSTRUCCION_PRUEBA.c3d"
    else 
        echo -e "${ROJO} no se encontró $INSTRUCCION_PRUEBA.c en ./tmp ni ./tmp/test-angel ${NC}"
        exit 1
    fi
fi

./cmc "$PATH_RELATIVO/$INSTRUCCION_PRUEBA.c"     # compila el archivo


gedit "$PATH_RELATIVO/$INSTRUCCION_PRUEBA.c" &  # abre editor para ver y editar

PID=$! # guardo el PID del ultimo proceso (gedit)

./bin/mvm -p "$PATH_RELATIVO/$INSTRUCCION_PRUEBA.c3d" # lanzo la maquina virtual para ejecutar el codigo

echo "EJECUCION TERMINADA"
read var
read var
kill $PID # mato el proceso gedit
