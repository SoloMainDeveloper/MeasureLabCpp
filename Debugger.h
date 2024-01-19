#pragma once
#include <ui_QtWidgetsMeasureLab.h>
#include "Command.h"
#include "Machine.h"
#include "HTMLReportCreator.h";

static class Debugger
{
public:
	static int commandNumber;
	static QList<FigureReport> figuresData;
	static Machine machine;
	static void RunCode(Ui::QtWidgetsMeasureLabClass ui, QString inputCode, int type);
	static void ReloadCommandNumber();
	static QList<double> TryGetArguments(CommandType type, QString input);

private:
	static QStringList RemoveUselessSymbolsAndParse(QString inputCode);
	static Command GetCommand(QString input);
	static QList<double> SubstringArguments(QString input, int start, int end, CommandType type);
	static int GetRightIndex(int index1, int index2);
	static int GetNumber(QString input, int index);
};