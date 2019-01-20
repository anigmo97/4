#!/opt/octave/4.2.1/bin/octave --no-gui
# Ejercicio 5 Practica 1 APR 2018/2019
# autor = Angel Igualada Moraga

load("data/usps/tr.dat");
load("data/usps/trlabels.dat");
load("data/usps/ts.dat");
load("data/usps/tslabels.dat");

[N, D] = size(ts);

for kernel = [2] # el mejor kernel era el radial
    C = 10;
    model = svmtrain(trlabels, tr, ["-quiet -type_kernel ", num2str(kernel), " -c ", num2str(C),]);
    vectores_soporte_con_etiqueta = [tr(model.sv_indices,:) trlabels(model.sv_indices)];
    for clase = [0,1,2,3,4,5,6,7,8,9] # nuestros conjuntos de datos tienen esas clases
        muestra_clase_n = tr(trlabels==clase, :)(1,:);
        
        ## IMAGEN MUESTRA CLASE N
        muestra_clase_n=reshape(muestra_clase_n,16,16);
        #figure; #con esto no se cierran las ventanas
        imshow(muestra_clase_n',[])
        title (strcat("Muestra Clase=",int2str(clase)))
        pause()

        #  OBTENEMOS UN VECTOR SOPORTE DE LA CLASE N SI LO HAY
        indices_vectores_soporte_clase_n = find(vectores_soporte_con_etiqueta(:,end)==clase);
        printf("Se han encontrado %d vectores soporte de la clase %d\n",rows(indices_vectores_soporte_clase_n),clase)
        vector_soporte_clase_n = vectores_soporte_con_etiqueta(indices_vectores_soporte_clase_n(1),1:end-1);
        
        ## IMAGEN VECTOR SOPORTE CLASE N
        vector_soporte_clase_n=reshape(vector_soporte_clase_n,16,16);
        #figure; #con esto no se cierran las ventanas
        imshow(vector_soporte_clase_n',[])
        title (strcat("Vector Soporte Clase=",int2str(clase)))
        pause()

        #MUESTRA LAS DOS IMAGENES JUNTAS
        #figure; #con esto no se cierran las ventanas
        subplot(1,2,1),imshow(muestra_clase_n',[])
        title (strcat("Muestra Clase=",int2str(clase)))
        #set(gca,'position',[0.1 0.1 0.3 0.85])
        subplot(1,2,2),imshow(vector_soporte_clase_n',[])
        #set(gca,'position',[0.55 0.1 0.3 0.85])
        title (strcat("Vector Soporte Clase=",int2str(clase)))
        pause()
        clf reset

    endfor
endfor

#fprintf("%s\n",cell2mat(resultados))

