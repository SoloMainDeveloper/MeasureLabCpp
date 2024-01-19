#include <string>
#include "ui_QtWidgetsMeasureLab.h"
#include "QtWidgetsMeasureLab.h"
#include "Figure.h"

QMap<int, Point> DataHolder::Points = {};
QMap<int, Plane> DataHolder::Planes = {};
QMap<int, Circle> DataHolder::Circles = {};

Vector::Vector(double x, double y, double z)
{
	this->VectorX = x;
    this->VectorY = y;
    this->VectorZ = z;
    this->ActualVectorX = 0.0;
    this->ActualVectorY = 0.0;
    this->ActualVectorZ = 0.0;
}

Vector::Vector()
{
    this->VectorX = 0.0;
    this->VectorY = 0.0;
    this->VectorZ = 0.0;
    this->ActualVectorX = 0.0;
    this->ActualVectorY = 0.0;
    this->ActualVectorZ = 0.0;
}

void Vector::UpdateFactVector(double x, double y, double z)
{
    this->ActualVectorX = x;
    this->ActualVectorY = y;
    this->ActualVectorZ = z;
}

QString Vector::ToQString()
{
    QString message = "Vector (%1, %2, %3). FACT Vector (%4, %5, %6)";
    return message.arg(VectorX).arg(VectorY).arg(VectorZ).arg(ActualVectorX).arg(ActualVectorY).arg(ActualVectorZ);
}

Point::Point(double x, double y, double z, Vector* vector, int number)
{
    this->X = x;
    this->Y = y;
    this->Z = z;
    this->RealX = 0;
    this->RealY = 0;
    this->RealZ = 0;
    this->Number = number;
    if (vector == nullptr)
        this->Normal = Vector();
    else
        this->Normal = *vector;
}

Point::Point(const QList<double>& arguments, int amount, int number)
{
    this->X = arguments[0];
    this->Y = arguments[1];
    this->Z = arguments[2];
    this->RealX = 0;
    this->RealY = 0;
    this->RealZ = 0;
    this->Number = number;
    if (amount == 6)
    {
        this->Normal = Vector(arguments[3], arguments[4], arguments[5]);
    }
    else
    {
        this->Normal = Vector();
    }
}

Point::Point(double x, double y, double z)
{
    this->X = x;
    this->Y = y;
    this->Z = z;
    this->RealX = 0;
    this->RealY = 0;
    this->RealZ = 0;
    this->Number = -1;
    this->Normal = Vector();
}

Point::Point(int number)
{
    this->X = 0;
    this->Y = 0;
    this->Z = 0;
    this->RealX = 0;
    this->RealY = 0;
    this->RealZ = 0;
    this->Number = number;
    this->Normal = Vector();
}

Point::Point()
{
    this->X = 0;
    this->Y = 0;
    this->Z = 0;
    this->RealX = 0;
    this->RealY = 0;
    this->RealZ = 0;
    this->Number = -1;
    this->Normal = Vector();
}

//int Point::GetNumber()
//{
//    return this->Number;
//}

void Point::UpdateCoordinates(double x, double y, double z)
{
    this->RealX = x;
    this->RealY = y;
    this->RealZ = z;
}

QString Point::ToQString() //todo
{
    QString message = "Dot #%1\nCoordinates of the dot: (%2, %3, %4). FACT(%5, %6, %7)\nDot Normal: %8\n";
    return message.arg(Number).arg(X).arg(Y).arg(Z).arg(RealX).arg(RealY).arg(RealZ).arg(Normal.ToQString());
}

Circle::Circle(Point* centre, double radius, double factRadius, Vector* normal, int number, double difference, double factDifference)
{
    this->Centre = *centre;
    this->Radius = radius;
    this->FactRadius = factRadius;
    this->Normal = *normal;
    this->Number = number;
    this->Difference = difference;
    this->FactDifference = factDifference;
}

Circle::Circle(int number)
{
    this->Number = number;
}

Circle::Circle()
{
}

//int Circle::GetNumber()
//{
//    return this->Number;
//}

QString Circle::ToQString() //todo
{
    QString message = "Circle #%1:\nCentre: %2Radius: %3. FACT Radius: %4\nCircle Normal: %5\nDifference: %6. FACT Difference: %7\n";
    return message.arg(Number).arg(Centre.ToQString()).arg(Radius).arg(FactRadius).arg(Normal.ToQString()).arg(Difference).arg(FactDifference);
}

Plane::Plane()
{
}

Plane::Plane(int number)
{
    this->Number = number;
}

Plane::Plane(Point start, Vector normal, int number, double difference, double factDifference)
{
    this->Start = start;
    this->Normal = normal;
    this->Number = number;
    this->Difference = difference;
    this->FactDifference = factDifference;
}

//int Plane::GetNumber()
//{
//    return this->Number;
//}

QString Plane::ToQString() //todo
{
    QString message = "Plane #%1:\nDot on the plane: %2Plane Normal: %3\nDifference: %4. FACT Difference: %5\n";
    return message.arg(Number).arg(Start.ToQString()).arg(Normal.ToQString()).arg(Difference).arg(FactDifference);
}

int Figure::GetNumber()
{
    return Number;
}
