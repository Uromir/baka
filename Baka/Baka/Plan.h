#include <vector>
#include <fstream>

using namespace std;

#pragma once
class Plan
{
private:
	void readPlan(); // считать начальный план из файла
public:
	int remarkCount; // количество наблюдений
	vector<vector<double>> plan;
	Plan();
	~Plan();
	vector<double> operator[](int i);
};

