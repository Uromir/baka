#include "DOptimization.h"

bool DOptimization::isOptimal(double x)
{
	return false;
}

DOptimization::DOptimization(Plan beginNonsingularPlan, double beginPoint, double endPoint, double step)
{
	optimal = beginNonsingularPlan;
	mainOwnershipFunction = new OwnershipFunction(4, 2, 0.1, 5);
	this->beginPoint = beginPoint;
	this->endPoint = endPoint;
	this->step = step;
}


DOptimization::~DOptimization()
{
}


Plan DOptimization::getPlan()
{
	return optimal;
}

Plan DOptimization::optimizePlan()
{
	mainOwnershipFunction->calcFCM(optimal[0]);

	for (double x = beginPoint; x <= endPoint; x += step)
	{
		if (isOptimal(x))
		{
			return optimal;
		}
		else 
		{
			optimizePlan();
		}
	}
}
