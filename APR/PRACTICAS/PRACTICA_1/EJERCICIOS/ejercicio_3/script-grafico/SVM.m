#!/opt/octave/4.2.1/bin/octave --no-gui
# Funci�n que resuelve el problema. El primer argumento es para documentar
# los resultados y las gr�ficas.
#
function SVM (dataFileName, data, labels, C)
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

    
    hold on # No se borra lo dibujado al hacer otro plot
    title("Ejercicio 3 CONJUNTO NO SEPARABLE")
    plot(X,Y,'Color', 'black') # dibujo la frontera de decision
    plot(X, Y1,'Color', 'red', X, Y2,'Color', 'red'); # dibujo las líneas del margen
    #dibujo los elementos de la clase 1
    plot(data(labels==1, 1), data(labels==1, 2), 'sr',"markersize", 12,'Color', 'green',"markerfacecolor","auto");
    #dibujo los elementos de la clase 2
    plot(data(labels==2, 1), data(labels==2, 2), 'o',"markersize", 12,'Color', 'blue',"markerfacecolor","auto");
    # marco los vectores soporte buenos
    plot(vectores_soporte(abs(multiplicadores_lagrange)<C,1), vectores_soporte(abs(multiplicadores_lagrange)<C,2), '+',"markersize", 15,"linewidth",3,'Color', 'magenta');
    # marco los vectores soporte malos
    plot(vectores_soporte(abs(multiplicadores_lagrange)>=C,1), vectores_soporte(abs(multiplicadores_lagrange)>=C,2), 'x',"markersize", 13,"linewidth",3,'Color', 'black');
    axis([0,7,0,7],"square"); # rango grafica
    # dibujo leyenda
    legend("Frontera decision","Margen 1","Margen 2","Clase 1","Clase 2","Vector soporte bueno","Vector soporte malo","location","southoutside")
    margenStr = strcat("Margen = ", num2str(round(margen*100)/100));
    text(0.2,6.4,margenStr); # anoto el margen
    CStr = strcat("C = ", num2str(round(C*100)/100));
    text(0.2,6.8,CStr); # anoto la C en la grafica
    set (gca, "xgrid", "on");
    set(gca,'GridLineStyle',':')
    #grid on;
    


    tolerancia_margen_sv = (abs(multiplicadores_lagrange) == C) .* (1 - sign(multiplicadores_lagrange) .* (vectores_soporte * w' + w0));

    tolerancia_margen = zeros(size(labels));
    tolerancia_margen(res.sv_indices) = tolerancia_margen_sv;
    multiplicadores_lagrange = zeros(size(labels));
    multiplicadores_lagrange(res.sv_indices) = res.sv_coef;

    #Señalamos datos en cada punto
    text(data(:,1)+0.2, data(:,2)+0.2, num2str(tolerancia_margen,"%3.2f"),'fontsize',8);
    text(data(:,1)+0.2, data(:,2)-0.2, num2str(multiplicadores_lagrange,"%3.2f"),'fontsize',8);
    
# ... ... ...

pause;
endfunction
