#include "DOptimization.h"

bool DOptimization::isOptimal(double x)
{
	vector<vector<double>> matrixFisherInX = mainLocalModel->invertMatrix(mainLocalModel->calcFisherMatrixInX(*mainOwnershipFunction, x));
	vector<vector<double>> matrixFisher = mainLocalModel->getFisherMatrix();
	vector<double> multipleMatrix;
	double sum = 0;
	for (int i = 0; i < mainOwnershipFunction->clasterCount * 2; i++)
	{
		for (int j = 0; j < mainOwnershipFunction->clasterCount * 2; j++)
		{
			multipleMatrix[i] += (*mainOwnershipFunction)[i][j] * matrixFisherInX[j][i];
		}
		sum += multipleMatrix[i];
	}
	return false;
}

DOptimization::DOptimization(Plan beginNonsingularPlan, double beginPoint, double endPoint, double step)
{
	optimal = beginNonsingularPlan;
	mainOwnershipFunction = new OwnershipFunction(4, 2, 0.1, 5);
	mainLocalModel = new LocalModel();
	this->beginPoint = beginPoint;
	this->endPoint = endPoint;
	this->step = step;
	mainOwnershipFunction->calcFCM(optimal[0]);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, optimal);
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

	for (double x = beginPoint; x <= endPoint; x += step)
	{
		if (isOptimal(x))
		{
			return optimal;
		}
		else 
		{
			optimal.enlarge();
			optimizePlan();
		}
	}
}
