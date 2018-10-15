import time
#array2= [210, 816, 357, 107, 889, 635, 733, 930, 842, 542]
def iterative_pract_opcional(array):
    l= []
    menores=[None,array[0]]
    for i in range(1,len(array)):
        max_freq=0
        max_freq_index = None
        if(array[i] > menores[-1]):
            menores.append(array[i])
        else:
            for e in range(1,len(menores)):
                if(array[i] < menores[e] ):
                    menores[e] = array[i]
                    break
            
    return menores


def iterative_pract_opcional_busqueda_dicotomica(array):
    l= []
    menores=[None,array[0]]
    for i in range(1,len(array)):
        max_freq=0
        max_freq_index = None
        if(array[i] > menores[-1]):
            menores.append(array[i])
        else:
            indice = recupera_indice_busquedaBinaria(menores[1:],array[i])
            menores[indice] = array[i]
    return menores

def recupera_indice_busquedaBinaria(array,elem):
    print(array)
    primero = 0
    ultimo = len(array)-1
    encontrado=0
    while primero<=ultimo and not encontrado:
        puntoMedio = (primero + ultimo) // 2
        if array[puntoMedio] <= elem and array[puntoMedio +1] > elem:
            encontrado = puntoMedio+1
        elif elem < array[puntoMedio]:
            ultimo = puntoMedio-1
        else:
            primero = puntoMedio+1
    print(encontrado+1)
    return encontrado+1


def get_timing(funcion, *argv):
    ti = time.time()
    funcion(argv)
    res = (time.time() - ti)*1000
    print("La funcion {} ha tardado {:.15f} ns".format(funcion.__name__,res))
    

if __name__ == "__main__":
    array = [10,3,5,12,7,20,18]
    array2= [210, 816, 357, 107, 889, 635, 733, 930, 842, 542]
    get_timing(iterative_pract_opcional,array2)
    get_timing(iterative_pract_opcional_busqueda_dicotomica,array2)



