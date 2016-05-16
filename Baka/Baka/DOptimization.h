#include "Plan.h"
#include "OwnershipFunction.h"
#include "LocalModel.h"
#include <iostream>
#include <algorithm>

using namespace std;

#pragma once
class DOptimization
{
private:
	Plan *optimal; // оптимальный план
	OwnershipFunction *mainOwnershipFunction;
	LocalModel *mainLocalModel;
	double beginPoint;
	double endPoint;
	double step; // ожидается 0.01
	double isOptimal(double x);
public:
	Plan getPlan(); // получить план
	DOptimization(Plan *beginNonsingularPlan, double beginPoint, double endPoint, double step);
	Plan optimizeСontinuousPlan();
	Plan optimizeDiscretePlan();
	~DOptimization();
};

