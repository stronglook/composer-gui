#include <QHostInfo>
#include <QString>
#include <QQueue>
#include <QHash>
#include <QDir>
#include <QtGlobal>

#include "Factory.h"
#include "globals.h"
#include "console/command/Command.h"
#include "console/command/Unknown.h"
#include "console/command/Composer.h"
#include "console/command/Clear.h"
#include "console/command/Help.h"
#include "console/command/Pwd.h"
#include "console/command/Php.h"
#include "console/command/Ls.h"
#include "console/command/Cd.h"

namespace composer_gui
{
namespace console
{
namespace factory
{

Console *Factory::create()
{
    QString name = qgetenv("USER");
    if (name.isEmpty()) {
        name = qgetenv("USERNAME");
    }

    QString whoami = "[" + name + "@" + QHostInfo::localHostName() + " ~]# ";

    Console *console = new Console(whoami, createInvoker());
    console->setCursorWidth(3);
    console->addText("Welcome to ComposerGUI shell\nType 'help' to see available commands.", false);
    console->addText(whoami);

    Completer *completer = createCompleter();
    completer->setWidget(console);
    completer->setParent(console);

    qRegisterMetaType<composer_gui::console::command::Command>("Command");

    attachConsoleListeners(console);
    attachInvokerListeners(console);
    attachCommandListeners(console);

    console->setStyleSheet(COMPOSERGUI_WIDGET_BACKGROUND);
    return console;
}

command::completer::Completer *Factory::createCompleter()
{
    Completer *completer = new Completer({"cd", "pwd", "composer", "ls", "ls -r", "clear", "php", "help"});
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    return completer;
}

command::invoker::Invoker *Factory::createInvoker()
{
    using namespace command;

    QHash<QString, AbstractCommand*> commands({
        {"composer", new Composer},
        {"php", new Php},
        {"cd", new Cd},
        {"ls", new Ls},
        {"pwd", new Pwd},
        {"clear", new Clear},
        {"help", new Help},
        {"unknown", new Unknown},
    });

    return new Invoker(commands);
}

void Factory::attachInvokerListeners(Console *console)
{
    QObject::connect(console->getCommandInvoker(), &Invoker::dequeued, console, &Console::insertPlainText);
}

void Factory::attachCommandListeners(Console *console)
{
    using namespace command;

    Invoker *invoker = console->getCommandInvoker();

    QObject::connect(invoker->getCommand("clear"), &AbstractCommand::finished, console, &Console::clear);
    QObject::connect(static_cast<Cd *>(invoker->getCommand("cd")), &Cd::directory, console, [console](const QString &dir)
    {
        console->setWorkingDirectory(QDir::toNativeSeparators(dir));
    });

    for (auto command : invoker->getCommands())
    {
        QObject::connect(command, &AbstractCommand::started, console, [console]
        {
            console->setActive(false);
            console->setTextColor(COMPOSERGUI_CONSOLE_PROCESSING_TEXT_COLOR);
        });

        QObject::connect(command, &AbstractCommand::message, console, [console](const QString &text){
            console->addText(text);
        });

        QObject::connect(command, &AbstractCommand::finished, console, [console]
        {
            console->setTextColor(COMPOSERGUI_DEFAULT_TEXT_COLOR);
            console->addWhoami();
            console->setActive(true);
        });
    }
}

void Factory::attachConsoleListeners(Console *console)
{
    Invoker *invoker = console->getCommandInvoker();

    QObject::connect(console, &Console::command, invoker, QOverload<composer_gui::console::command::Command>::of(&Invoker::invoke));
    QObject::connect(console, &Console::paste, invoker, QOverload<QQueue<composer_gui::console::command::Command>>::of(&Invoker::invoke));
}

} // factory
} // console
} // composer_gui
