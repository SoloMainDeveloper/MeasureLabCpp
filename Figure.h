#pragma once
#include <QMap>
#include "ui_QtWidgetsMeasureLab.h"

class Figure
{
public:
	int Number;
	int GetNumber();
};

class Vector{
public:
	double VectorX;
	double VectorY;
	double VectorZ;

	double ActualVectorX;
	double ActualVectorY;
	double ActualVectorZ;

	Vector(double x, double y, double z);
	Vector();
	void UpdateFactVector(double x, double y, double z);
	QString ToQString();
};

class Point : public Figure {
public:
	double X;
	double Y;
	double Z;
	double RealX;
	double RealY;
	double RealZ;
	Vector Normal;
	Point(double x, double y, double z, Vector* vector, int number = -1);
	Point(const QList<double>& arguments, int amount = 3, int number = -1);
	Point(double x, double y, double z);
	Point(int number);
	Point();
	//int GetNumber();
	void UpdateCoordinates(double x, double y, double z);
	QString ToQString();
};

class Circle : public Figure {
public:
	Point Centre;
	double Radius;
	double FactRadius;
	Vector Normal;
	double Difference;
	double FactDifference;

	Circle(Point* centre, double radius, double factRadius, Vector* normal, int number, double difference = 0, double factDifference = 0);
	Circle(int number);
	Circle();
	//int GetNumber();
	QString ToQString();
};

class Plane : public Figure {
public:
	Point Start;
	Vector Normal;
	double Difference;
	double FactDifference;

	Plane();
	Plane(int number);
	Plane(Point start, Vector normal, int number, double difference = 0, double factDifference = 0);
	//int GetNumber();
	QString ToQString();
};

class DataHolder
{
public:
	static QMap<int, Point> Points;
	static QMap<int, Circle> Circles;
	static QMap<int, Plane> Planes;
};