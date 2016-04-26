#include "OwnershipFunction.h"

int main()
{
	OwnershipFunction mainOwnershipFunction(4, 2, 0.1, 5);
	vector<double> x;
	x.resize(5);
	x[0] = 1;
	x[1] = 2;
	x[2] = 3;
	x[3] = 0.5;
	x[4] = -1;
	mainOwnershipFunction.calcFCM(x);

	int a = mainOwnershipFunction[0][0];
	return 0;
}