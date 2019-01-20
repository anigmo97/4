function [errorFinal]= p3(datos,etiquetas,datosTest,etiquetasTest,nGauss,numIter)
    addpath('~/asigDSIC/ETSINF/apr/p2/BNT')
    addpath(genpathKPM('~/asigDSIC/ETSINF/apr/p2/BNT'))
    %warning off all;
    %CARGAMOS LOS DATOS DE ENTRENAMIENTO
    datApr = load(datos, '-ascii');
    etqApr = load(etiquetas, '-ascii');

    % BLANQUEAMOS LAS MUESTRAS (restar la media de de su dimension a cada
    % una y dividir por su desviacion tipica
    dataApr = zscore(datApr);
    
    %RE-ETIQUETAMOS LOS DATOS (ESTAN EN {0,1} y la libreria acepta >0) 
    etiqApr = etqApr + 1;

    % obtenemos las filas (numMuestras) y columnas
    [numVec dim] = size(dataApr);
    
    % obtenemos el numero de clases (asumimos que van de 1 a N de 1 en 1)
    numClas = max(etiqApr);
    
    %FIJAMOS EL NUMERO DE GAUSIANAS
    numGaus = nGauss;
    
    % creamos el grafo
    grafo = [ 0 1 1 ; 0 0 1 ; 0 0 0 ];
    numNodos = length(grafo);
    tallaNodos = [numClas numGaus dim];
    nodosDiscretos = [1 2];

    % CREAMOS LA RED BAYESIANA
    redB = mk_bnet(grafo, tallaNodos, 'discrete', nodosDiscretos);

    % TABLA DE PROBABILIDADES CONDICINALES
    % Damos valores a la TCP
    redB.CPD{1} = tabular_CPD(redB, 1);
    redB.CPD{2} = tabular_CPD(redB, 2);
    redB.CPD{3} = gaussian_CPD(redB, 3, 'cov_type', 'diag'); %definicion gausiana
    
    %POBLAR EN CADA VECTOR DE ENTRENAMIENTO EL CAMPO DONDE VAN LAS ETIQUETAS DE CLASE
    datosApr = cell(numNodos, numVec);
    datosApr(numNodos,:) = num2cell(dataApr', 1);
    datosApr(1,:) = num2cell(etiqApr', 1);

    %CREAMOS UN MOTOR DE INFERENCIA
    motor = jtree_inf_engine(redB);

    %FIJAMOS LOS PARAMETROS
    maxIter = numIter; %NUMERO MAXIMO DE ITERACIONES
    epsilon = 0.1;

    % ESTIMAMOS SUS PROBABILIDADES (ENTRENAMOS)
    [redB2, ll, motor2] = learn_params_em(motor, datosApr, maxIter, epsilon);
    
    %CARGAMOS LOS DATOS DE TEST
    datTest = load(datosTest, '-ascii');
    etqTest = load(etiquetasTest, '-ascii');

    % BLANQUEAMOS (NORMALIZAMOS) LOS DATOS
    dataTest = zscore(datTest);

    %RE-ETIQUETAMOS LOS DATOS (ESTAN EN {0,1} y la libreria acepta >0) 
    etiqTest = etqTest + 1;
    
    % CREAMOS UNA MATRIZ CON TODO ZEROS 
    % PARA ALMACENAR PARA CADA MUESTRA LAS PROBABILIDADES A POSTERIORI 
    % DE CADA CLASE
    p = zeros(length(dataTest), numClas);

    %% creamos un cell array vacio para las observaciones
    evidencia = cell(numNodos,1);

    for i=1:length(dataTest)
        evidencia{numNodos} = dataTest(i,:)'; %tomamos la clase mas probable como evidencia
        [motor3, ll] = enter_evidence(motor2, evidencia); %insertamos la evidencia
        m = marginal_nodes(motor3, 1); %CALCULAMOS
        p(i,:) = m.T';
    end
    
    %NO ESTA EN EL PDF
    numErrores = 0;
    for i=1:length(p)
        [val,index] = max(p(i,:));
        if(index ~= etiqTest(i)) 
            numErrores = numErrores + 1; 
        end
    end
    errorFinal = 100*(numErrores/length(p));
    confidence = 1.96 * sqrt((numErrores/length(p)) * (1-(numErrores/length(p))) / length(p));

    % lo guardamos en un csv
    M = [ nGauss maxIter errorFinal confidence*100]
    dlmwrite("log.csv",M,'delimiter','|',"-append");
    exit;
    end
    


    %matlab -nodesktop -nosplash -nosoftwareopengl -r "p3("data/spam/tr.dat","data/spam/trlabels.dat","data/spam/ts.dat","data/spam/tslabels.dat",2,2)"
