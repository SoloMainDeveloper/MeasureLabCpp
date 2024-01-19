#include "Machine.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include "Figure.h"
using namespace std;

Machine::Machine(double height, double width, double length, double deviation, Point position)
{
    this->MaxHeightCm = height;
    this->MaxWidthCm = width;
    this->MaxLengthCm = length;
    this->Deviation = deviation;
    this->Position = position;
}

void Machine::ReloadPosition()
{
    this->Position = Point(0, 0, 0);
}

Point Machine::Move(Point route)
{
    auto wantedX = Position.X + route.X;
    auto wantedY = Position.Y + route.Y;
    auto wantedZ = Position.Z + route.Z;
    if (wantedX < 0)
        wantedX = 0;
    if (wantedX > MaxLengthCm)
        wantedX = MaxLengthCm;
    if (wantedY < 0)
        wantedY = 0;
    if (wantedY > MaxWidthCm)
        wantedY = MaxWidthCm;
    if (wantedZ < 0)
        wantedZ = 0;
    if (wantedZ > MaxHeightCm)
        wantedZ = MaxHeightCm;
    return Point(wantedX, wantedY, wantedZ);
}

bool Machine::TryCreatePoint(QList<double> arguments, int number, Point& point)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.5, 0.5);
    
    auto normal = GetNormalizedVector(arguments[3], arguments[4], arguments[5]);
    point = Point(arguments, 3, number);
    point.Normal = normal;
    auto realX = point.X + round((dis(gen)) * Deviation * 10000) / 10000;
    auto realY = point.Y + round((dis(gen)) * Deviation * 10000) / 10000;
    auto realZ = point.Z + round((dis(gen)) * Deviation * 10000) / 10000;
    auto normalRealX = point.Normal.VectorX + round((dis(gen)) * Deviation * 0.5 * 10000) / 10000;
    auto normalRealY = point.Normal.VectorY + round((dis(gen)) * Deviation * 0.5 * 10000) / 10000;
    auto normalRealZ = point.Normal.VectorZ + round((dis(gen)) * Deviation * 0.5 * 10000) / 10000;
    auto realNormal = GetNormalizedVector(normalRealX, normalRealY, normalRealZ);
    point.UpdateCoordinates(realX, realY, realZ);
    point.Normal.UpdateFactVector(realNormal.VectorX, realNormal.VectorY, realNormal.VectorZ);
    if (DataHolder::Points.contains(point.GetNumber()) || isnan(normal.VectorX) || isnan(normal.VectorY) || isnan(normal.VectorZ))
        return false;
    else
        DataHolder::Points.insert(point.GetNumber(), point);
        return true;
}

Vector Machine::GetNormalizedVector(double v1, double v2, double v3)
{
    auto invLength = GetInvLength(v1, v2, v3);
    return Vector(round(v1 * invLength * 10000) / 10000, round(v2 * invLength * 10000) / 10000, round(v3 * invLength * 10000) / 10000);
}

double Machine::GetInvLength(double x, double y, double z)
{
    double length = sqrt(x * x + y * y + z * z);
    return 1.0 / length;
}

bool Machine::TryCreateCircle(QList<double> arguments, int number, Circle& circle)
{
    auto points = QList<Point>();
    for (int i = 0; i < arguments.length(); i++) {
        if (DataHolder::Points.contains(arguments[i]))
            points.append(DataHolder::Points[arguments[i]]);
        else
            return false;
    }
    auto equation = GetNormalAndDifference(points[0], points[1], points[2]);
    auto radius = GetRadiusBy3Points(points[0], points[1], points[2]);
    auto centre = GetCircleCentre(points[0], points[1], points[2], get<0>(equation), number);
    if (isnan(centre.X) || isnan(centre.Y) || isnan(centre.Z))
        return false;
    circle = Circle(&centre, get<0>(radius), get<1>(radius), &get<0>(equation), number, get<1>(equation), get<2>(equation));
    if (DataHolder::Circles.contains(circle.GetNumber()))
        return false;
    else
        DataHolder::Circles.insert(circle.GetNumber(), circle);
    return true;
}

std::tuple<double, double> Machine::GetRadiusBy3Points(Point p1, Point p2, Point p3) //todo
{
    auto length1 = get<0>(GetSegmentLength(p1, p2));
    auto length2 = get<0>(GetSegmentLength(p2, p3));
    auto length3 = get<0>(GetSegmentLength(p1, p3));
    auto halfPerim = (length1 + length2 + length3) / 2;
    auto square = sqrt(halfPerim * (halfPerim - length1) * (halfPerim - length2) * (halfPerim - length3));
    auto radius = round((length1 * length2 * length3 / (4 * square)) * 10000) / 10000;

    auto realLength1 = get<1>(GetSegmentLength(p1, p2));
    auto realLength2 = get<1>(GetSegmentLength(p2, p3));
    auto realLength3 = get<1>(GetSegmentLength(p1, p3));
    auto realHalfPerim = (realLength1 + realLength2 + realLength3) / 2;
    auto realSquare = sqrt(realHalfPerim * (realHalfPerim - realLength1) * (realHalfPerim - realLength2) * (realHalfPerim - realLength3));
    auto realRadius = round((realLength1 * realLength2 * realLength3 / (4 * realSquare)) * 10000) / 10000;

    return tuple(radius, realRadius);
}

tuple<double, double> Machine::GetSegmentLength(Point p1, Point p2)
{
    auto length = round(sqrt(pow(p1.X - p2.X, 2) + pow(p1.Y - p2.Y, 2) + pow(p1.Z - p2.Z, 2)) * 10000) / 10000;
    auto realLength = round(sqrt(pow(p1.RealX - p2.RealX, 2) + pow(p1.RealY - p2.RealY, 2) + pow(p1.RealZ - p2.RealZ, 2)) * 10000) / 10000;
    return tuple(length, realLength);
}

bool Machine::TryCreatePlane(QList<double> arguments, int number, Plane& plane)
{
    auto points = QList<Point>();
    for (double item : arguments) {
        if (DataHolder::Points.contains(int(item))) {
            Point point = DataHolder::Points[int(item)];
            points.append(point);
        }
        else
            return false;
    }
    auto equation = GetNormalAndDifference(points[0], points[1], points[2]);
    plane = Plane(points[0], get<0>(equation), number, get<1>(equation), get<2>(equation));
    if (DataHolder::Planes.contains(number))
        return false;
    else {
        DataHolder::Planes.insert(number, plane);
        return true;
    }
}

QString Machine::DevCircle(QList<double> arguments, int number)
{
    auto devCentre = arguments[0];
    auto devRadius = arguments[1];
    if (DataHolder::Circles.contains(number))
    {
        Circle circle = DataHolder::Circles[number];
        auto centre = circle.Centre;
        auto diffCentre = sqrt(pow(centre.X - centre.RealX, 2) + pow(centre.Y - centre.RealY, 2) + pow(centre.Z - centre.RealZ, 2));
        auto diffRadius = abs(circle.Radius - circle.FactRadius);
        QString message = "Deviation of the Circle#%7 centre: %1 cm. Standard deviation %2 cm. Conforms to the standard: %3\n";
        message.append("Deviation of the Circle#%7 radius: %4 cm. Standard deviation %5 cm. Conforms to the standard: %6\n");
        return message.arg(round(diffCentre * 100000) / 100000).arg(devCentre).arg(diffCentre < devCentre)
            .arg(round(diffRadius * 1000000) / 1000000).arg(devRadius).arg(diffRadius < devRadius).arg(number);
    }
    else
    {
        return QString("Could not find the circle");
    }
}

QString Machine::DevPoint(QList<double> arguments, int number)
{
    auto deviation = arguments[0];
    if (DataHolder::Points.contains(number)) {
        Point point = DataHolder::Points[number];
        auto distance = sqrt(pow(point.X - point.RealX, 2) + pow(point.Y - point.RealY, 2) + pow(point.Z - point.RealZ, 2));
        QString message = "Deviation for Dot #%4: from nominal values: %1 cm. Standard deviation: %2 cm. Conforms to the standard: %3\n";
        return message.arg(round(distance * 100000) / 100000).arg(deviation).arg(deviation > distance).arg(number);
    }
    else {
        return QString("Could not find the dot");
    }
}

bool Machine::TryCreateProjection(QList<double> arguments, int number, Point& projectionPoint) //todo
{
    auto planeNumber = arguments[0];
    auto pointNumber = arguments[1];
    projectionPoint = Point(0, 0, 0);
    if (DataHolder::Points.contains((int)pointNumber) && DataHolder::Planes.contains((int)planeNumber))
    {
        auto point = DataHolder::Points[(int)pointNumber];
        auto plane = DataHolder::Planes[(int)planeNumber];
        auto normal = plane.Normal;
        auto paramT = -1 * (normal.VectorX * point.X + normal.VectorY * point.Y + normal.VectorZ * point.Z + plane.Difference)
            / (pow(normal.VectorX, 2) + pow(normal.VectorY, 2) + pow(normal.VectorZ, 2));
        auto x = normal.VectorX * paramT + point.X;
        auto y = normal.VectorY * paramT + point.Y;
        auto z = normal.VectorZ * paramT + point.Z;
        auto realParamT = -1 * (normal.ActualVectorX * point.RealX + normal.ActualVectorY * point.RealY + normal.ActualVectorZ * point.RealZ + plane.Difference)
            / (pow(normal.ActualVectorX, 2) + pow(normal.ActualVectorY, 2) + pow(normal.ActualVectorZ, 2));
        auto realX = normal.ActualVectorX * realParamT + point.RealX;
        auto realY = normal.ActualVectorY * realParamT + point.RealY;
        auto realZ = normal.ActualVectorZ * realParamT + point.RealZ;
        projectionPoint = Point(round(x * 10000) / 10000, round(y * 10000) / 10000, round(z * 10000) / 10000, &normal, number);
        projectionPoint.UpdateCoordinates(round(realX * 10000) / 10000, round(realY * 10000) / 10000, round(realZ * 10000) / 10000);
        if (DataHolder::Points.contains(projectionPoint.GetNumber()))
            return false;
        else {
            DataHolder::Points.insert(projectionPoint.GetNumber(), projectionPoint);
            return true;
        }
    }
    else
    {
        return false;
    }
}

QString Machine::Comment(QString comment)
{
    QString message = "Comment: %1\n";
    return message.arg(comment);
}

QString Machine::UnknownCommand(QString input)
{
    QString message = "Unknown command: %1\n";
    return message.arg(input);
}

tuple<Vector, double, double> Machine::GetNormalAndDifference(Point start, Point p2, Point p3) //todo
{
    auto coefX = GetDetermintantDoubleMatrix(p2.Y - start.Y, p3.Y - start.Y, p2.Z - start.Z, p3.Z - start.Z);
    auto coefY = -GetDetermintantDoubleMatrix(p2.X - start.X, p3.X - start.X, p2.Z - start.Z, p3.Z - start.Z);
    auto coefZ = GetDetermintantDoubleMatrix(p2.X - start.X, p3.X - start.X, p2.Y - start.Y, p3.Y - start.Y);
    auto d = round((coefX * (-start.X) + coefY * (-start.Y) + coefZ * (-start.Z)) * 10000) / 10000;

    auto coefX2 = GetDetermintantDoubleMatrix(p2.RealY - start.RealY, p3.RealY - start.RealY, p2.RealZ - start.RealZ, p3.RealZ - start.RealZ);
    auto coefY2 = -GetDetermintantDoubleMatrix(p2.RealX - start.RealX, p3.RealX - start.RealX, p2.RealZ - start.RealZ, p3.RealZ - start.RealZ);
    auto coefZ2 = GetDetermintantDoubleMatrix(p2.RealX - start.RealX, p3.RealX - start.RealX, p2.RealY - start.RealY, p3.RealY - start.RealY);
    auto d2 = round((coefX2 * (-start.RealX) + coefY2 * (-start.RealY) + coefZ2 * (-start.RealZ)) * 10000) / 10000;
    auto normal = GetNormalizedVector(coefX, coefY, coefZ);
    auto realNormal = GetNormalizedVector(coefX2, coefY2, coefZ2);
    auto invLength = GetInvLength(coefX, coefY, coefZ);
    auto invLength2 = GetInvLength(coefX2, coefY2, coefZ2);
    normal.UpdateFactVector(realNormal.VectorX, realNormal.VectorY, realNormal.VectorZ);
    return tuple(normal, round(d * invLength * 10000) / 10000, round(d2 * invLength2 * 10000) / 10000);
}

double Machine::GetDetermintantDoubleMatrix(double n1, double n2, double m1, double m2) //todo
{
    return n1 * m2 - m1 * n2;
}

Point Machine::GetCircleCentre(Point p1, Point p2, Point p3, Vector vector, int number) //todo
{
    double point1[3] = { p1.X, p1.Y, p1.Z };
    double point2[3] = { p2.X, p2.Y, p2.Z };
    double point3[3] = { p3.X, p3.Y, p3.Z };
    double realPoint4[3] = { p1.RealX, p1.RealY, p1.RealZ };
    double realPoint5[3] = { p2.RealX, p2.RealY, p2.RealZ };
    double realPoint6[3] = { p3.RealX, p3.RealY, p3.RealZ };
    auto centre = GetCenterPoint(point1, point2, point3);

    auto actualCentre = GetCenterPoint(realPoint4, realPoint5, realPoint6);
    auto result = Point(centre[0], centre[1], centre[2], &vector, number * 100 + number);
    result.UpdateCoordinates(actualCentre[0], actualCentre[1], actualCentre[2]);
    if (!DataHolder::Points.contains(result.GetNumber()))
        DataHolder::Points.insert(result.GetNumber(), result);
    return result;
}

double* Machine::GetCenterPoint(double* p1, double* p2, double* p3) //todo
{
    double delta;
    if (IsValueLessOrEqualZero(p1, p2, p3, delta))
    {
        delta = abs(delta);
        for (auto i = 0; i < 3; i++)
        {
            *(p1 + i) += delta + 1;
            *(p2 + i) += delta + 1;
            *(p3 + i) += delta + 1;
        }
    }

    //Находим нормальный вектор
    auto normalVector = GetNormalVector(p1, p2, p3);

    auto sidesSquareNorVec = (*normalVector) * (*normalVector) +
        (*(normalVector+1)) * (*(normalVector+1)) + (*(normalVector+2)) * (*(normalVector+2));

    //Находим коэфициенты уравнений
    auto coefFirstEquat = GetCoefficientEquation(p1);
    auto coefSecondEquat = GetCoefficientEquation(p2);
    auto coefThirdEquat = GetCoefficientEquation(p3);

    //Решаем сиcтему уравнений методом Крамера
    double fourthPoint[3];
    auto generalDeterminant = GetValueDeterminant(0, 1, 2, coefFirstEquat, coefSecondEquat, coefThirdEquat);
    auto firstDeterminant = GetValueDeterminant(3, 1, 2, coefFirstEquat, coefSecondEquat, coefThirdEquat);
    auto secondDeterminant = GetValueDeterminant(0, 3, 2, coefFirstEquat, coefSecondEquat, coefThirdEquat);
    auto thirdDeterminant = GetValueDeterminant(0, 1, 3, coefFirstEquat, coefSecondEquat, coefThirdEquat);

    fourthPoint[0] = firstDeterminant / generalDeterminant;
    fourthPoint[1] = secondDeterminant / generalDeterminant;
    fourthPoint[2] = thirdDeterminant / generalDeterminant;

    //Находим центр окружности
    auto newNormalVector = GetNewNormalVector(normalVector, p1, fourthPoint, sidesSquareNorVec);

    auto centerPoint = new double[3];
    for (auto i = 0; i < 3; i++)
    {
        *(centerPoint + i) = round(((*(fourthPoint + i)) - (*(newNormalVector + i)) - delta - 1) * 10000) / 10000;
    }

    return centerPoint;
}

bool Machine::IsValueLessOrEqualZero(double* p1, double* p2, double* p3, double& minimal) //todo
{
    minimal = *p1;
    for (int i = 0; i < 3; i++) {
        auto c = min(*(p1+i), min(*(p2 + i), *(p3 + i)));
        if (c < minimal)
            minimal = c;
    }
    return minimal <= 0;
}

double* Machine::GetNormalVector(double* p1, double* p2, double* p3)
{
    auto firstVector = new double[3];
    auto secondVector = new double[3];
    auto normalVector = new double[3];

    for (auto i = 0; i < 3; i++) {
        *(firstVector + i) = *(p2+i) - *(p1+i);
        *(secondVector + i) = *(p3+i) - *(p1+i);
    }

    *(normalVector) = round(((*(firstVector +1)) * (*(secondVector +2)) - (*(firstVector +2)) * (*(secondVector +1))) * 10000) / 10000;
    *(normalVector + 1) = round(-((*(firstVector +0)) * (*(secondVector +2)) - (*(firstVector +2)) * (*(secondVector +0))) * 10000) / 10000;
    *(normalVector + 2) = round(((*(firstVector +0)) * (*(secondVector +1)) - (*(firstVector +1)) * (*(secondVector +0))) * 10000) / 10000;

    return normalVector;
}

double* Machine::GetCoefficientEquation(double* point)
{
    auto coefficientsEquat = new double[4];
    for (int i = 0; i < 3; i++)
        *(coefficientsEquat+i) = *(point + i) * 2;
    *(coefficientsEquat+3) = (*point) * (*point) + (*(point + 1)) * (*(point + 1)) + (*(point + 2)) * (*(point + 2));
    return coefficientsEquat;
}

double Machine::GetValueDeterminant(int i, int j, int k, double* a, double* b, double* c)
{
    auto determinant = (*(a + i)) * (*(b + j)) * (*(c + k)) +
        (*(a + j)) * (*(b + k)) * (*(c + i)) +
        (*(a + k)) * (*(b + i)) * (*(c + j)) -
        ((*(a + k)) * (*(b + j)) * (*(c + i)) +
            (*(a + i)) * (*(b + k)) * (*(c + j)) +
            (*(a + j)) * (*(b + i)) * (*(c + k)));

    return determinant;
}

double* Machine::GetNewNormalVector(double* normalVector, double* firstPoint, double* fourthPoint, double sidesSquareNorVec)
{
    double firstScalarProduct = 0.0;
    double secondScalarProduct = 0.0;

    for (auto i = 0; i < 3; i++)
    {
        firstScalarProduct += (*(normalVector + i)) * (*(fourthPoint + i));
        secondScalarProduct += (*(normalVector + i)) * (*(firstPoint + i));
    }

    auto k = (firstScalarProduct - secondScalarProduct) / sidesSquareNorVec;
    auto newNormalVector = new double[3];

    for (auto i = 0; i < 3; i++)
    {
        *(newNormalVector + i) = round((k * (*(normalVector + i))) * 10000) / 10000;
    }

    return newNormalVector;

}
