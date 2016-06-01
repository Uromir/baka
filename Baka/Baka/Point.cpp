#include "Point.h"


Point::Point(void)
{
}

Point::Point(int size)
{
	this->size = size;
	coord.resize(size);
}

Point::Point(int size, vector<double> coord)
{
	this->size = size;
	this->coord.resize(size);
	this->coord = coord;
}

Point::~Point(void)
{
}

double Point::operator[](int i)
{
	return coord[i];
}

double Point::Norm()
{
	double norm = 0;
	for (int i = 0; i < size; i++)
	{
		norm += coord[i]*coord[i];
	}
	return sqrt(norm);
}
	
bool Point::operator == (Point x)
{
	bool isEqual = true;

	for (int i = 0; i < size; i++)
	{
		if ((*this)[i] != x[i])
		{
			isEqual = false;
			break;
		}
	}

	return isEqual;
}

Point Point::operator - (Point x)
{
	Point result(this->size);
	for (int i = 0; i < this->size; i++)
	{
		result.coord[i] = (*this)[i] - x[i];
	}
	return result;
}

Point Point::operator - (Point x)
{
	Point result(this->size);
	for (int i = 0; i < this->size; i++)
	{
		result.coord[i] = (*this)[i] + x[i];
	}
	return result;
}

Point Point::operator * (double x)
{
	for (int i = 0; i < size; i++)
	{
		coord[i] *= x;
	}
	return *this;
}

Point Point::operator * (int x)
{
	for (int i = 0; i < size; i++)
	{
		coord[i] *= x;
	}
	return *this;
}
