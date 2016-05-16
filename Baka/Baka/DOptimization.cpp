#include "DOptimization.h"

double DOptimization::isOptimal(double x)
{
	vector<vector<double>> matrixFisherInX = mainLocalModel->calcFisherMatrixInX(*mainOwnershipFunction, x);
	vector<vector<double>> matrixFisher = mainLocalModel->invertMatrix(mainLocalModel->getFisherMatrix());
	vector<double> multipleMatrix;
	multipleMatrix.resize(mainOwnershipFunction->clasterCount * 2);
	double sum = 0;
	for (int i = 0; i < mainOwnershipFunction->clasterCount * 2; i++)
	{
		for (int j = 0; j < mainOwnershipFunction->clasterCount * 2; j++)
		{
			multipleMatrix[i] += matrixFisher[i][j] * matrixFisherInX[j][i];
		}
		sum += multipleMatrix[i];
	}
	return sum;
}

DOptimization::DOptimization(Plan *beginNonsingularPlan, double beginPoint, double endPoint, double step)
{
	optimal = beginNonsingularPlan;
	mainLocalModel = new LocalModel();
	this->beginPoint = beginPoint;
	this->endPoint = endPoint;
	this->step = step;
}


DOptimization::~DOptimization()
{
}


Plan DOptimization::getPlan()
{
	return *optimal;
}

Plan DOptimization::optimizePlan()
{
	double maxX = 0;
	double maxTrace = 0;

	mainOwnershipFunction = new OwnershipFunction(3, 2, 0.1, optimal->remarkCount);
	mainOwnershipFunction->calcFCMWithoutCenter((*optimal)[0]);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, *optimal);

	for (double x = beginPoint; x <= endPoint; x += step)
	{
		double trace = isOptimal(x);

		if (abs(trace - mainOwnershipFunction->elementCount) > 0.0001)
		{
			if (abs(maxTrace - mainOwnershipFunction->elementCount) > abs(trace - mainOwnershipFunction->elementCount))
			{
				maxTrace = trace;
				maxX = x;
			}
		}
		else
			return *optimal;
	}
	
	optimal->enlarge(maxX);
	optimal->clean();

	cout << maxTrace << endl;

	return optimizePlan();
}
