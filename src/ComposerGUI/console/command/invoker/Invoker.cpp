#include <string>
#include <stdexcept>

#include "Invoker.h"
#include "concurrent/thread/Thread.h"

namespace composer_gui
{
namespace console
{
namespace command
{
namespace invoker
{

Invoker::Invoker(const QHash<QString, AbstractCommand *> &commands, QObject *parent)
    : QObject(parent),
      m_commands(commands)
{
    for (auto command : getCommands())
    {
        command->setParent(this);
    }
}

bool Invoker::exists(const QString &command)
{
    return (m_commands.find(command) != m_commands.end());
}

void Invoker::execute(const Command &command)
{
    if (!exists(command.getName().toLower())) {
        if (!exists("unknown")) {
            throw std::runtime_error(
                QString("Command %1 does not exists").arg(command.getName()).toStdString()
            );
        }

        m_commands["unknown"]->setCommand(command);
        m_commands["unknown"]->run();
    } else {
        AbstractCommand *cmd = m_commands[command.getName().toLower()];
        cmd->setCommand(command);
        cmd->run();
    }
}

AbstractCommand *Invoker::getCommand(const QString &command)
{
    if (!exists(command)) {
        throw std::runtime_error(
            QString("Command %1 does not exists").arg(command).toStdString()
        );
    }

    return m_commands[command];
}

const QHash<QString, AbstractCommand *> &Invoker::getCommands() const
{
    return m_commands;
}

void Invoker::invoke(QQueue<composer_gui::console::command::Command> commands)
{
    concurrent::thread::Thread([commands, this]() mutable
    {
        while (!commands.empty())
        {
            Command command = commands.dequeue();
            emit dequeued(command.getFullname());

            if (commands.count() > 0 && static_cast<bool>(command)) {
                execute(command);
            }
        }
    });
}

void Invoker::invoke(composer_gui::console::command::Command command)
{
    concurrent::thread::Thread([command, this]
    {
        execute(command);
    });
}

} // invoker
} // command
} // console
} // composer_gui
