#pragma once

#include <QObject>
#include <QString>
#include <QQueue>
#include <QHash>

#include "console/command/AbstractCommand.h"
#include "console/command/Command.h"

namespace composer_gui
{
namespace console
{
namespace command
{
namespace invoker
{

class Invoker : public QObject
{
    Q_OBJECT
public:
    explicit Invoker(const QHash<QString, AbstractCommand *> &commands, QObject *parent = nullptr);
    AbstractCommand *getCommand(const QString &command);
    const QHash<QString, AbstractCommand *> &getCommands() const;

private:
    QHash<QString, AbstractCommand *> m_commands;

    bool exists(const QString &command);
    void execute(const Command &command);

public slots:
    void invoke(QQueue<composer_gui::console::command::Command> commands);
    void invoke(composer_gui::console::command::Command command);

signals:
    void dequeued(QString command);
};

} // invoker
} // command
} // console
} // composer_gui
