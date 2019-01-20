{
    int z= 2;
    //print(array_enteros[0]); //da error
    int array_enteros[4];
    //print(array_enteros[0]);  // da -1
    //bool array_booleanos[2];
    //print(array_enteros[10]); // deberia fallar
    //print(array_enteros[2]);
    //print(array_enteros[z]);
    // print(array_enteros[true]); da error
    // print(array_enteros[-2]); da error
    array_enteros[0] = 7;print(array_enteros[0]);
    array_enteros[1] = 7;print(array_enteros[1]);
    array_enteros[2] = 7;print(array_enteros[2]);
    array_enteros[z] = 9;print(array_enteros[z]);
    array_enteros[3] = 7;print(array_enteros[3]);
    array_enteros[4] = 7;print(array_enteros[4]); //deberia fallar CORREGIR ACCESO
    
    array_enteros[0] = 4;print(array_enteros[0]);
    array_enteros[0] += 2;print(array_enteros[0]);
    array_enteros[0] /= 2;print(array_enteros[0]); 
    array_enteros[0] *= 10;print(array_enteros[0]);
    
}
