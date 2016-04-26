﻿#include "LocalModel.h"

/* вычисление матрицы фишера одной итерации по линейной модели */
vector<vector<double>> LocalModel::linearFisher(vector<double> FCMLine, double x, double weight)
{
	vector<vector<double>> currentFisherMatrix;
	for (int i = 0; i < FCMLine.size(); i++)
	{
		for (int j = 0; j < FCMLine.size(); j++)
		{
			currentFisherMatrix[i][j * 2] = FCMLine[i] * FCMLine[j] * weight;
			currentFisherMatrix[i + 1][j * 2] = FCMLine[i] * FCMLine[j] * x * weight;
			
			currentFisherMatrix[i][j * 2 + 1] = FCMLine[i] * FCMLine[j] * x * weight;
			currentFisherMatrix[i + 1][j * 2 + 1] = FCMLine[i] * FCMLine[j] * x * x * weight;
		}
	}
	return fisherMatrix;
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
	fisherMatrix.resize(2 * FCMfunction.clasterCount);
	for (int i = 0; i < 2 * FCMfunction.clasterCount; i++)
	{
		fisherMatrix[i].resize(2 * FCMfunction.clasterCount);
	}
	for (int i = 0; i < FCMfunction.elementCount; i++)
	{
		sumFromFisher(linearFisher(FCMfunction[i], mainPlan[0][i], mainPlan[1][i]));
	}
	return fisherMatrix;
}

vector<vector<double>> LocalModel::calcFisherMatrixInX(OwnershipFunction FCMFunction, double x)
{
	vector<double> newOwnershipValue = FCMFunction.FCMLineInX(x);
	vector<vector<double>> matrix;

	double weight = 1; // какой-то вес, его надо понять откуда брать

	for (int i = 0; i < newOwnershipValue.size(); i++)
	{
		for (int j = 0; j < newOwnershipValue.size(); j++)
		{
			matrix[i][j * 2] = newOwnershipValue[i] * newOwnershipValue[j] * weight;
			matrix[i + 1][j * 2] = newOwnershipValue[i] * newOwnershipValue[j] * x * weight;

			matrix[i][j * 2 + 1] = newOwnershipValue[i] * newOwnershipValue[j] * x * weight;
			matrix[i + 1][j * 2 + 1] = newOwnershipValue[i] * newOwnershipValue[j] * x * x * weight;
		}
	}
}

LocalModel::LocalModel()
{
}


LocalModel::~LocalModel()
{
}