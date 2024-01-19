#include "Debugger.h"
#include "Figure.h"
#include <QMap>

int Debugger::commandNumber = 0;
QList<FigureReport> Debugger::figuresData = QList<FigureReport>();

Machine Debugger::machine = Machine::Machine(100, 150, 200, 0.001, Point::Point());

void Debugger::RunCode(Ui::QtWidgetsMeasureLabClass ui, QString inputCode, int type)
{
    auto parsedCode = RemoveUselessSymbolsAndParse(inputCode);
    QString result = "";
    if (type == 0) {
        ReloadCommandNumber();
        commandNumber = 0;
    }
    for (auto i = commandNumber; i < parsedCode.length(); i++) {
        if (i == 0) {
            machine.ReloadPosition();
            ReloadCommandNumber();
            DataHolder::Points.clear();
            DataHolder::Planes.clear();
            DataHolder::Circles.clear();
            ui.output->clear();
        }
        try {
            auto command = GetCommand(parsedCode[i]);
            if (command.Type == CommandType::MOVE) {
                Point point(command.Arguments, 3, 2);
                machine.Position = machine.Move(point);
                QString message = "LOCATION: Coordinates of measure machine: (%1, %2, %3)\n";
                result += message.arg(machine.Position.X).arg(machine.Position.Y).arg(machine.Position.Z);
            }
            else if (command.Type == CommandType::POINT) {
                Point point = Point(command.Number);
                if (machine.TryCreatePoint(command.Arguments, command.Number, point)) {
                    result += point.ToQString();
                    QList<double> args = { point.X, point.Y, point.Z, point.RealX, point.RealY, point.RealZ,
                        point.Normal.VectorX, point.Normal.VectorY, point.Normal.VectorZ,
                        point.Normal.ActualVectorX, point.Normal.ActualVectorY, point.Normal.ActualVectorZ };
                    figuresData.append(FigureReport(QString("POINT"), point, &args));
                }
                else {
                    result += "Error: dot has not been added\n";
                }
            }
            else if (command.Type == CommandType::CIRCLE) {
                Circle circle = Circle(command.Number);
                if (machine.TryCreateCircle(command.Arguments, command.Number, circle)) {
                    result += circle.ToQString();
                    QList<double> args = { circle.Centre.X, circle.Centre.Y, circle.Centre.Z,
                        circle.Centre.RealX, circle.Centre.RealY, circle.Centre.RealZ,
                        circle.Normal.VectorX, circle.Normal.VectorY, circle.Normal.VectorZ,
                        circle.Normal.ActualVectorX, circle.Normal.ActualVectorY, circle.Normal.ActualVectorZ,
                        circle.Radius, circle.FactRadius, circle.Difference, circle.FactDifference };
                    figuresData.append(FigureReport(QString("CIRCLE"), circle, &args));
                }
                else
                    result += "Error: Circle has not been added\n";
            }
            else if (command.Type == CommandType::PLANE) {
                Plane plane = Plane(command.Number);
                if (machine.TryCreatePlane(command.Arguments, command.Number, plane)) {
                    result += plane.ToQString();
                    QList<double> args = { plane.Start.X, plane.Start.Y, plane.Start.Z,
                        plane.Start.RealX, plane.Start.RealY, plane.Start.RealZ,
                        plane.Normal.VectorX, plane.Normal.VectorY, plane.Normal.VectorZ,
                        plane.Normal.ActualVectorX, plane.Normal.ActualVectorY, plane.Normal.ActualVectorZ,
                        plane.Difference, plane.FactDifference };
                    figuresData.append(FigureReport(QString("PLANE"), plane, &args));
                }
                else
                    result += "Error: Plane has not been added\n";
            }
            else if (command.Type == CommandType::DEVPOINT) {
                result += machine.DevPoint(command.Arguments, command.Number);
                figuresData.append(FigureReport(QString("DEVPOINT"), command));
            }
            else if (command.Type == CommandType::DEVCIRCLE) {
                result += machine.DevCircle(command.Arguments, command.Number);
                figuresData.append(FigureReport(QString("DEVCIRCLE"), command));
            }
            else if (command.Type == CommandType::PROJECTION) {
                Point point = Point(command.Number);
                if (machine.TryCreateProjection(command.Arguments, command.Number, point)) {
                    result += "Projection dot:\n" + point.ToQString();
                    QList<double> args = { point.X, point.Y, point.Z, point.RealX, point.RealY, point.RealZ,
                        point.Normal.VectorX, point.Normal.VectorY, point.Normal.VectorZ,
                        point.Normal.ActualVectorX, point.Normal.ActualVectorY, point.Normal.ActualVectorZ };
                    figuresData.append(FigureReport(QString("POINT"), point, &args));
                }
                else
                    result += "Error: Projection dot has not been added\n";
            }
            else if (command.Type == CommandType::COMMENT) {
                result += machine.Comment(command.Comment);
                QList<double> args = {};
                figuresData.append(FigureReport(QString("COMMENT"), Figure(), &args, command.Comment));
            }
            else if (command.Type == CommandType::UNKNOWN) {
                result += machine.UnknownCommand(command.Comment);
            }
            if (type == 0)
                result += '\n';
        }
        catch (...) {
            result.append(QString("Error"));
        }
        if (type != 0) {
            commandNumber += 1;
            ui.output->appendPlainText(result);
            if (i + 1 == parsedCode.length())
                commandNumber = 0;
            break;
        }
    }
    if (type == 0) {
        ui.output->setPlainText(result);
        commandNumber = 0;
    }
}

void Debugger::ReloadCommandNumber()
{
	commandNumber = 0;
    figuresData.clear();
}

QStringList Debugger::RemoveUselessSymbolsAndParse(QString inputCode)
{
    auto input = inputCode.split(";");
    input.removeLast();
    for (auto i = 0; i < input.length(); i++)
    {
        input[i] = input[i].trimmed().replace("\n", "").replace("\t", "");
        if (!input[i].startsWith("//")) {
            input[i].replace(" ", "");
        }
    }
    return input;
}

Command Debugger::GetCommand(QString input)
{
	int indexOpenBracket = input.indexOf("(");
	int indexUnderline = input.indexOf("_");
    if (indexOpenBracket != -1 || indexUnderline != -1)
    {
        int index = GetRightIndex(indexOpenBracket, indexUnderline);
        QString commandName = input.mid(0, index).trimmed();
        if (Commands::CommandList.contains(commandName))
        {
            CommandType commandType = Commands::CommandList[commandName];
            int number = GetNumber(input, index);
            QList<double> arguments = TryGetArguments(commandType, input);
            return Command(commandType, arguments, number);
        }
    }
    else if (input.trimmed().mid(-2) == "//")
    {
        return Command(CommandType::COMMENT, input.trimmed().mid(2));
    }
    return Command(CommandType::UNKNOWN, input);
}

QList<double> Debugger::TryGetArguments(CommandType type, QString input)
{
    auto indexOpenBracket = input.indexOf("(");
    auto indexEndBracket = input.lastIndexOf(")");
    auto indexOpenSquareBracket = input.indexOf("[");
    auto indexEndSquareBracket = input.indexOf("]");
    QList<double> arguments = SubstringArguments(input, indexOpenBracket, indexEndBracket, type);
    if (type == CommandType::POINT)
    {
        auto extraArgs = SubstringArguments(input, indexOpenSquareBracket, indexEndSquareBracket, type);
        arguments.append(extraArgs);
    }
    //return &args;
    return arguments;
}

QList<double> Debugger::SubstringArguments(QString input, int start, int end, CommandType type)
{
    if (type == CommandType::MOVE || type == CommandType::POINT || type == CommandType::DEVPOINT) {
        QString inputString = input.mid(start + 1, end - start - 1);
        QStringList inputList = inputString.split(",");
        QList<double> myList;
        for (const QString& str : inputList) {
            myList << str.toDouble();
        }
        return myList;
    }
    else if (type == CommandType::PLANE || type == CommandType::CIRCLE || type == CommandType::PROJECTION || type == CommandType::DEVCIRCLE) {
        QString inputString = input.mid(start + 1, end - start - 1);
        inputString.replace("POINT_", "");
        inputString.replace("PLANE_", "");
        QStringList inputList = inputString.split(",");
        QList<double> myList;
        for (const QString& str : inputList) {
            myList << str.toDouble();
        }
        return myList;
    }
    else {
        return QList<double>();
    }
}

int Debugger::GetRightIndex(int index1, int index2)
{
    if (index1 < 0)
        return index2;
    else if (index2 < 0)
        return index1;
    else
        return std::min(index1, index2);
    return 0;
}

int Debugger::GetNumber(QString input, int index)
{
    QString number;
    for (int i = index + 1; i < input.length(); i++) {
        if (input[i].isDigit())
            number.append(input[i]);
        else
            break;
    }
    return number.length() > 0 ? number.toInt() : -1;
    return 0;
}
