addpath('~/asigDSIC/ETSINF/apr/p2/BNT')
addpath(genpathKPM('~/asigDSIC/ETSINF/apr/p2/BNT'))
N = 4; % num_nodos
C = 1; % Cloudy (nublado) es el nodo 1
S = 2; % Sprinkler (aspersor) es el nodo 2;
R = 3; % Rain (llueve) es el nodo 3;
W = 4; % WetGrass (césped húmedo) es el nodo 4;

grafo = zeros(N , N); % crea una matriz N x N con todo ceros

%%%% MARCAMOS CON 1's LAS POSICIONES A LAS QUE E PUEDE IR DESDE CADA NODO %%%
grafo(C, [R S]) = 1;  % de C se puede ir a R y S
grafo(R,W) = 1; % de R se puede ir a W
grafo(S,W) = 1; % de S se puede ir a W

draw_graph(grafo); % VISUALIZAR GRAFO FUNCIONA

nodosDiscretos = 1:N; % es un range(1,N+1)
tallaNodos = 2*ones(1,N); %crea un vector de talla N con todos los elementos a 2

% CREAMOS LA RED BAYESIANA
redB = mk_bnet(grafo, tallaNodos, 'discrete', nodosDiscretos); 

% TABLA DE PROBABILIDADES CONDICINALES false-> 1    true -> 2
% Damos valores a la TCP 
redB.CPD{W} = tabular_CPD(redB,W, [1.0 0.1 0.1 0.01 0.0 0.9 0.9 0.99]);
redB.CPD{C} = tabular_CPD(redB,C, [0.5 0.5]);
redB.CPD{S} = tabular_CPD(redB,S, [0.5 0.9 0.5 0.1]);
redB.CPD{R} = tabular_CPD(redB,R, [0.8 0.2 0.2 0.8]);

%INICIALIZAMOS EL GENERADOR DE NUMEROS ALEATORIOS
semilla = 0;
rng(semilla);

% FIJAMOS EL NUMERO DE MUESTRAS A GENERAR
nMuestras = 1000;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DATOS COMPLETOS %%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% CREAMOS UNA MATRIZ DE N FILAS Y nMuestras COLUMNAS
muestras = cell(N,nMuestras);

% GENERAMOS LAS MUESTRAS 
for i=1:nMuestras 
    muestras(:,i) = sample_bnet(redB);
end

%podemos ver que lo que se ha generado son muestras a 1 (false) y 2 (true)
%muestras'

%CREAMOS OTRA RED BAYESIANA VACIA
redAPR = mk_bnet(grafo, tallaNodos);
redAPR.CPD{C} = tabular_CPD(redAPR, C);
redAPR.CPD{R} = tabular_CPD(redAPR, R);
redAPR.CPD{S} = tabular_CPD(redAPR, S);
redAPR.CPD{W} = tabular_CPD(redAPR, W);

% ESTIMAMOS SUS PROBABILIDADES CON LAS MUESTRAS QUE GENERAMOS ANTES
redAPR2=learn_params(redAPR, muestras);

%PREPARAMOS UNA ESTRUCTURA PARA VISUALIZAR LAS PROBABILIDADES ESTIMADAS
TPCaux = cell(1,N);
for i=1:N 
    s=struct(redAPR2.CPD{i}); TPCaux{i}=s.CPT; 
end

%VEMOS LAS PROBABILIDADES ESTIMADAS DE CADA NODO
fprintf("\n\n\n\n\nPROBALIDADES ESTIMADAS CON DATOS COMPLETOS\n");
fprintf("numMuestras = %d",nMuestras);
fprintf("\n\nPROBALIDADES ESTIMADAS PARA EL NODO W\n");
dispcpt(TPCaux{W});
fprintf("\n\nPROBALIDADES ESTIMADAS PARA EL NODO C\n");
dispcpt(TPCaux{C});
fprintf("\n\nPROBALIDADES ESTIMADAS PARA EL NODO S\n");
dispcpt(TPCaux{S});
fprintf("\n\nPROBALIDADES ESTIMADAS PARA EL NODO R\n");
dispcpt(TPCaux{R});

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DATOS INCOMPLETOS %%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% PARTIMOS DE UNA MATRIZ DE N FILAS Y nMuestras COLUMNAS
muestrasS = muestras;

%INICIALIZAMOS EL GENERADOR DE NUMEROS ALEATORIOS
semilla = 0; 
rng(semilla);

%CREAMOS UNA MATRIZ SEÑALANDO LAS POSICIONES QUE SE OCULTARAN
ocultas = rand(N, nMuestras) > 0.5;

%COGEMOS LAS POSICIONES DE LAS QUE SE OCULTARAN
[I,J] = find(ocultas);

%OCULTAMOS LAS PARTES QUE TENGAMOS QUE OCULTAR
for k=1:length(I) 
    muestrasS{I(k), J(k)} = []; 
end

%podemos ver que lo que se ha generado.
% son muestras a 1 (false) , 2 (true) y []
%muestrasS'


%CREAMOS OTRA RED BAYESIANA VACIA
redEM = mk_bnet(grafo, tallaNodos, 'discrete', nodosDiscretos);
redEM.CPD{C} = tabular_CPD(redEM, C);
redEM.CPD{R} = tabular_CPD(redEM, R);
redEM.CPD{S} = tabular_CPD(redEM, S);
redEM.CPD{W} = tabular_CPD(redEM, W);

%CREAMOS UN MOTOR DE INFERENCIA
motorEM = jtree_inf_engine(redEM);

%FIJAMOS LOS PARAMETROS
maxIter = 1000;  %NUMERO MAXIMO DE ITERACIONES
eps = 1e-4;

semilla = 0; rng(semilla); % ¿por que?

% ESTIMAMOS SUS PROBABILIDADES CON LAS MUESTRAS QUE GENERAMOS ANTES
[redEM2, trazaLogVer] = learn_params_em(motorEM, muestrasS, maxIter, eps);

%PREPARAMOS UNA ESTRUCTURA PARA VISUALIZAR LAS PROBABILIDADES ESTIMADAS
auxTPC = cell(1,N);
for i=1:N 
    s=struct(redEM2.CPD{i}); 
    auxTPC{i}=s.CPT; 
end


%VEMOS LAS PROBABILIDADES ESTIMADAS DE CADA NODO
fprintf("\n\n\n\n\nPROBALIDADES ESTIMADAS CON DATOS INCOMPLETOS\n");
fprintf("numMuestras = %d  Máximo numero de iteraciones = %d",[nMuestras, maxIter]);
fprintf("\n\nPROBALIDADES ESTIMADAS PARA EL NODO W\n");
dispcpt(auxTPC{W});
fprintf("\n\nPROBALIDADES ESTIMADAS PARA EL NODO C\n");
dispcpt(auxTPC{C});
fprintf("\n\nPROBALIDADES ESTIMADAS PARA EL NODO S\n");
dispcpt(auxTPC{S});
fprintf("\n\nPROBALIDADES ESTIMADAS PARA EL NODO R\n");
dispcpt(auxTPC{R});