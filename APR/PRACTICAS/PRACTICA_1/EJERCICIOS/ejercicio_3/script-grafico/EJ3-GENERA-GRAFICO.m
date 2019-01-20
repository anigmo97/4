#!/opt/octave/4.2.1/bin/octave --no-gui
#Main ------------------------------------------------------------------

# Elegimos el programa para visualizaci�n de gr�ficos:
#
graphics_toolkit gnuplot;
#graphics_toolkit qt;


# Obtenemos los argumentos la l�nea de commando:  data_file labels_file C
#
if (nargin!=3 )
    printf("Usage: EJ3-GENERA-GRAFICO.m <data_filename> <labels_filename> <int C> ");
    exit(1);
end
arg_list = argv ();
eval(sprintf("data=load(\"-ascii\",\"%s\")",arg_list{1}));
eval(sprintf("labels=load(\"-ascii\",\"%s\")",arg_list{2}));
C=str2num(arg_list{3});
#
# un valor grande de C simula la optimizaci�n st�ndard del caso separable

# Invocamos la funci�n que resuelve el problema
#
SVM(arg_list{1},data,labels,C);
# ------------------------------------------------------------------------

