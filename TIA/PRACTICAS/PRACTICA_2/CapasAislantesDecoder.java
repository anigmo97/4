package capas.aislantes;

import java.util.ArrayList;
import org.opt4j.core.genotype.PermutationGenotype;
import org.opt4j.core.problem.Decoder;

public class CapasAislantesDecoder implements Decoder<PermutationGenotype<Integer>,ArrayList<Integer>>{


	//no es necesario devolverlo como String , pero si un tipo imprimibl/visualizable en el visor Opt4J
	
	public ArrayList<Integer> decode(PermutationGenotype<Integer> genotype){
		ArrayList<Integer> phenotype = new ArrayList<Integer>();
		for(int elem : genotype) {
			phenotype.add(elem);
		}
		return phenotype;
	}

}
