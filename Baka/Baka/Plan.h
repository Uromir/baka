#include <vector>
#include <fstream>
#include "Point.h"

using namespace std;

#pragma once
class Plan
{
private:
	void readPlan(); // считать начальный план из файла
public:
	int dimensionSpace; // мерность пространства
	int remarkCount; // количество наблюдений
	vector<Point> plan;
	void enlarge(Point x);
	void enlargeDiscrete(Point x);
	void reduce(Point x);
	void reduceDiscrete(Point x);
	void clean();
	void createRandomPlan(int elementCount);
	vector<vector<double>> getLocalModelMatrix();
	vector<vector<double>> getTransponLocalModelMatrix();
	Plan(int remarkCount, int dimensionSpace);
	~Plan();
	Point operator[](int i);
};

