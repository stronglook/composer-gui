#include "Pwd.h"

namespace composer_gui
{
namespace console
{
namespace command
{

Pwd::Pwd(QObject *parent) : AbstractCommand(parent)
{

}

void Pwd::run()
{
    emit started(getCommand());
    emit message(getCommand().getExecutionDirectory());
    emit finished(getCommand());
}

} // command
} // console
} // composer_gui
