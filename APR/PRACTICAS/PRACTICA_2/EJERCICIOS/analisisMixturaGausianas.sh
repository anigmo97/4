#!/bin/bash

if [ $# != 5 ]
then
    echo -e "\n\e[42mUSO: analisisMixturaGausianas.sh [1] [2] [3] [4] [5] \e[0m"
    echo -e "\t [1] = datos_entrenamiento \n\t [2] = etiquetas_entrenamiento"
    echo -e "\t [3] = datos_test \n\t [4] = etiquetas_test"
    echo -e "\t [5] = nom_funcion_matlab "
    echo -e "\n\n Ejemplo ejecucion:"
    echo -e "\n ./analisisMixturaGausianas.sh ./data/spam/tr.dat ./data/spam/trlabels.dat ./data/spam/ts.dat ./data/spam/tslabels.dat p3\n\n\n"
    exit 1
fi

FICHERO_DATOS_ENTRENAMIENTO=$1
FICHERO_ETIQUETAS_ENTRENAMIENTO=$2
FICHERO_DATOS_TEST=$3
FICHERO_ETIQUETAS_TEST=$4
FUNCION_MATLAB=$5

if [ ! -f "$FICHERO_DATOS_ENTRENAMIENTO" ] 
then
    echo "ERROR NO SE ENCONTRO EL FICHERO $FICHERO_DATOS_ENTRENAMIENTO"
    exit 1
fi

if [ ! -f "$FICHERO_ETIQUETAS_ENTRENAMIENTO" ] 
then
    echo "ERROR NO SE ENCONTRO EL FICHERO $FICHERO_ETIQUETAS_ENTRENAMIENTO"
    exit 1
fi

if [ ! -f "$FICHERO_DATOS_TEST" ] 
then
    echo "ERROR NO SE ENCONTRO EL FICHERO $FICHERO_DATOS_TEST"
    exit 1
fi

if [ ! -f "$FICHERO_ETIQUETAS_TEST" ] 
then
    echo "ERROR NO SE ENCONTRO EL FICHERO $FICHERO_ETIQUETAS_TEST"
    exit 1
fi
rm log.csv
printf 'Numero-gaussianas|Numero-maximo-iteraciones|Porcentaje-error|Intervalo-Confianza\n' >> log.csv

for (( numGausianas=1; numGausianas<30; numGausianas++ ));
do
    for (( maxIteraciones=2; maxIteraciones<50; maxIteraciones*=2 ));
    do
        matlab -nodesktop -nosplash -nosoftwareopengl -r "$FUNCION_MATLAB('$FICHERO_DATOS_ENTRENAMIENTO','$FICHERO_ETIQUETAS_ENTRENAMIENTO','$FICHERO_DATOS_TEST','$FICHERO_ETIQUETAS_TEST',$numGausianas,$maxIteraciones)"
    done

done