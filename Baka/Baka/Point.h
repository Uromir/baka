#include <vector>

#pragma once

using namespace std;

class Point
{
public:
	int size;
	vector<double> coord;
	double weight;
	double Norm();
	Point(void);
	Point(int size);
	Point(int size, vector<double> coord);
	~Point(void);
	double operator[](int i);
	bool operator == (Point x);
	Point operator - (Point x);
	Point operator + (Point x);
	Point operator * (double x);
	Point operator * (int x);
};

