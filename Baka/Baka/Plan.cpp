#include "Plan.h"

/* считать начальный план из файла */
void Plan::readPlan()
{
	ifstream in("plan.txt");
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < remarkCount; j++)
		{
			in >> plan[i][j];
		}
	}
}

Plan::Plan(int remarkCount)
{
	this->remarkCount = remarkCount;
	plan.resize(2);
	plan[0].resize(remarkCount);
	plan[1].resize(remarkCount);
	readPlan();
}


Plan::~Plan()
{
}

vector<double> Plan::operator[](int i)
{
	return plan[i];
}

void Plan::enlarge(double x)
{
	for (int i = 0; i < remarkCount; i++)
	{
		plan[1][i] /= 2;
	}
	remarkCount++;
	plan[0].push_back(x);
	plan[1].push_back(0.5);
}

void Plan::enlargeDiscrete(double x)
{
	remarkCount++;
	plan[0].push_back(x);
	plan[1].push_back(1);
	for (int i = 0; i < remarkCount; i++)
	{
		plan[1][i] = 1.0 / remarkCount;
	}
}

void Plan::reduce(double x)
{
	remarkCount--;
	for (int i = 0; i < remarkCount; i++)
	{
		if (x == plan[0][i])
		{
			plan[0].erase(plan[0].begin() + i);
			plan[1].erase(plan[1].begin() + i);
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
			if (abs(plan[0][i] - plan[0][j]) < 0.01)
			{
				numbersForClean.push_back(j);
			}
		}
		if (numbersForClean.size() > 1)
		{
			processedPoint.push_back(i);
			double sumP = 0;
			double sumX = 0;
			for (int j = 0; j < numbersForClean.size(); j++)
			{
				sumP += plan[1][j];
				sumX += plan[0][j] * plan[1][j];
			}

			plan[1][i] = sumP;
			plan[0][i] = (1 / sumP) * sumX;

			for (int j = numbersForClean.size(); j > 0 ; j--)
			{
				plan[0].erase(plan[0].begin() + numbersForClean[j - 1]);
				plan[1].erase(plan[1].begin() + numbersForClean[j - 1]);
				remarkCount--;
			}
		}
		else if (plan[1][i] < 0.01)
		{
			plan[0].erase(plan[0].begin() + i);
			plan[1].erase(plan[1].begin() + i);
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
		matrix[i].resize(2);

		matrix[i][0] = 1;
		matrix[i][1] = plan[0][i];
	}

	return matrix;
}

vector<vector<double>> Plan::getTransponLocalModelMatrix()
{
	vector<vector<double>> matrix;
	matrix.resize(2);

	matrix[0].resize(remarkCount);
	matrix[1].resize(remarkCount);
	for (int i = 0; i < remarkCount; i++)
	{
		matrix[0][i] = 1;
		matrix[1][i] = plan[0][i];
	}

	return matrix;
}

void Plan::createRandomPlan(int elementCount)
{
	remarkCount = elementCount;
	plan[0].resize(remarkCount);
	plan[1].resize(remarkCount);
	for (int i = 0; i < remarkCount; i++)
	{
		plan[0][i] = pow(-1.0, rand() % 2 + 1) * (double(rand() % 500) / 500);
	}
}