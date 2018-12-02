package capas.aislantes;

import java.util.Collections;
import java.util.Random;

import org.opt4j.core.genotype.PermutationGenotype;
import org.opt4j.core.problem.Creator;
import capas.aislantes.Data;

public class CapasAislantesCreator implements Creator<PermutationGenotype<Integer>> {

	
	
	public PermutationGenotype<Integer> create(){
		
		PermutationGenotype<Integer> genotype = new PermutationGenotype<Integer>();
		// el genotipo estara formado por "numeroSimbolos" matematicos elegidos al azar
		// en nuestro caso la poblacion sera un conjunto de individuos, donde cada individuo son 5 simbolos
		for (int i=0;i<Data.numAislantes;i++) {
			genotype.add(i);
		}
		genotype.init(new Random());
		//Collections.shuffle(genotype); // otra opcion
		
		return genotype;
	}

}
