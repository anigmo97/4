#!/opt/octave/4.2.1/bin/octave --no-gui
#Main ------------------------------------------------------------------

# Elegimos el programa para visualizaci�n de gr�ficos:
#
graphics_toolkit gnuplot;
#graphics_toolkit qt;


# Obtenemos los argumentos la l�nea de commando:  data_file labels_file C
#
if (nargin!=5 )
    printf("Usage: EJ3-GENERA-CSV.m <data_filename> <labels_filename> <int start_C> <int end_C> <increment_C>");
    exit(1);
end
arg_list = argv ();
eval(sprintf("data=load(\"-ascii\",\"%s\")",arg_list{1}));
eval(sprintf("labels=load(\"-ascii\",\"%s\")",arg_list{2}));
start_C=str2num(arg_list{3});
end_C=str2num(arg_list{4});
increment=str2num(arg_list{5});
#
# un valor grande de C simula la optimizaci�n st�ndard del caso separable

# Invocamos la funci�n que resuelve el problema
#
save_precision(3)
matriz_tabla = []
C=start_C
while C<= end_C
    matriz_tabla = [matriz_tabla ;SVM_TABLA(arg_list{1},data,labels,C)];  
    C = C + increment;  
end
printf("\nRESULTADOS EN EJ3-RESULTADOS.csv \n\n")
header="C,margen,num_soporte,num_buenos,num_malos";
csvwrite("EJ3-RESULTADOS.csv",header,'delimiter','')
csvwrite("EJ3-RESULTADOS.csv",matriz_tabla,'delimiter',',',"-append")
# ------------------------------------------------------------------------

