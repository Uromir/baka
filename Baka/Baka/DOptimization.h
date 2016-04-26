﻿#include "Plan.h"
#include "OwnershipFunction.h"
#include "LocalModel.h"

#pragma once
class DOptimization
{
private:
	Plan optimal; // оптимальный план
	OwnershipFunction *mainOwnershipFunction;
	double beginPoint;
	double endPoint;
	double step;
	bool isOptimal(double x);
public:
	Plan getPlan(); // получить план
	DOptimization(Plan beginNonsingularPlan, double beginPoint, double endPoint, double step);
	Plan optimizePlan();
	~DOptimization();
};
