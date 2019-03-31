#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

#include "Command.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class AbstractCommand : public QObject
{
    Q_OBJECT
public:
    AbstractCommand(QObject *parent = nullptr);
    void setCommand(const Command &command);
    const Command &getCommand() const;
    virtual void run() = 0;
private:
    QString m_executionDirectory;
    Command m_command;

signals:
    void started(Command);
    void message(QString);
    void finished(Command);
};

} // command
} // console
} // composer_gui
