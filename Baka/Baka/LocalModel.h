#include <vector>
#include "OwnershipFunction.h"
#include "Plan.h"

using namespace std;

#pragma once
class LocalModel
{
private:
	vector<vector<double>> linearFisher(vector<double> FCMLine, Point x, double weight); // вычисление матрицы фишера одной итерации по линейной модели
	vector<vector<double>> squareFisher(vector<double> FCMLine, Point x, double weight); // вычисление матрицы фишера одной итерации по квадратичной модели
	vector<vector<double>> fisherMatrix; // матрица фишера
	void sumFromFisher(vector<vector<double>> additingMatrix); // добавить к существующей матрице фишера новое слагаемое
public:
	vector<vector<double>> calcFisherMatrix(OwnershipFunction FCMfunction, Plan mainPlan); // вычислить матрицу фишера
	vector<vector<double>> calcLinearFisherMatrixInX(OwnershipFunction FCMFunction, Point x);
	vector<vector<double>> calcSquareFisherMatrixInX(OwnershipFunction FCMFunction, Point x);
	vector<vector<double>> invertMatrix(vector<vector<double>> matrix);
	vector<vector<double>> getFisherMatrix();
	vector<vector<double>> getLocalMatrix();
	double getY(Point x);
	LocalModel();
	~LocalModel();
};

