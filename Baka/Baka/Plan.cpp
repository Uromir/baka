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