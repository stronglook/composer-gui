#include "console/command/AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

AbstractCommand::AbstractCommand(QObject *parent) : QObject (parent){}

void AbstractCommand::setCommand(const Command &command)
{
    m_command = command;
}

const Command &AbstractCommand::getCommand() const
{
    return m_command;
}

} // command
} // console
} // composer_gui
