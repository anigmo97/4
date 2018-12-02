package capas.aislantes;

import org.opt4j.core.problem.ProblemModule;



public class CapasAislantesModule extends ProblemModule {

	protected void config() {
		bindProblem(CapasAislantesCreator.class, CapasAislantesDecoder.class, CapasAislantesEvaluator.class);
	}
}
