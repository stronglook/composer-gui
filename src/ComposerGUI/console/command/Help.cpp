#include "Help.h"

namespace composer_gui
{
namespace console
{
namespace command
{

Help::Help(QObject *parent) : AbstractCommand(parent)
{

}

void Help::run()
{
    emit started(getCommand());

    QString msg;
    msg += "composer \t Run composer command: composer --version\n";
    msg += "php \t Run php command: php -v\n";
    msg += "pwd \t Prints present working directory\n";
    msg += "cd \t Change directory: cd path/to/new/directory\n";
    msg += "ls \t Lists the files in the current working directory: ls, ls -r (recursive)\n";
    msg += "clear \t Clear console contents";

    emit message(msg);
    emit finished(getCommand());
}

} // command
} // console
} // composer_gui
