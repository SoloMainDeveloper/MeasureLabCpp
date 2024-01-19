#include "HTMLReportCreator.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <errno.h>
#include <io.h>

#include  <stdio.h>
#include  <stdlib.h>

using namespace std;

FigureReport::FigureReport(QString name, Figure figure, QList<double>* arguments, QString comment)
{
	this->FigureName = name;
	this->figure = figure;
	this->Arguments = *arguments;
	this->Comment = comment;
}

FigureReport::FigureReport(QString name, Command command)
{
	this->FigureName = name;
	this->command = command;
}

void HTMLReportCreator::CreateHTMLReport(QList<FigureReport>& data, QString name)
{
	wstring folderName = L"Report";
	if (_wmkdir(folderName.c_str()) == 0 || _waccess(folderName.c_str(), 00) == 0) {
		wstring fullName = folderName + L"\\" + name.toStdWString() + L".html";
		ofstream file(fullName);
		auto header = name.toStdString();
		if (file.is_open()) {
			file << "<html><head><title>" << header << "</title>";
			file << "<style>";
			file << "h1 { margin-bottom: 15px; }";
			file << ".flex-figure { display: flex; }";
			file << ".table-flex { margin-right: 20px; }";
			file << "table { margin-bottom: 10px; width: 500px; }";
			file << "td, th { height: 40px; padding: 5px; font-size: 20px; }";
			file << "</style></head><body><h1>Название отчёта: " << header << "</h1>";
			for (auto figure : data) {
				auto args = figure.Arguments;
				if (figure.FigureName == "POINT") {
					file << "<table style=\"text-align: center;\" border=\"1\"><thead><tr><th>" << "POINT #" << figure.figure.Number << "</th>";
					file << "<th>X</th><th>Y</th><th>Z</th></tr></thead>";
					file <<	"<tbody>";
					file << "<tr><td><b>Coordinates</b></td><td>" << args[0] << "</td><td>" << args[1] << "</td><td>" << args[2] << "</td></tr>";
					file << "<tr><td><b>Real coordinates</b></td><td>" << args[3] << "</td><td>" << args[4] << "</td><td>" << args[5] << "</td></tr>";
					file << "<tr><td><b>Normal</b></td><td>" << args[6] << "</td><td>" << args[7] << "</td><td>" << args[8] << "</td></tr>";
					file << "<tr><td><b>Real normal</b></td><td>" << args[9] << "</td><td>" << args[10] << "</td><td>" << args[11] << "</td></tr>";
					file << "</tbody></table>";
				}
				else if (figure.FigureName == "CIRCLE") {
					file << "<div class=\"flex-figure\"><table class=\"table-flex\" style=\"text-align: center;\" border=\"1\"><thead><tr><th>" << "CIRCLE #" << figure.figure.Number << "</th>";
					file << "<th>X</th><th>Y</th><th>Z</th></tr></thead>";
					file << "<tbody>";
					file << "<tr><td><b>Center</b></td><td>" << args[0] << "</td><td>" << args[1] << "</td><td>" << args[2] << "</td></tr>";
					file << "<tr><td><b>Real center</b></td><td>" << args[3] << "</td><td>" << args[4] << "</td><td>" << args[5] << "</td></tr>";
					file << "<tr><td><b>Normal</b></td><td>" << args[6] << "</td><td>" << args[7] << "</td><td>" << args[8] << "</td></tr>";
					file << "<tr><td><b>Real normal</b></td><td>" << args[9] << "</td><td>" << args[10] << "</td><td>" << args[11] << "</td></tr>";
					file << "</tbody></table>";
					file << "<table class=\"table-flex\" style=\"text-align: center;\" border=\"1\"><thead><tr>";
					file << "<th>Radius</th><th>Fact Radius</th><th>Difference</th><th>Fact Difference</th></thead>";
					file << "<tbody>";
					file << "<tr><td>" << args[12] << "</td><td>" << args[13] << "</td><td>" << args[14] << "</td><td>" << args[14] << "</td></tr></tbody></table>";
					file << "</div>";
				}
				else if (figure.FigureName == "PLANE") {
					file << "<div class=\"flex-figure\"><table class=\"table-flex\" style=\"text-align: center;\" border=\"1\"><thead><tr><th>" << "PLANE #" << figure.figure.Number << "</th>";
					file << "<th>X</th><th>Y</th><th>Z</th></tr></thead>";
					file << "<tbody>";
					file << "<tr><td><b>Dot on the plane</b></td><td>" << args[0] << "</td><td>" << args[1] << "</td><td>" << args[2] << "</td></tr>";
					file << "<tr><td><b>Real dot</b></td><td>" << args[3] << "</td><td>" << args[4] << "</td><td>" << args[5] << "</td></tr>";
					file << "<tr><td><b>Normal</b></td><td>" << args[6] << "</td><td>" << args[7] << "</td><td>" << args[8] << "</td></tr>";
					file << "<tr><td><b>Real normal</b></td><td>" << args[9] << "</td><td>" << args[10] << "</td><td>" << args[11] << "</td></tr>";
					file << "</tbody></table>";
					file << "<table class=\"table-flex\" style=\"text-align: center;\" border=\"1\"><thead><tr>";
					file << "<th>Difference</th><th>Fact Difference</th>";
					file << "<tbody>";
					file << "<tr><td>" << args[12] << "</td><td>" << args[13] << "</td></tr></tbody></table>";
					file << "</div>";
				}
				else if (figure.FigureName == "DEVPOINT") {
					args = figure.command.Arguments;
					auto deviation = args[0];
					auto number = figure.command.Number;
					if (DataHolder::Points.contains(number)) {
						Point point = DataHolder::Points[number];
						auto distance = sqrt(pow(point.X - point.RealX, 2) + pow(point.Y - point.RealY, 2) + pow(point.Z - point.RealZ, 2));
						file << "<table style=\"text-align: center;\" border=\"1\"><thead><tr><th>" << "Deviation for Point #" << number << "</th>";
						file << "<th>Standard deviation</th><th>Conforms to the standard</th></tr></thead><tbody>";
						file << "<tr><td>" << distance << "</td><td>" << deviation << "</td><td>" << BooleanToString(deviation > distance) << "</td></tr>";
						file << "</tbody></table>";
					}
				}
				else if (figure.FigureName == "DEVCIRCLE") {
					args = figure.command.Arguments;
					auto deviation = args[0];
					auto devCentre = args[0];
					auto devRadius = args[1];
					auto number = figure.command.Number;
					if (DataHolder::Circles.contains(number))
					{
						Circle circle = DataHolder::Circles[number];
						auto centre = circle.Centre;
						auto diffCentre = round((sqrt(pow(centre.X - centre.RealX, 2) + pow(centre.Y - centre.RealY, 2) + pow(centre.Z - centre.RealZ, 2))) * 10000) / 10000;
						auto diffRadius = round(abs(circle.Radius - circle.FactRadius) * 10000) / 10000;
						file << "<table style=\"text-align: center;\" border=\"1\"><thead><tr><th>" << "Deviation for Circle #" << number << "</th>";
						file << "<th>Real deviation</th><th>Standard deviation</th><th>Conforms to the standard</th></tr></thead><tbody>";
						file << "<tr><td>Centre</td><td>" << diffCentre << "</td><td>" << devCentre << "</td><td>" << BooleanToString(devCentre > diffCentre) << "</td></tr>";
						file << "<tr><td>Radius</td><td>" << diffRadius << "</td><td>" << devRadius << "</td><td>" << BooleanToString(devRadius > diffRadius) << "</td></tr>";
						file << "</tbody></table>";
					}
				}
				else if (figure.FigureName == "PROJECTION") {
					file << "<table style=\"text-align: center;\" border=\"1\"><thead><tr><th>" << "PROJECTION POINT #" << figure.figure.Number << "</th>";
					file << "<th>X</th><th>Y</th><th>Z</th></tr></thead>";
					file << "<tbody>";
					file << "<tr><td><b>Coordinates</b></td><td>" << args[0] << "</td><td>" << args[1] << "</td><td>" << args[2] << "</td></tr>";
					file << "<tr><td><b>Real coordinates</b></td><td>" << args[3] << "</td><td>" << args[4] << "</td><td>" << args[5] << "</td></tr>";
					file << "<tr><td><b>Normal</b></td><td>" << args[6] << "</td><td>" << args[7] << "</td><td>" << args[8] << "</td></tr>";
					file << "<tr><td><b>Real normal</b></td><td>" << args[9] << "</td><td>" << args[10] << "</td><td>" << args[11] << "</td></tr>";
					file << "</tbody></table>";
				}
				else if (figure.FigureName == "COMMENT") {
					file << "<h2>Комментарий: " << figure.Comment.toStdString() << "</h2";
				}
			}
			file << "</body></html>";
			file.close();
		}
	}
	else {
		return;
	}
}

string HTMLReportCreator::BooleanToString(bool arg)
{
	return arg ? "True" : "False";
}
