#include "DOptimization.h"

vector<vector<double>> DOptimization::multMatrix(vector<vector<double>> a, vector<vector<double>> b)
{
	vector<vector<double>> result;
	result.resize(a.size());
	for (int i = 0; i < a.size(); i++)
	{
		result[i].resize(b[i].size());

		for (int j = 0; j < b[i].size(); j++)
		{
			for (int k = 0; k < b.size(); k++)
			{
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return result;
}

double DOptimization::isOptimal(Point x)
{
	vector<vector<double>> matrixFisherInX = mainLocalModel->calcLinearFisherMatrixInX(*mainOwnershipFunction, x);
	vector<vector<double>> matrixFisher = mainLocalModel->invertMatrix(mainLocalModel->getFisherMatrix());
	vector<double> multipleMatrix;
	int sizeModel = x.coord.size() + 1;
	multipleMatrix.resize(mainOwnershipFunction->clasterCount * sizeModel);
	double sum = 0;
	for (int i = 0; i < mainOwnershipFunction->clasterCount * sizeModel; i++)
	{
		for (int j = 0; j < mainOwnershipFunction->clasterCount * sizeModel; j++)
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

double DOptimization::isOptimal(Point x, Plan plan)
{
	OwnershipFunction tempOwnFunc(5, 2, 0.1, plan.remarkCount, x.size);
	tempOwnFunc.calcFCMWithoutCenter(plan.plan);
	vector<vector<double>> matrixFisherInX = mainLocalModel->calcLinearFisherMatrixInX(tempOwnFunc, x);
	vector<vector<double>> matrixFisher = mainLocalModel->invertMatrix(mainLocalModel->getFisherMatrix());
	vector<double> multipleMatrix;
	int sizeModel = 2;
	multipleMatrix.resize(mainOwnershipFunction->clasterCount * sizeModel);
	double sum = 0;
	for (int i = 0; i < mainOwnershipFunction->clasterCount * sizeModel; i++)
	{
		for (int j = 0; j < mainOwnershipFunction->clasterCount * sizeModel; j++)
		{
			multipleMatrix[i] += matrixFisher[i][j] * matrixFisherInX[j][i];
		}
		sum += multipleMatrix[i];
	}
	return sum;
}

bool DOptimization::optimize—ontinuousPlan()
{
	Point maxX(optimal->plan[0].coord.size());
	double maxTrace = 0;

	mainOwnershipFunction = new OwnershipFunction(5, 2, 0.1, optimal->remarkCount, optimal->plan[0].coord.size());
	mainOwnershipFunction->calcFCMWithoutCenter(optimal->plan);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, *optimal);
	
	for (double x = beginPoint; x <= endPoint; x += step)
	{
		for (double y = beginPoint; y <= endPoint; y += step)
		{
			vector<double> coord(optimal->plan[0].coord.size());
			coord[0] = x;
			coord[1] = y;
			Point newPoint(optimal->plan[0].coord.size(), coord);

			double trace = isOptimal(newPoint);

			if (abs(maxTrace - mainLocalModel->getFisherMatrix().size()) > abs(trace - mainLocalModel->getFisherMatrix().size()))
			{
				maxTrace = trace;
				maxX = newPoint;
			}
		}
		
	}

	if (abs(maxTrace - mainLocalModel->getFisherMatrix().size()) > 0.001)
	{
		Point newX = calcX();
		newX.weight = 0.5;
		optimal->enlarge(newX);
		optimal->clean();

		double newTrace = isOptimal(maxX, *optimal);

		cout << abs(maxTrace - mainLocalModel->getFisherMatrix().size()) << " " << abs(newTrace - mainLocalModel->getFisherMatrix().size()) << endl;

		if (abs(newTrace - mainLocalModel->getFisherMatrix().size()) < 0.001)
		{
			return true;
		}
		else return false;

	} 
	else
	{
		Point newX = calcX();
		return true;
	}
}

Point DOptimization::calcX()
{
	Point result(optimal->remarkCount);
	vector<vector<double>> value;
	vector<vector<double>> matrixFisher = mainLocalModel->invertMatrix(mainLocalModel->getFisherMatrix());
	for (double x = beginPoint; x <= endPoint; x += step)
	{
		for (double y = beginPoint; y <= endPoint; y += step)
		{
			vector<vector<double>> newValue;
			vector<double> coord(optimal->plan[0].coord.size());
			coord[0] = x;
			coord[1] = y;
			Point newPoint(optimal->plan[0].coord.size(), coord);

			vector<double> fcm = mainOwnershipFunction->FCMLineInX(newPoint);
			vector<vector<double>> f(fcm.size() * (optimal->plan[0].coord.size() + 1));
			vector<vector<double>> ft(1);
			ft[0].resize(fcm.size() * (optimal->plan[0].coord.size() + 1));

			for (int i = 0; i < f.size(); i++)
			{
				f[i].resize(1);
			}

			for (int i = 0; i < fcm.size(); i++)
			{
				f[i * (optimal->plan[0].coord.size() + 1)][0] = fcm[i];
				f[i * (optimal->plan[0].coord.size() + 1) + 1][0] = fcm[i] * newPoint.coord[0];
				f[i * (optimal->plan[0].coord.size() + 1) + 2][0] = fcm[i] * newPoint.coord[1];

				
				ft[0][i * (optimal->plan[0].coord.size() + 1)] = fcm[i];
				ft[0][i * (optimal->plan[0].coord.size() + 1) + 1] = fcm[i] * newPoint.coord[0];
				ft[0][i * (optimal->plan[0].coord.size() + 1) + 2] = fcm[i] * newPoint.coord[1];
			}

			newValue = multMatrix(multMatrix(ft, matrixFisher), f);

			if (value.size() == 0 || value[0][0] < newValue[0][0])
			{
				value = newValue;
				result = newPoint;
			}
		}
		
	}
	ofstream out("fi.txt");
	out << value[0][0] << endl;

	return result;
}


vector<Point> DOptimization::optimizeDiscretePlan()
{
	Point maxX(optimal->plan[0].coord.size());
	double maxTrace = 0;
	vector<Point> arrayX = optimal->plan;

	mainOwnershipFunction = new OwnershipFunction(5, 2, 0.1, optimal->remarkCount, optimal->plan[0].coord.size());
	mainOwnershipFunction->calcFCMWithoutCenter(arrayX);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, *optimal);

	for (double x = beginPoint; x <= endPoint; x += step)
	{
		for (double y = beginPoint; y <= endPoint; y += step)
		{
			vector<double> coord(optimal->plan[0].coord.size());
			coord[0] = x;
			coord[1] = y;
			Point newPoint(optimal->plan[0].coord.size(), coord);

			if (find(arrayX.begin(), arrayX.end(), newPoint) == arrayX.end())
			{
				double trace = isOptimal(newPoint);

				if (maxTrace < trace)
				{
					maxTrace = trace;
					maxX = newPoint;
				}
			}		
		}		
	}

	optimal->enlargeDiscrete(maxX);

	Point minX(optimal->plan[0].coord.size());
	double minTrace = 0;

	mainOwnershipFunction = new OwnershipFunction(5, 2, 0.1, optimal->remarkCount, optimal->plan[0].coord.size());
	mainOwnershipFunction->calcFCMWithoutCenter(optimal->plan);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, *optimal);

	for (int i = 0; i < optimal->plan.size(); i++)
	{
		double trace = isOptimal(optimal->plan[i]);

		if (minTrace > trace || minTrace == 0)
		{
			minTrace = trace;
			minX = optimal->plan[i];
		}
	}

	optimal->reduceDiscrete(minX);
	mainOwnershipFunction = new OwnershipFunction(5, 2, 0.1, optimal->remarkCount, optimal->plan[0].coord.size());
	mainOwnershipFunction->calcFCMWithoutCenter(optimal->plan);
	mainLocalModel->calcFisherMatrix(*mainOwnershipFunction, *optimal);

	cout << maxTrace << " " << minTrace << endl;

	vector<Point> res(2);
	res[0] = minX;
	res[1] = maxX;
	res[0].weight = minTrace;
	res[1].weight = maxTrace;

	return res;
}

Plan DOptimization::calcOptimalPlan()
{
	/*vector<Point> result(2);
	result = optimizeDiscretePlan();
	for (int i = 0; i > -1; i = i)
		if (result[0] == result[1] || abs(result[0].weight - result[1].weight) < 0.00001)
			break;
		else
			result = optimizeDiscretePlan();
	return *optimal;*/
	bool result = optimize—ontinuousPlan();
	for (int i = 0; i > -1; i++)
	{
		if (result)
			break;
		else
			result = optimize—ontinuousPlan();
	}
	return *optimal;
}


vector<vector<double>> DOptimization::getMNK(int i)
{
	vector<vector<double>> result;
	result = multMatrix(optimal->getTransponLocalModelMatrix(), mainOwnershipFunction->getDiagOwnershipMatrix(i));
	result = multMatrix(result, optimal->getLocalModelMatrix());
	result = mainLocalModel->invertMatrix(result);
	result = multMatrix(result, optimal->getTransponLocalModelMatrix());
	result = multMatrix(result, mainOwnershipFunction->getDiagOwnershipMatrix(i));
	result = multMatrix(result, mainLocalModel->getLocalMatrix(*optimal));

	return result;
}

double DOptimization::getRespondValue(int i)
{
	double sum = 0;
	for (int j = 0; j < mainOwnershipFunction->clasterCount; j++)
	{
		sum += mainLocalModel->getY((*optimal)[0][i]) * (*mainOwnershipFunction)[i][j];
	}

	return sum + sum * (int(pow(-1.0, rand() % 2) * rand()) % 15) / 100;
}

vector<vector<double>> DOptimization::getFisherMatrix()
{
	return mainLocalModel->getFisherMatrix();
}
