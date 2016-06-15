#include "Plan.h"

/* считать начальный план из файла */
void Plan::readPlan()
{
	ifstream in("plan.txt");
	double x = 0;
	for (int i = 0; i < remarkCount; i++)
	{
		Point newPoint = Point(dimensionSpace);
		for (int j = 0; j < dimensionSpace; j++)
		{
			in >> x;
			newPoint.coord[j] = x;
		}
		in >> newPoint.weight;
		plan[i] = newPoint;
	}
}

Plan::Plan(int remarkCount, int dimensionSpace)
{
	this->remarkCount = remarkCount;
	this->dimensionSpace = dimensionSpace;
	plan.resize(remarkCount);
	readPlan();
}


Plan::~Plan()
{
}

Point Plan::operator[](int i)
{
	return plan[i];
}

void Plan::enlarge(Point x)
{
	for (int i = 0; i < remarkCount; i++)
	{
		plan[i].weight /= 2;
	}
	remarkCount++;
	x.weight = 0.5;
	plan.push_back(x);
}

void Plan::enlargeDiscrete(Point x)
{
	remarkCount++;
	x.weight = 1;
	plan.push_back(x);
	for (int i = 0; i < remarkCount; i++)
	{
		plan[i].weight = 1.0 / remarkCount;
	}
}

void Plan::reduce(Point x)
{
	remarkCount--;
	for (int i = 0; i < remarkCount; i++)
	{
		if (x == plan[i])
		{
			plan.erase(plan.begin() + i);
			break;
		}
	}
}

void Plan::reduceDiscrete(Point x)
{
	remarkCount--;
	for (int i = 0; i < remarkCount + 1; i++)
	{
		plan[i].weight = 1.0 / remarkCount;
	}
	for (int i = 0; i < remarkCount + 1; i++)
	{
		if (x == plan[i])
		{
			plan.erase(plan.begin() + i);
			break;
		}
	}
}

void Plan::clean()
{
	vector<int> processedPoint;
	for (int i = 0; i < remarkCount; i++)
	{
		vector<int> numbersForClean;
		numbersForClean.push_back(i);
		for (int j = i + 1; j < remarkCount; j++)
		{
			if ((plan[i] - plan[j]).Norm() < 0.01)
			{
				numbersForClean.push_back(j);
			}
		}
		if (numbersForClean.size() > 1)
		{
			processedPoint.push_back(i);
			double sumP = 0;
			Point sumX(dimensionSpace);
			for (int j = 0; j < numbersForClean.size(); j++)
			{
				sumP += plan[j].weight;
				sumX = sumX + plan[j] * plan[j].weight;
			}

			plan[i].weight = sumP;
			plan[i] = sumX * (1 / sumP);

			for (int j = numbersForClean.size(); j > 0 ; j--)
			{
				plan.erase(plan.begin() + numbersForClean[j - 1]);
				remarkCount--;
			}
		}
		else if (plan[i].weight < 0.0001)
		{
			plan.erase(plan.begin() + i);
			remarkCount--;
		}
	}
}

vector<vector<double>> Plan::getLocalModelMatrix()
{
	vector<vector<double>> matrix;
	matrix.resize(remarkCount);
	for (int i = 0; i < remarkCount; i++)
	{
		matrix[i].resize(dimensionSpace + 1);

		matrix[i][0] = 1;
		for (int j = 0; j < dimensionSpace; j++)
			matrix[i][j + 1] = plan[i][j];
	}

	return matrix;
}

vector<vector<double>> Plan::getTransponLocalModelMatrix()
{
	vector<vector<double>> matrix;
	matrix.resize(dimensionSpace + 1);

	for (int i = 0; i < matrix.size(); i++)
	{
		matrix[i].resize(remarkCount);
	}
	for (int i = 0; i < remarkCount; i++)
	{
		matrix[0][i] = 1;
		for (int j = 0; j < dimensionSpace; j++)
			matrix[j + 1][i] = plan[i][j];
	}

	return matrix;
}

void Plan::createRandomPlan(int elementCount)
{
	remarkCount = elementCount;
	plan.resize(remarkCount);
	for (int i = 0; i < remarkCount; i++)
	{
		for (int j = 0; j < dimensionSpace; j++)
			plan[i].coord[j] = pow(-1.0, rand() % 2 + 1) * (double(rand() % 500) / 500);
	}
}