#include "OwnershipFunction.h"

/* посчитать сумму учавствующую в алгоритме FCM, с участием начальной матрицы принадлежности и иксов */
double OwnershipFunction::summWithBeginOwnershipMatrix(vector<Point> x, vector<vector<double>> ownershipMatrix,int i)
{
	double sum = 0;
	for (int j = 0; j < elementCount; j++)
	{
		sum += pow(ownershipMatrix[j][i], exponentialWeight) * x[j].Norm();
	}
	return sum;
}

/* посчитать сумму учавствующую в алгоритме FCM, с участием начальной матрицы принадлежности */
double OwnershipFunction::summWithBeginOwnershipMatrix(vector<vector<double>> ownershipMatrix, int i)
{
	double sum = 0;
	for (int j = 0; j < elementCount; j++)
	{
		sum += pow(ownershipMatrix[j][i], exponentialWeight);
	}
	return sum;
}

/* посчитать сумму учавствующую в алгоритме FCM, с участием расстояний между иксами и центрами кластеров */
double OwnershipFunction::summWithDistanceFromCenter(vector<double> distanceFromCenter, int j)
{
	double sum = 0;
	for (int k = 0; k < clasterCount; k++)
	{
		sum += pow(distanceFromCenter[j] / distanceFromCenter[k], 2 / (exponentialWeight - 1));
	}
	return sum;
}

/* генерация начальной матрицы принадлежности */
void OwnershipFunction::generateBeginOwnershipMatrix()
{
	int beginColumn = 0;
	// пробегаем по всем строкам, и начинаем обработку с 1 столбца
	for (int i = 0; i < elementCount; i++)
	{
		// делим строки на elementCount/clasterCount блоков, при достижении границы блока смещаемся на 1 столбец вправо
		if (i >= (double(elementCount) / double(clasterCount - 1)) * (beginColumn + 1) && beginColumn + 1 < clasterCount)
			beginColumn++;

		// пробегаем по всем столбцам начиная с 1, заполняя необходимыми элементами
		for (int j = 0; j < clasterCount; j++)
		{
			// если достигли нужного столбца, заполняем его случайным знаечением с 0 по 1
			if (j == beginColumn)
			{
				// если это последний столбец пишем в него 1
				if (j != clasterCount - 1)
					beginOwnershipMatrix[i][j] = double(rand() % 10) / 10;
				else
					beginOwnershipMatrix[i][j] = 1;
			}
			// иначе смотрим слева или справа находимся, если справа на 1, заполняем значением 1 - сгенерированное число
			else if (j - beginColumn == 1)
			{
				beginOwnershipMatrix[i][j] = 1 - beginOwnershipMatrix[i][j - 1];
			}
			// иначе записываем 0
			else
				beginOwnershipMatrix[i][j] = 0;
		}
	}
}

/* считать центры кластеров из файла */
void OwnershipFunction::readClasterCenter(vector<Point> &clasterCenter)
{
	ifstream in("clasterCenter.txt");
	for (int i = 0; i < clasterCount; i++)
	{
		for (int j = 0; j < clasterCenter[i].coord.size(); j++)
			in >> clasterCenter[i].coord[j];
	}
}

OwnershipFunction::OwnershipFunction(int clasterCount, int exponentialWeight, double eps, int elementCount)
{
	this->clasterCount = clasterCount;
	this->exponentialWeight = exponentialWeight;
	this->eps = eps;
	this->elementCount = elementCount;
	beginOwnershipMatrix.resize(elementCount);
	ownershipMatrix.resize(elementCount);
	clasterCenter.resize(clasterCount);
	for (int i = 0; i < elementCount; i++)
	{
		beginOwnershipMatrix[i].resize(clasterCount);
		ownershipMatrix[i].resize(clasterCount);
	}
	// генерация начальной матрицы принадлежности
	generateBeginOwnershipMatrix();
}

void OwnershipFunction::calcFCMWithoutCenter(vector<Point> x)
{
	readClasterCenter(clasterCenter);
	// матрица расстояний от объекта до центра кластера
	vector<vector<double>> distanceFromCenter;
	distanceFromCenter.resize(elementCount);
	for (int i = 0; i < elementCount; i++)
	{
		distanceFromCenter[i].resize(clasterCount);
		for (int j = 0; j < clasterCount; j++)
		{
			// вычисляем расстояние от i-го объекта до j-го кластера
			distanceFromCenter[i][j] = sqrt(pow((x[i] - clasterCenter[j]).Norm(), 2));
		}
	}
	// вычисляем матрицу принадлежности
	for (int i = 0; i < elementCount; i++)
	{
		for (int j = 0; j < clasterCount; j++)
		{
			ownershipMatrix[i][j] = 1 / (summWithDistanceFromCenter(distanceFromCenter[i], j));
		}
	}
}

vector<double> OwnershipFunction::FCMLineInX(Point x)
{
	vector<double> result;
	result.resize(clasterCount);

	vector<double> distances;
	distances.resize(clasterCount);

	for (int i = 0; i < clasterCount; i++)
	{
		distances[i] = sqrt(pow((x - clasterCenter[i]).Norm(), 2));
	}

	for (int i = 0; i < clasterCount; i++)
	{
		result[i] = 1 / summWithDistanceFromCenter(distances, i);
	}

	return result;
}

vector<vector<double>> OwnershipFunction::getOwnerchipMatrix()
{
	return ownershipMatrix;
}

vector<vector<double>> OwnershipFunction::getDiagOwnershipMatrix(int i)
{
	vector<vector<double>> result;
	result.resize(elementCount);
	for (int j = 0; j < elementCount; j++)
	{
		result[j].resize(elementCount);
		result[j][j] = ownershipMatrix[j][i];
	}

	return result;
}

OwnershipFunction::~OwnershipFunction()
{
}

vector<double> OwnershipFunction::operator[](int i)
{
	return ownershipMatrix[i];
}
