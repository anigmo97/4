#!/opt/octave/4.2.1/bin/octave --no-gui
# Funci�n que resuelve el problema. El primer argumento es para documentar
# los resultados y las gr�ficas.
#
function [vector_resultado] = SVM_TABLA(dataFileName, data, labels, C)
    output_precision(3)
    printf("inicio SVM")
    # SVM - kernel lineal - C == 1000
    options = strcat({"-t 0 -c "},{num2str(C)})
    res = svmtrain(labels, data,  options{1,1})

    # Multiplicadores de Lagrange
    multiplicadores_lagrange = res.sv_coef;
    # Vectores soporte
    vectores_soporte = data(res.sv_indices,:);

    # Vector de pesos, umbral
    w  = multiplicadores_lagrange' * vectores_soporte;
    w0 = -res.rho;

    # Margen
    margen = 2 / sqrt(w * w');

    # Recta y margenes (1 y 2)
    m  = -  w(1)         / w(2)
    b  = -  w0           / w(2);
    b1 = - (w0 - 1) / w(2);
    b2 = - (w0 + 1) / w(2);
    

    X  = [0:0.001:7]; # rango de la linea a dibujar
    Y  = m * X + b ;
    Y1 = m * X + b1;
    Y2 = m * X + b2;
    
    num_vectores_soporte_buenos = rows(vectores_soporte(abs(multiplicadores_lagrange)<C,1))
    num_vectores_soporte_malos = rows(vectores_soporte(abs(multiplicadores_lagrange)>=C,1))
    vector_resultado = [C margen rows(vectores_soporte) num_vectores_soporte_buenos num_vectores_soporte_malos]

endfunction
