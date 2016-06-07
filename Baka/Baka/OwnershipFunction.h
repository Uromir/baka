#include <vector>
#include <math.h>
#include <fstream>
#include "Point.h"

using namespace std;

#pragma once
class OwnershipFunction
{
private:
	void generateBeginOwnershipMatrix(); // генерация начальной матрицы принадлежности
	double summWithBeginOwnershipMatrix(vector<Point> x, vector<vector<double>> ownershipMatrix, int i); // посчитать сумму учавствующую в алгоритме FCM, с участием начальной матрицы принадлежности и иксов
	double summWithBeginOwnershipMatrix(vector<vector<double>> ownershipMatrix, int i); // посчитать сумму учавствующую в алгоритме FCM, с участием начальной матрицы принадлежности
	double summWithDistanceFromCenter(vector<double> distanceFromCenter, int j); // посчитать сумму учавствующую в алгоритме FCM, с участием расстояний между иксами и центрами кластеров
	void readClasterCenter(vector<Point> &clasterCenter); // считать центры кластеров из файла
public:
	int clasterCount; // количество кластеров
	int exponentialWeight; // экспоненциальный вес \ коэффициент нечеткости
	double eps; // параметр останова алгоритма
	int elementCount; // количество элементов объекта
	vector<Point> clasterCenter; // центры кластеров
	vector<vector<double>> ownershipMatrix; // матрица принадлежности
	vector<vector<double>> beginOwnershipMatrix; // начальная матрица принадлежности
	void calcFCM(vector<Point> x); // алгоритм FCM
	void calcFCMWithoutCenter(vector<Point> x); // алгоритм FCM с задаваемыми центрами кластеров
	vector<vector<double>> getOwnerchipMatrix();
	vector<double> FCMLineInX(Point x); // вычислить значение каждой функции принадлежности в переданном Х
	vector<vector<double>> getDiagOwnershipMatrix(int i);
	OwnershipFunction(int clasterCount, int exponentialWeight, double eps, int elementCount);
	~OwnershipFunction();
	vector<double> operator[](int i);
};

