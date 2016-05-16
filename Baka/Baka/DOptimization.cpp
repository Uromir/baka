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

Plan DOptimization::optimizeÑontinuousPlan()
{
	double maxX = 0;
	double maxTrace = 0;

	mainOwnershipFunction = new OwnershipFunction(3, 2, 0.1, optimal->remarkCount);
	mainOwnershipFunction->calcFCMWithoutCenter((*optimal)[0]);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, *optimal);

	for (double x = beginPoint; x <= endPoint; x += step)
	{
		double trace = isOptimal(x);

		if (abs(maxTrace - mainOwnershipFunction->elementCount) > abs(trace - mainOwnershipFunction->elementCount))
		{
			maxTrace = trace;
			maxX = x;
		}
	}

	if (abs(maxTrace - mainOwnershipFunction->elementCount) > 0.0001)
	{
		optimal->enlarge(maxX);
		optimal->clean();

		cout << maxTrace << endl;

		return optimizeÑontinuousPlan();
	} 
	else
	{
		return *optimal;
	}
}

Plan DOptimization::optimizeDiscretePlan()
{
	double maxX = 0;
	double maxTrace = 0;
	vector<double> arrayX = (*optimal)[0];

	mainOwnershipFunction = new OwnershipFunction(3, 2, 0.1, optimal->remarkCount);
	mainOwnershipFunction->calcFCMWithoutCenter(arrayX);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, *optimal);

	for (double x = beginPoint; x <= endPoint; x += step)
	{
		if (find(arrayX.begin(), arrayX.end(), x) == arrayX.end())
		{
			double trace = isOptimal(x);

			if (maxTrace < trace)
			{
				maxTrace = trace;
				maxX = x;
			}
		}		
	}

	optimal->enlargeDiscrete(maxX);

	double minX = 0;
	double minTrace = 0;

	mainOwnershipFunction = new OwnershipFunction(3, 2, 0.1, optimal->remarkCount);
	mainOwnershipFunction->calcFCMWithoutCenter((*optimal)[0]);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, *optimal);

	for (int i = 0; i < (*optimal)[0].size(); i++)
	{
		double trace = isOptimal((*optimal)[0][i]);

		if (minTrace > trace || minTrace == 0)
		{
			minTrace = trace;
			minX = (*optimal)[0][i];
		}
	}

	if (minX == maxX)
		return *optimal;
	else
	{
		optimal->reduce(minX);
		return optimizeDiscretePlan();
	}
}
