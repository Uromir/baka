﻿#include "LocalModel.h"

/* вычисление матрицы фишера одной итерации по линейной модели */
vector<vector<double>> LocalModel::linearFisher(vector<double> FCMLine, double x, double weight)
{
	vector<vector<double>> currentFisherMatrix;
	currentFisherMatrix.resize(FCMLine.size() * 2);
	for (int i = 0; i < FCMLine.size(); i++)
	{
		currentFisherMatrix[i * 2].resize(FCMLine.size() * 2);
		currentFisherMatrix[i * 2 + 1].resize(FCMLine.size() * 2);
		for (int j = 0; j < FCMLine.size(); j++)
		{
			currentFisherMatrix[i * 2][j * 2] = FCMLine[i] * FCMLine[j] * weight;
			currentFisherMatrix[i * 2 + 1][j * 2] = FCMLine[i] * FCMLine[j] * x * weight;
			
			currentFisherMatrix[i * 2][j * 2 + 1] = FCMLine[i] * FCMLine[j] * x * weight;
			currentFisherMatrix[i * 2 + 1][j * 2 + 1] = FCMLine[i] * FCMLine[j] * x * x * weight;
		}
	}
	return currentFisherMatrix;
}

/* вычисление матрицы фишера одной итерации по квадратичной модели */
vector<vector<double>> LocalModel::squareFisher(vector<double> FCMLine, double x, double weight)
{
	vector<vector<double>> currentFisherMatrix;
	currentFisherMatrix.resize(FCMLine.size() * 3);
	for (int i = 0; i < FCMLine.size(); i++)
	{
		currentFisherMatrix[i * 3].resize(FCMLine.size() * 3);
		currentFisherMatrix[i * 3 + 1].resize(FCMLine.size() * 3);
		currentFisherMatrix[i * 3 + 2].resize(FCMLine.size() * 3);
		for (int j = 0; j < FCMLine.size(); j++)
		{
			currentFisherMatrix[i * 3][j * 3] = FCMLine[i] * FCMLine[j] * weight;
			currentFisherMatrix[i * 3 + 1][j * 3] = FCMLine[i] * FCMLine[j] * x * weight;
			currentFisherMatrix[i * 3 + 2][j * 3] = FCMLine[i] * FCMLine[j] * x * x * weight;

			currentFisherMatrix[i * 3][j * 3 + 1] = FCMLine[i] * FCMLine[j] * x * weight;
			currentFisherMatrix[i * 3 + 1][j * 3 + 1] = FCMLine[i] * FCMLine[j] * x * x * weight;
			currentFisherMatrix[i * 3 + 2][j * 3 + 1] = FCMLine[i] * FCMLine[j] * x * x * x * weight;

			currentFisherMatrix[i * 3][j * 3 + 2] = FCMLine[i] * FCMLine[j] * x * x * weight;
			currentFisherMatrix[i * 3 + 1][j * 3 + 2] = FCMLine[i] * FCMLine[j] * x * x * x * weight;
			currentFisherMatrix[i * 3 + 2][j * 3 + 2] = FCMLine[i] * FCMLine[j] * x * x * x * x * weight;
		}
	}
	return currentFisherMatrix;
}

/* добавить к существующей матрице фишера новое слагаемое */
void LocalModel::sumFromFisher(vector<vector<double>> additingMatrix)
{
	for (int i = 0; i < fisherMatrix.size(); i++)
	{
		for (int j = 0; j < fisherMatrix[i].size(); j++)
		{
			fisherMatrix[i][j] += additingMatrix[i][j];
		}
	}
}

vector<vector<double>> LocalModel::invertMatrix(vector<vector<double>> matrix)
{
	double temp;
	vector<vector<double>> identity;
	identity.resize(matrix.size());
	for (int i = 0; i < matrix.size(); i++)
	{
		identity[i].resize(matrix.size());
		identity[i][i] = 1;
	}

	for (int k = 0; k < matrix.size(); k++)
	{
		temp = matrix[k][k];

		for (int j = 0; j < matrix.size(); j++)
		{
			matrix[k][j] /= temp;
			identity[k][j] /= temp;
		}

		for (int i = k + 1; i < matrix.size(); i++)
		{
			temp = matrix[i][k];

			for (int j = 0; j < matrix.size(); j++)
			{
				matrix[i][j] -= matrix[k][j] * temp;
				identity[i][j] -= identity[k][j] * temp;
			}
		}
	}

	for (int k = matrix.size() - 1; k > 0; k--)
	{
		for (int i = k - 1; i >= 0; i--)
		{
			temp = matrix[i][k];

			for (int j = 0; j < matrix.size(); j++)
			{
				matrix[i][j] -= matrix[k][j] * temp;
				identity[i][j] -= identity[k][j] * temp;
			}
		}
	}

	for (int i = 0; i < matrix.size(); i++)
		for (int j = 0; j < matrix.size(); j++)
			matrix[i][j] = identity[i][j];

	return matrix;
}

/* вычислить матрицу фишера */
vector<vector<double>> LocalModel::calcFisherMatrix(OwnershipFunction FCMfunction, Plan mainPlan)
{
	int sizeModel = 2;
	fisherMatrix.resize(sizeModel * FCMfunction.clasterCount);
	for (int i = 0; i < sizeModel * FCMfunction.clasterCount; i++)
	{
		fisherMatrix[i].resize(sizeModel * FCMfunction.clasterCount);
	}
	for (int i = 0; i < FCMfunction.elementCount; i++)
	{
		sumFromFisher(linearFisher(FCMfunction[i], mainPlan[0][i], mainPlan[1][i]));
	}
	return fisherMatrix;
}

vector<vector<double>> LocalModel::calcLinearFisherMatrixInX(OwnershipFunction FCMFunction, double x)
{
	vector<double> newOwnershipValue = FCMFunction.FCMLineInX(x);
	vector<vector<double>> matrix;

	matrix.resize(newOwnershipValue.size() * 2);

	for (int i = 0; i < newOwnershipValue.size(); i++)
	{
		matrix[i * 2].resize(newOwnershipValue.size() * 2);
		matrix[i * 2 + 1].resize(newOwnershipValue.size() * 2);
		for (int j = 0; j < newOwnershipValue.size(); j++)
		{
			matrix[i * 2][j * 2] = newOwnershipValue[i] * newOwnershipValue[j];
			matrix[i * 2 + 1][j * 2] = newOwnershipValue[i] * newOwnershipValue[j] * x;

			matrix[i * 2][j * 2 + 1] = newOwnershipValue[i] * newOwnershipValue[j] * x;
			matrix[i * 2 + 1][j * 2 + 1] = newOwnershipValue[i] * newOwnershipValue[j] * x * x;
		}
	}

	return matrix;
}

vector<vector<double>> LocalModel::calcSquareFisherMatrixInX(OwnershipFunction FCMFunction, double x)
{
	vector<double> newOwnershipValue = FCMFunction.FCMLineInX(x);
	vector<vector<double>> matrix;

	matrix.resize(newOwnershipValue.size() * 3);

	for (int i = 0; i < newOwnershipValue.size(); i++)
	{
		matrix[i * 3].resize(newOwnershipValue.size() * 3);
		matrix[i * 3 + 1].resize(newOwnershipValue.size() * 3);
		matrix[i * 3 + 2].resize(newOwnershipValue.size() * 3);
		for (int j = 0; j < newOwnershipValue.size(); j++)
		{
			matrix[i * 3][j * 3] = newOwnershipValue[i] * newOwnershipValue[j];
			matrix[i * 3 + 1][j * 3] = newOwnershipValue[i] * newOwnershipValue[j] * x;
			matrix[i * 3 + 2][j * 3] = newOwnershipValue[i] * newOwnershipValue[j] * x * x;

			matrix[i * 3][j * 3 + 1] = newOwnershipValue[i] * newOwnershipValue[j] * x;
			matrix[i * 3 + 1][j * 3 + 1] = newOwnershipValue[i] * newOwnershipValue[j] * x * x;
			matrix[i * 3 + 2][j * 3 + 1] = newOwnershipValue[i] * newOwnershipValue[j] * x * x * x;

			matrix[i * 3][j * 3 + 2] = newOwnershipValue[i] * newOwnershipValue[j] * x * x;
			matrix[i * 3 + 1][j * 3 + 2] = newOwnershipValue[i] * newOwnershipValue[j] * x * x * x;
			matrix[i * 3 + 2][j * 3 + 2] = newOwnershipValue[i] * newOwnershipValue[j] * x * x * x * x;
		}
	}

	return matrix;
}

vector<vector<double>> LocalModel::getFisherMatrix()
{
	return fisherMatrix;
}

vector<vector<double>> LocalModel::getLocalMatrix()
{
	vector<vector<double>> y;

	y.resize(2);
	
	y[0].resize(1);
	y[0][0] = 1;
	
	y[1].resize(1);
	y[1][0] = 0.2;

	return y;
}

double LocalModel::getY(double x)
{
	return 3 + 0.2 * x;
}

LocalModel::LocalModel()
{
}


LocalModel::~LocalModel()
{
}