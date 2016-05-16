﻿#include <vector>
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
	void enlarge(double x);
	void enlargeDiscrete(double x);
	void reduce(double x);
	void clean();
	Plan(int remarkCount);
	~Plan();
	vector<double> operator[](int i);
};

