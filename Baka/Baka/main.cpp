#include "DOptimization.h"

int main()
{
	Plan mainPlan(4);
	DOptimization mainOptimizationAlgoritm(&mainPlan, -1, 1, 0.01);
	mainOptimizationAlgoritm.optimizePlan();
	return 0;
}