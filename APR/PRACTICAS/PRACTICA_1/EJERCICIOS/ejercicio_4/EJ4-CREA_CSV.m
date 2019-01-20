#!/opt/octave/4.2.1/bin/octave --no-gui
# Ejercicio 4 Practica 1 APR 2018/2019
# autor = Angel Igualada Moraga

load("data/spam/tr.dat");
load("data/spam/trlabels.dat");
load("data/spam/ts.dat");
load("data/spam/tslabels.dat");

printf("\t%-10s%-10s%-10s%-12s%-15s%5s\n","C","Kernel","grado","exactitud","intervalo","tiempo");
printf("=============================================================================\n");

[N, D] = size(ts);

kernel_names ={"lineal";"polinomial";"radial";"sigmoide"};
# -t 0 kernel lineal
# -t 1 kernel polinomial
# -t 2 radial cuadratico?
# -t 3 sigmoid
header="C,Kernel,grado,exactitud,intervalo,tiempo_ejecucion";
csvwrite("EJ4-RESULTADOS.csv",header,'delimiter','')

for kernel = [0,1,2,3]
    C = 0.01;
    if kernel == 1
        grados = [1,2,3,4];
    else
        grados = [1];
    endif
    while C <= 1000
            for grado = grados
                initial_time = time();
                model = svmtrain(trlabels, tr, ["-quiet -type_kernel ", num2str(kernel), " -c ", num2str(C), " -d ", num2str(grado)]);
                [prediction, accuracy, decision_values] = svmpredict(tslabels, ts, model, "-quiet");
                result_time = time() -initial_time;
                exactitud = accuracy(1) / 100;
                confidence = 1.96 * sqrt(exactitud * (1-exactitud) / N);
                nom_kernel = strcat(num2str(kernel),"-",kernel_names{kernel+1});
                #valores=strcat(num2str(C,4),nom_kernel,num2str(grado),num2str(exactitud),num2str(confidence),num2str(result_time));
                valores=strjoin({num2str(C,"%.4f"),nom_kernel,num2str(grado),num2str(exactitud,"%.4f"),num2str(confidence,"%.4f"),num2str(result_time,"%.4f")},",");
                csvwrite("EJ4-RESULTADOS.csv",valores,'delimiter','',"-append")
                printf("\t%-10d-%-15s%-10d%-10f%-15f%-5d\n",C, nom_kernel,grado, exactitud, confidence,result_time)
            endfor
        C *= 10;
    endwhile
endfor

printf("\n\n RESULTADOS GUARDADOS EN EJ4-RESULTADOS.csv \n\n")

