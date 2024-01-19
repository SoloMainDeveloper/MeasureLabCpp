#pragma once
#include "ui_QtWidgetsMeasureLab.h"
#include "Figure.h"
#include "Command.h"
using namespace std;

class FigureReport
{
public:
	QString FigureName;
	Figure figure;
	QList<double> Arguments;
	QString Comment;
	Command command;

	FigureReport(QString name, Figure figure, QList<double>* arguments, QString comment = "");
	FigureReport(QString name, Command command);
};

class HTMLReportCreator
{
public:
	static void CreateHTMLReport(QList<FigureReport>& data, QString name);
	static string BooleanToString(bool arg);
};

