#pragma once
#include <QMap>

enum class CommandType {
    MOVE,
    POINT,
    PLANE,
    CIRCLE,
    DEVPOINT,
    DEVCIRCLE,
    PROJECTION,
    COMMENT,
    UNKNOWN
};

class Commands
{
public:
    static QMap<QString, CommandType> CommandList;
};

class Command
{
public: 
    CommandType Type;
    QList<double> Arguments;
    int Number;
    QString Comment;

    Command(CommandType type, QList<double> arguments, int number);
    Command(CommandType type, QString comment);
    Command();
};