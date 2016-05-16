#include "DOptimization.h"

int main()
{
	Plan mainPlan(6);
	DOptimization mainOptimizationAlgoritm(&mainPlan, -0.9, 0.9, 0.01);
	Plan optimal = mainOptimizationAlgoritm.optimizeDiscretePlan();
	return 0;
}