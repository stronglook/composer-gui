#pragma once

#include "console/Console.h"
#include "console/command/invoker/Invoker.h"
#include "console/command/completer/Completer.h"

namespace composer_gui
{
namespace console
{
namespace factory
{

class Factory
{
    using Completer = command::completer::Completer;
    using Invoker = command::invoker::Invoker;

public:
    static Console *create();

private:
    static Completer *createCompleter();
    static Invoker *createInvoker();
    static void attachInvokerListeners(Console *console);
    static void attachCommandListeners(Console *console);
    static void attachConsoleListeners(Console *console);
};

} // factory
} // console
} // composer_gui
