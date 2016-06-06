#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

#pragma once
class Plan
{
private:
	void readPlan(); // считать начальный план из файла
public:
	int remarkCount; // количество наблюдений
	vector<vector<double>> plan;
	void enlarge(double x);
	void enlargeDiscrete(double x);
	void reduce(double x);
	void clean();
	void createRandomPlan(int elementCount);
	vector<vector<double>> getLocalModelMatrix();
	vector<vector<double>> getTransponLocalModelMatrix();
	Plan(int remarkCount);
	~Plan();
	vector<double> operator[](int i);
};

