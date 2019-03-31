#pragma once

#include <QStringList>
#include <QMetaType>
#include <QString>

namespace composer_gui
{
namespace console
{
namespace command
{

class Command
{
public:
    Command() = default;
    Command(const QString &fullname, const QString &executionPath);
    const QString &getExecutionDirectory() const;
    const QStringList &getArguments() const;
    const QString &getFullname() const;
    const QString &getName() const;
    explicit operator bool() const;

private:
    QString m_executionPath;
    QStringList m_arguments;
    QString m_fullname;
    QString m_name;
};

} // command
} // console
} // composer_gui

Q_DECLARE_METATYPE(composer_gui::console::command::Command);
