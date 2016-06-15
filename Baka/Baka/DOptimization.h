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
	vector<vector<double>> multMatrix(vector<vector<double>> a, vector<vector<double>> b);
	double beginPoint;
	double endPoint;
	double step;
	double isOptimal(Point x, Plan plan);
	double isOptimal(Point x);
public:
	Plan getPlan(); // получить план
	DOptimization(Plan *beginNonsingularPlan, double beginPoint, double endPoint, double step);
	bool optimizeСontinuousPlan();
	vector<Point> optimizeDiscretePlan();
	Plan calcOptimalPlan();
	vector<vector<double>> getMNK(int i);
	double getRespondValue(int i);
	Point calcX();
	vector<vector<double>> getFisherMatrix();
	~DOptimization();
};

