#include "LocalModel.h"

/* вычисление матрицы фишера одной итерации по линейной модели */
vector<vector<double>> LocalModel::linearFisher(vector<double> FCMLine, Point x, double weight)
{
	vector<vector<double>> currentFisherMatrix;
	int size = x.coord.size() + 1;
	currentFisherMatrix.resize(FCMLine.size() * size);
	for (int i = 0; i < FCMLine.size(); i++)
	{
		for (int j = 0; j < size; j++)
			currentFisherMatrix[i * size + j].resize(FCMLine.size() * size);
		for (int j = 0; j < FCMLine.size(); j++)
		{
			for (int k = 0; k < size; k++)
			{
				for (int l = 0; l < size; l++)
				{
					double mult = 1;

					if (k != 0)
						mult *= x.coord[k - 1];

					if (l != 0)
						mult *= x.coord[l - 1];

					currentFisherMatrix[i * size + l][j * size + k] = FCMLine[i] * FCMLine[j] * mult * weight;
				}
			}
		}
	}
	return currentFisherMatrix;
}

/* вычисление матрицы фишера одной итерации по квадратичной модели */
vector<vector<double>> LocalModel::squareFisher(vector<double> FCMLine, Point x, double weight)
{
	vector<vector<double>> currentFisherMatrix;
	int size = x.coord.size() * 2 + 1;
	currentFisherMatrix.resize(FCMLine.size() * size);
	for (int i = 0; i < FCMLine.size(); i++)
	{
		for (int j = 0; j < size; j++)
			currentFisherMatrix[i * size + j].resize(FCMLine.size() * size);
		for (int j = 0; j < FCMLine.size(); j++)
		{
			for (int k = 0; k < size; k++)
			{
				for (int l = 0; l < size; l++)
				{
					double mult = 1;

					if (k != 0)
					{
						double num = k % x.coord.size() || x.coord.size();
						mult *= pow(x.coord[num - 1], k / x.coord.size() + 1);
					}

					if (l != 0)
					{
						double num = l % x.coord.size() || x.coord.size();
						mult *= pow(x.coord[num - 1], k / x.coord.size() + 1);
					}

					currentFisherMatrix[i * size][j * size] = FCMLine[i] * FCMLine[j] * mult * weight;
				}
			}
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
	int sizeModel = mainPlan.plan[0].coord.size() + 1;
	fisherMatrix.resize(sizeModel * FCMfunction.clasterCount);
	for (int i = 0; i < sizeModel * FCMfunction.clasterCount; i++)
	{
		fisherMatrix[i].resize(sizeModel * FCMfunction.clasterCount);
	}
	for (int i = 0; i < FCMfunction.elementCount; i++)
	{
		sumFromFisher(linearFisher(FCMfunction[i], mainPlan[i], mainPlan[i].weight));
	}
	return fisherMatrix;
}

vector<vector<double>> LocalModel::calcLinearFisherMatrixInX(OwnershipFunction FCMFunction, Point x)
{
	vector<double> newOwnershipValue = FCMFunction.FCMLineInX(x);
	vector<vector<double>> matrix;
	int size = x.coord.size() + 1;
	matrix.resize(newOwnershipValue.size() * size);
	for (int i = 0; i < newOwnershipValue.size(); i++)
	{
		for (int j = 0; j < size; j++)
			matrix[i * size + j].resize(newOwnershipValue.size() * size);
		for (int j = 0; j < newOwnershipValue.size(); j++)
		{
			for (int k = 0; k < size; k++)
			{
				for (int l = 0; l < size; l++)
				{
					double mult = 1;

					if (k != 0)
						mult *= x.coord[k - 1];

					if (l != 0)
						mult *= x.coord[l - 1];

					matrix[i * size + l][j * size + k] = newOwnershipValue[i] * newOwnershipValue[j] * mult;
				}
			}
		}
	}

	return matrix;
}

vector<vector<double>> LocalModel::calcSquareFisherMatrixInX(OwnershipFunction FCMFunction, Point x)
{
	vector<double> newOwnershipValue = FCMFunction.FCMLineInX(x);
	vector<vector<double>> matrix;

	int size = x.coord.size() * 2 + 1;
	matrix.resize(newOwnershipValue.size() * size);

	for (int i = 0; i < newOwnershipValue.size(); i++)
	{
		for (int j = 0; j < size; j++)
			matrix[i * size + j].resize(newOwnershipValue.size() * size);
		for (int j = 0; j < newOwnershipValue.size(); j++)
		{
			for (int k = 0; k < size; k++)
			{
				for (int l = 0; l < size; l++)
				{
					double mult = 1;

					if (k != 0)
					{
						double num = k % x.coord.size() || x.coord.size();
						mult *= pow(x.coord[num - 1], k / x.coord.size() + 1);
					}

					if (l != 0)
					{
						double num = l % x.coord.size() || x.coord.size();
						mult *= pow(x.coord[num - 1], k / x.coord.size() + 1);
					}

					matrix[i * size][j * size] = newOwnershipValue[i] * newOwnershipValue[j] * mult;
				}
			}
		}
	}

	return matrix;
}

vector<vector<double>> LocalModel::getFisherMatrix()
{
	return fisherMatrix;
}

vector<vector<double>> LocalModel::getLocalMatrix(Plan plan)
{
	vector<vector<double>> y;

	y.resize(plan.remarkCount);

	for (int i = 0; i < plan.remarkCount; i++)
	{
		y[i].resize(1);
		y[i][0] = getY(plan[i]);
		y[i][0] += y[i][0] * (int(pow(-1.0, rand() % 2) * rand()) % 15) / 100;
	}

	return y;
}

double LocalModel::getY(Point x)
{
	if (x[0] <= -0.3)
		return 3 + 0.2 * x[0];

	if (x[0] > -0.3 && x[0] <= 0.3)
		return 1 - 4 * x[0];

	if (x[0] > 0.3)
		return 0.6 + 0.5 * x[0];
}

LocalModel::LocalModel()
{
}


LocalModel::~LocalModel()
{
}