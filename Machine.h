#pragma once
#include "Figure.h"

using namespace std;

class Machine
{
public:
	Point Position;
	double MaxHeightCm;
	double MaxWidthCm;
	double MaxLengthCm;
	double Deviation;

	void ReloadPosition();
	Machine(double height, double width, double length, double deviation, Point position);
	Point Move(Point route);
	bool TryCreatePoint(QList<double> arguments, int number, Point& point);
	bool TryCreateCircle(QList<double> arguments, int number, Circle& circle);
	bool TryCreatePlane(QList<double> arguments, int number, Plane& plane);
	QString DevCircle(QList<double> arguments, int number);
	QString DevPoint(QList<double> arguments, int number);
	bool TryCreateProjection(QList<double> arguments, int number, Point& projectionPoint);
	QString Comment(QString comment);
	QString UnknownCommand(QString input);

private:
	Vector GetNormalizedVector(double v1, double v2, double v3);
	double GetInvLength(double x, double y, double z);
	tuple<double, double> GetRadiusBy3Points(Point p1, Point p2, Point p3);
	tuple<double, double> GetSegmentLength(Point p1, Point p2);
	tuple<Vector, double, double> GetNormalAndDifference(Point start, Point p2, Point p3);
	double GetDetermintantDoubleMatrix(double n1, double n2, double m1, double m2);
	Point GetCircleCentre(Point p1, Point p2, Point p3, Vector vector, int number = -1);
	double* GetCenterPoint(double* p1, double* p2, double* p3);
	bool IsValueLessOrEqualZero(double* p1, double* p2, double* p3, double& min);
	double* GetNormalVector(double* p1, double* p2, double* p3);
	double* GetCoefficientEquation(double* point);
	double GetValueDeterminant(int i, int j, int k, double* coefFirstEquat, double* coefSecondEquat, double* coefThirdEquat);
	double* GetNewNormalVector(double* normalVector, double* firstPoint, double* fourthPoint, double sidesSquareNorVec);
};

