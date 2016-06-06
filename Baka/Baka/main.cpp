#include "DOptimization.h"

int main()
{
	Plan mainPlan(25);
	ofstream out("norm.txt");

	ofstream out2("fish.txt");
	out2 << "начальный план" << endl;
	for (int i = 0; i < mainPlan.remarkCount; i++)
	{
		out2 << mainPlan[0][i] << " "; 
	}
	out2 << endl;
	out2 << "веса начального план" << endl;
	for (int i = 0; i < mainPlan.remarkCount; i++)
	{
		out2 << mainPlan[1][i] << " "; 
	}
	out2 << endl << endl;

	DOptimization mainOptimizationAlgoritm(&mainPlan, -0.9, 0.9, 0.01);
	mainOptimizationAlgoritm.optimizeDiscretePlan();
	vector<vector<double>> test = mainOptimizationAlgoritm.getMNK(0);
	vector<vector<double>> test2 = mainOptimizationAlgoritm.getMNK(1);
	vector<vector<double>> test3 = mainOptimizationAlgoritm.getMNK(2);

	double norm1 = 0;
	double norm2 = 0;
	double norm3 = 0;

	norm1 = sqrt(pow(test[0][0] - 1, 2) + pow(test[1][0] - 0.2, 2));
	norm2 = sqrt(pow(test2[0][0] - 1, 2) + pow(test2[1][0] - 0.2, 2));
	norm3 = sqrt(pow(test3[0][0] - 1, 2) + pow(test3[1][0] - 0.2, 2));

	out << "нормы" << endl;
	out << norm1 << "           " << norm2 << "            " << norm3 << endl;

	out2 << "оптимальный план" << endl;
	for (int i = 0; i < mainPlan.remarkCount; i++)
	{
		out2 << mainPlan[0][i] << " "; 
	}
	out2 << endl;
	out2 << "веса оптимального плана" << endl;
	for (int i = 0; i < mainPlan.remarkCount; i++)
	{
		out2 << mainPlan[1][i] << " "; 
	}
	out2 << endl;
	out2 << "отклик" << endl;
	for (int i = 0; i < mainPlan.remarkCount; i++)
	{
		out2 << mainOptimizationAlgoritm.getRespondValue(mainPlan[0][i]) << " "; 
	}
	out2 << endl;
	
	double num = 0;

	for (int i = 0; i < 25; i++)
	{
		if (i % 2 == 0)
			num = double(rand() % 100) / 100;
		else
			num = -double(rand() % 100) / 100;
		out << num << " ";
	}
	/*ofstream out("fish.txt");
	vector<double> arrayX = mainPlan[0];
	OwnershipFunction own(3, 2, 0.1, mainPlan.remarkCount);
	own.calcFCMWithoutCenter(arrayX);
	mainLocalModel.calcFisherMatrix(own, mainPlan);
	vector<vector<double>> matrix = mainLocalModel.getFisherMatrix();

	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			out << matrix[i][j] << " ";
		}
		out << endl;
	}*/

	return 0;
}