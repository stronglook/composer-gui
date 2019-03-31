#include "Command.h"

namespace composer_gui
{
namespace console
{
namespace command
{

Command::Command(const QString &fullname, const QString &executionPath)
{
    m_executionPath = executionPath;

    m_arguments = fullname.section(' ', 1).split(' ');
    m_fullname  = fullname;
    m_name      = fullname.section(' ', 0, 0);
}

const QString &Command::getExecutionDirectory() const
{
    return m_executionPath;
}

const QStringList &Command::getArguments() const
{
    return m_arguments;
}

const QString &Command::getFullname()const
{
    return m_fullname;
}

const QString &Command::getName() const
{
    return m_name;
}

Command::operator bool() const
{
    return !m_fullname.isEmpty();
}

} // command
} // console
} // composer_gui
