#include <vector>
#include "OwnershipFunction.h"
#include "Plan.h"

using namespace std;

#pragma once
class LocalModel
{
private:
	vector<vector<double>> linearFisher(vector<double> FCMLine, double x, double weight); // вычисление матрицы фишера одной итерации по линейной модели
	vector<vector<double>> fisherMatrix; // матрица фишера
	void sumFromFisher(vector<vector<double>> additingMatrix); // добавить к существующей матрице фишера новое слагаемое
public:
	vector<vector<double>> calcFisherMatrix(OwnershipFunction FCMfunction, Plan mainPlan); // вычислить матрицу фишера
	vector<vector<double>> calcFisherMatrixInX(OwnershipFunction FCMFunction, double x);
	vector<vector<double>> invertMatrix(vector<vector<double>> matrix);
	LocalModel();
	~LocalModel();
};

