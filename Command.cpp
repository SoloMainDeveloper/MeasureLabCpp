#include "Command.h"

QMap<QString, CommandType> Commands::CommandList = {
        { "MOVE", CommandType::MOVE },
        { "POINT", CommandType::POINT},
        { "PLANE", CommandType::PLANE},
        { "CIRCLE", CommandType::CIRCLE},
        { "DEVIATION-POINT", CommandType::DEVPOINT},
        { "DEVIATION-CIRCLE", CommandType::DEVCIRCLE},
        { "POINT-BY-PROJECTION", CommandType::PROJECTION }
};

Command::Command()
{
}

Command::Command(CommandType type, QList<double> arguments, int number)
{
    this->Type = type;
    this->Arguments = arguments;
    if (number != -1)
        this->Number = number;
    else
        this->Number = -1;
}

Command::Command(CommandType type, QString comment)
{
    this->Type = type;
    this->Number = -1;
    this->Comment = comment;
}
