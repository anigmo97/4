addpath('~/asigDSIC/ETSINF/apr/p2/BNT')
addpath(genpathKPM('~/asigDSIC/ETSINF/apr/p2/BNT'))
warning off all
N = 5; % num_nodos
P = 1; % Polucion es el nodo 1
F = 2; % Fumador es el nodo 2
C = 3; % Cancer de pulmon es el nodo 3
R = 4; % Rayos X es el nodo 4
D = 5; % Disnea es el nodo 5

grafo = zeros(N , N); % crea una matriz N x N con todo ceros

%%%% MARCAMOS CON 1's LAS POSICIONES A LAS QUE E PUEDE IR DESDE CADA NODO %%%
grafo(P,C)=1; % de P se puede ir a C
grafo(F,C)=1; % de F se puede ir a C
grafo(C,[D R])=1; % de C se puede ir a D y R

draw_graph(grafo); % VISUALIZAR GRAFO FUNCIONA

nodosDiscretos=1:N; % es un range(1,N+1)

tallaNodos = 2*ones(1,N);  %crea un vector de talla N con todos los elementos a 2

% CREAMOS LA RED BAYESIANA
redB = mk_bnet(grafo, tallaNodos, 'discrete', nodosDiscretos);

% TABLA DE PROBABILIDADES CONDICINALES false-> 1    true -> 2
% Damos valores a la TCP 
redB.CPD{P} = tabular_CPD(redB, P, [0.9 0.1]);
redB.CPD{F} = tabular_CPD(redB, F, [0.7 0.3]);
%redB.CPD{R} = tabular_CPD(redB, R, [0.8 0.1 0.1 0.2 0.1 0.7]); %REVISAR 3 VALORES
redB.CPD{R} = tabular_CPD(redB, R, [0.8 0.1 0.1 0.7]);
redB.CPD{D} = tabular_CPD(redB, D, [0.7 0.35 0.3 0.65]);
redB.CPD{C} = tabular_CPD(redB, C, [0.999 0.95 0.97 0.92 0.001 0.05 0.03 0.08]);

%CREAMOS UN MOTOR DE INFERENCIA
motor = jtree_inf_engine(redB);

fprintf('\n\nResultados ejercicio  B.2\n');

%CREAMOS LAS EVIDENCIAS
evidencia1 = cell(1,N);
evidencia1{F} = 1; % FUMADOR = 1 (false)
evidencia1{R} = 1; % RAYOS X = 1 (false)
evidencia1{D} = 2; % DISNEA = 2 (true)

% Insertamos la evidencia en el motor de inferencia
[motor1, logVerosim] = enter_evidence(motor, evidencia1);

%Calculamos la probabilidad de que  un paciente no fumador 
% no tenga cancer de pulmon si la radiografıa ha dado un resultado negativo
% pero sufre disnea  C pertenece a {false, true}
x = marginal_nodes(motor1, C);
x.T; %posibilidad false y true

fprintf("\tPROBABILIDAD DE QUE NO TENGA CANCER DE PULMON %f \n\n\n",x.T(1));

fprintf('Resultados ejercicio B.3');

%CREAMOS LAS EVIDENCIAS
evidencia2 = cell(1,N);
evidencia2{C} = 2; % CANCER = 2 (true)

% Insertamos la evidencia en el motor de inferencia
[motor2, logVerosim] = enter_evidence(motor, evidencia2);

%Calculamos la probabilidad conjunta maxima
[explMaxProb, logVer] = calc_mpe(motor2, evidencia2);

% ver los valores mas probables
explMaxProb;

% PRINTAMOS LA SOLUCION
arrayNombres = ["POLUCION" "FUMADOR" "CANCER" "RAYOS X" "DISNEA"];
BoolStrArray = {'false' 'true'};
fprintf("\nEXPLICACION MAS PROBABLE PARA QUE UN PACIENTE TENGA CANCER:\n");

for i=1:N
    valor = explMaxProb(i);
    fprintf("\t%s: %s \n",[arrayNombres(i),BoolStrArray{valor{1}}]);
end

