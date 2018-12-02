package capas.aislantes;

import java.util.ArrayList;

import org.opt4j.core.Objectives;
import org.opt4j.core.Objective.Sign;
import org.opt4j.core.problem.Evaluator;


public class CapasAislantesEvaluator implements Evaluator<ArrayList<Integer>> {
	public Objectives evaluate (ArrayList<Integer> phenotype) {
			
			int resultado = 0;
			
			for(int i =1; i<phenotype.size();i++) {
				resultado += Data.matriz[phenotype.get(i-1)][phenotype.get(i)];
			}
			
		
			//si hay un individuo que no cumple con ciertas restricciones le ponemos un valor de fitness indeseado
			// queremos minmizar la diferencia entre el resultado objetivo y el evaluado
			
			Objectives objectives = new Objectives();
			objectives.add("MAXIMO VALOR AISLAMIENTO", Sign.MAX ,resultado);
			return objectives;
			
		}
}
