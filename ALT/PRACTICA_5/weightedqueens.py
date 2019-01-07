# -*- coding: utf-8 -*-
# Fecha: 15/12/2018
# Alumno(s):Angel Igualada Moraga
import numpy as np
import random
import heapq # a priority queue, es un minheap
import argparse
import time

def pessimistic(N,weights):
    return sum(max(weights[row,col] for col in range(N))
               for row in range(N))

def evaluate(s,weights):
    return sum(weights[row,col] for row,col in enumerate(s))

def show_solution(s,N,weights):
    print("    "+"".join("%5d" % c for c in  range(N)))
    for r in range(N):
        print("%3d %s" % (r,"".join((" %4d" % (weights[r,c]))
                                              if s[r]==c else "   --"
                                              for c in range(N))))
    print("+".join(str(weights[r,c]) for r,c in enumerate(s)),"=",
          evaluate(s,weights))

def backtracking(N,weights):
    # weights es una matriz NxN
    assert((type(weights) is np.ndarray) and (weights.shape == (N,N)))
    bestSolution, bestScore = None, pessimistic(N,weights)
    def is_promising(s, newcol):
        # si tenemos el estado s = [1,3,0]
        # en row 0 la reina va en col 1
        # en row 1 la reina va en col 3
        # en row 2 la reina va en col 0
        # check if a new queen can be put in coordinate [newrow,newcol]
        newrow = len(s)
        return all(newcol != col and newrow-row != abs(newcol-col)
                   for row,col in enumerate(s))
    def back(s):
        nonlocal bestSolution, bestScore
        if len(s) == N:
            current = evaluate(s,weights)
            if current < bestScore:
                bestScore = current
                bestSolution = s
        else:
            for col in range(N):
                if is_promising(s, col):
                    back(s+[col])
    back([])
    return bestSolution, bestScore

def optimisticSimple(s,weights,parentScore):
    # parentScore es el score del padre de s
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################     INICIO CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    col = s[-1] # columna_recien_añadida
    row = len(s)-1 # fila_columna_recien_añadida ¿-1?
    opt = parentScore + weights[row,col] - min([weights[row,x] for x in range(N) ])
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################        FIN CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    return opt
    
def optimisticVert(s,weights,parentScore):
    Infty=2**30
    # parte conocida:
    opt = sum(weights[row,col] for row,col in enumerate(s))
    # cota optimista de la parte que nos queda por completar:
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################     INICIO CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    
    # COMPLETAR, se aconseja usar min con un iterador y el argumento default
    for row in range(len(s),N):
        opt += min([weights[row,x] for x in range(N) if x not in s],default=Infty) # default no necesario

    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################        FIN CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    return opt

def optimisticEllaborate(s,weights,parentScore):
    Infty=2**30
    # la diagonal ppal:
    #     c0 c1 c2 c3
    #    -------------
    # r0 | 0| 1| 2| 3|
    #    -------------
    # r1 |-1| 0| 1| 2|
    #    -------------
    # r2 |-2|-1| 0| 1|
    #    -------------
    # r3 |-3|-2|-1| 0|
    #    -------------
    # esto se consigue para [r,c] con el valor c-r

    # la diagonal inversa:
    #     c0 c1 c2 c3
    #    -------------
    # r0 |-3|-2|-1|0 |
    #    -------------
    # r1 |-2|-1| 0| 1|
    #    -------------
    # r2 |-1| 0| 1| 2|
    #    -------------
    # r3 | 0| 1| 2| 3|
    #    -------------
    # se consigue con r+c-(N-1)
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################     INICIO CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    opt = 0
    posiciones_ocupadas = enumerate(s)
    diag_principal    = set()
    diag_inversa      = set()

    def posicionNoAmenazada(row,col):
        return col not in s and (col-row) not in diag_principal and (row-col) not in diag_inversa

    for row,col in posiciones_ocupadas:
        diag_principal.add(col-row) # añado valor diagonal principal
        diag_inversa.add(row-col-(N-1)) #añado valor diagonal inversa
        opt += weights[row,col] #calculo la cota de la parte conocida
    
    # parte desconocida
    for row in range(len(s),N):
        opt += min([weights[row,col] for col in range(N) if posicionNoAmenazada(row,col)],default=Infty)
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################        FIN CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ########################################################################################### 
    return opt

def branchAndBound(N,weights,
                   verbosity=0,
                   explicitPruning=False,
                   reportStatistics=False,
                   optimistic=optimisticSimple):
    # weights es una matriz NxN
    assert((type(weights) is np.ndarray) and (weights.shape == (N,N)))

    def branch(s):
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################     INICIO CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
        # COMPLETAR, solamente debe ramificar las columnas no amenazadas
        #print(s)
        #input()
        def amenazada(col):
            longSol = len(s)
            return not all(col != s[i] and longSol-i != abs(col-s[i]) for i in range(longSol))
        return [s+[col] for col in range(N) if not amenazada(col)]
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################        FIN CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################

    def is_complete(s):
        return len(s)==N

    def initial_score():
        return sum(min(weights[row,col] for col in range(N))
                   for row in range(N))

    def implicit():
        A = [] # empty priority queue
        x = None
        fx = pessimistic(N,weights)

        # anyadimos el estado inicial:
        s = []
        opt = initial_score()
        heapq.heappush(A,(opt,s))
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################     INICIO CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
        iter = 0
        maxA = 0
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################        FIN CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################

        # bucle principal de ramificacion y poda con PODA IMPLICITA
        while len(A)>0 and A[0][0] < fx:
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################     INICIO CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
            # COMPLETAR: contadores para que esto funcione
            iter += 1
            lenA = len(A)
            maxA = max(lenA,maxA)
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################        FIN CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
            score_s,s = heapq.heappop(A)
            if verbosity > 1:
                print("Iter. %04d |A|=%05d max|A|=%05d fx=%04d len(s)=%02d score_s=%04d" % \
                      (iter,lenA,maxA,fx,len(s),score_s))
            for child in branch(s):
                if is_complete(child): # si es terminal
                    # seguro que es factible
                    # falta ver si mejora la mejor solucion en curso
                    opt_child = evaluate(child,weights)
                    if opt_child < fx:
                        if verbosity > 0:
                            print("MEJORAMOS",x,fx,"CON",child,opt_child)
                        x, fx = child, opt_child
                else: # no es terminal
                    # la función optimistic recibe como 3er argumento
                    # el score del padre para poder realizar el
                    # cálculo de manera incremental:
                    opt_child = optimistic(child,weights,score_s)
                    # lo metemos en el cjt de estados activos si supera
                    # la poda por cota optimista:
                    if opt_child < fx:
                        heapq.heappush(A,(opt_child,child))
        if verbosity > 0:
            print("%4d Iterations, max|A|=%05d" % (iter,maxA))
        return x,fx

    def explicit():
        A = [] # empty priority queue
        x = None
        fx = pessimistic(N,weights)

        # anyadimos el estado inicial:
        s = []
        opt = initial_score()
        heapq.heappush(A,(opt,s))
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
    #################################     INICIO CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
        iter = 0
        maxA = 0
        # bucle principal de ramificacion y poda con PODA EXPLICITA

        while len(A)>0: # PODA EXPLÍCITA
            iter += 1
            lenA = len(A)
            maxA = max(lenA,maxA)
            score_s,s = heapq.heappop(A)
            if verbosity > 1:
                print("Iter. %04d |A|=%05d max|A|=%05d fx=%04d len(s)=%02d score_s=%04d" % \
                (iter,lenA,maxA,fx,len(s),score_s))
            for child in branch(s):
                if is_complete(child):
                    opt_child = evaluate(child,weights)
                    if opt_child < fx:
                        x, fx = child, opt_child
                        # HACER AQUÍ LA PODA EXPLÍCITA
                        # eliminando de A los elementos >=fx
                        changed = False
                        i=0
                        while i < len(A):
                            if A[i][0] >= fx:
                                changed = True
                                A.pop(i)
                            else:
                                i+=1
                        if changed:
                            heapq.heapify(A)
                else: # no es terminal
                    opt_child = optimistic(child,weights,score_s)
                    if opt_child < fx: # poda por cota optimista
                        heapq.heappush(A,(opt_child,child))
    ############################################(row-col-(N-1))###############################################
    ###########################################################################################
    ###########################################################################################
    #################################        FIN CODIGO AÑADIDO   #############################
    ###########################################################################################
    ###########################################################################################
    ###########################################################################################
        if verbosity > 0:
            print("%4d Iterations, max|A|=%05d" % (iter,maxA))
        return x,fx
    
    return explicit() if explicitPruning else implicit()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-v", "--verbosity", help="increase output verbosity",
                        type=int, choices=[0,1,2], default = 0)
    parser.add_argument("--seed", help="seed of random number generator", type=int, default=1234)
    parser.add_argument("-N", help="chess board size", type=int)
    parser.add_argument("--Nmin", help="minimum chess board size", type=int, default=4)
    parser.add_argument("--Nmax", help="maximum chess board size", type=int, default=10)
    parser.add_argument("-W", help="maximum weight", type=int, default=9999)
    parser.add_argument("-w", "--weights", help="show weights",
                        action="store_true", default=False)
    parser.add_argument("-o", "--optimistic", help="Type of optimistic function (simple, vert, ellaborate)", default="simple", choices=["simple","vert","ellaborate"])
    parser.add_argument("-s", "--statistics", help="Report statistic information",
                        action="store_true")
    parser.add_argument("-b", "--backtracking", help="Also computes solution with backtracking",
                        action="store_true", default=False)
    parser.add_argument("-i", "--implicit", help="Also computes solution B&B implicit pruning",
                        action="store_true", default=False)
    parser.add_argument("-e", "--explicit", help="Also computes solution B&B explicit pruning",
                        action="store_true", default=False)
    args = parser.parse_args()
    random.seed(args.seed)
    np.random.seed(args.seed)
    N = args.N if args.N != None else random.randint(args.Nmin,args.Nmax)
    weights = np.random.randint(args.W, size=(N,N))
    optimistic = optimisticSimple
    if args.optimistic.lower() == 'vert':
        optimistic = optimisticVert
    elif args.optimistic.lower() == 'ellaborate':
        optimistic = optimisticEllaborate
    print("N",N)
    if args.weights:
        print("weights")
        print(weights)
    if args.backtracking:
        print("Probamos con backtracking:")
        time_backtracking_start = time.process_time()
        x,fx = backtracking(N,weights)
        time_backtracking_stop = time.process_time()
        print(x,fx,"(ellapsed time %.5f)" %
              (time_backtracking_stop-time_backtracking_start,))
        show_solution(x,N,weights)
    if args.implicit:
        print("Ahora probamos con branch and bound poda implícita:")
        time_bbimplicit_start = time.process_time()
        x,fx = branchAndBound(N,weights,
                              optimistic = optimistic,
                              reportStatistics=args.statistics,
                              verbosity = args.verbosity)
        time_bbimplicit_stop = time.process_time()
        print(x,fx,"(ellapsed time %.5f)" %
              (time_bbimplicit_stop-time_bbimplicit_start,))
        show_solution(x,N,weights)
    if args.explicit:
        print("Ahora probamos con branch and bound poda explícita:")
        time_bbexplicit_start = time.process_time()
        x,fx = branchAndBound(N,weights,
                              optimistic = optimistic,
                              explicitPruning = True,
                              reportStatistics=args.statistics,
                              verbosity = args.verbosity)
        time_bbexplicit_stop = time.process_time()
        print(x,fx,"(ellapsed time %.5f)" %
              (time_bbexplicit_stop-time_bbexplicit_start,))
        show_solution(x,N,weights)

