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

Plan::Plan()
{
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

			for (int j = 1; j < numbersForClean.size(); j++)
			{
				plan[0].erase(plan[0].begin() + j);
				plan[1].erase(plan[1].begin() + j);
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