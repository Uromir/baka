#include <vector>
#include <math.h>
#include <fstream>

using namespace std;

#pragma once
class OwnershipFunction
{
private:
	void generateBeginOwnershipMatrix(); // генерация начальной матрицы принадлежности
	double summWithBeginOwnershipMatrix(vector<double> x, vector<vector<double>> ownershipMatrix, int i); // посчитать сумму учавствующую в алгоритме FCM, с участием начальной матрицы принадлежности и иксов
	double summWithBeginOwnershipMatrix(vector<vector<double>> ownershipMatrix, int i); // посчитать сумму учавствующую в алгоритме FCM, с участием начальной матрицы принадлежности
	double summWithDistanceFromCenter(vector<double> distanceFromCenter, int j); // посчитать сумму учавствующую в алгоритме FCM, с участием расстояний между иксами и центрами кластеров
	double calcObjectveFunction(vector<double> x, vector<vector<double>> distanceFromCenter); // посчитать значение целевой функции с матрицей принадлежности прошлой итерации
	double calcObjectveFunction(vector<double> x); // посчитать значение целевой функции
	void readClasterCenter(vector<double> &clasterCenter); // считать центры кластеров из файла
public:
	int clasterCount; // количество кластеров
	int exponentialWeight; // экспоненциальный вес \ коэффициент нечеткости
	double eps; // параметр останова алгоритма
	int elementCount; // количество элементов объекта
	vector<double> clasterCenter; // центры кластеров
	vector<vector<double>> ownershipMatrix; // матрица принадлежности
	vector<vector<double>> beginOwnershipMatrix; // начальная матрица принадлежности
	void calcFCM(vector<double> x); // алгоритм FCM
	void calcFCMWithoutCenter(vector<double> x); // алгоритм FCM с задаваемыми центрами кластеров
	vector<vector<double>> getOwnerchipMatrix();
	vector<double> FCMLineInX(double x); // вычислить значение каждой функции принадлежности в переданном Х
	OwnershipFunction(int clasterCount, int exponentialWeight, double eps, int elementCount);
	~OwnershipFunction();
	vector<double> operator[](int i);
};

