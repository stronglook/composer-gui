#include "Unknown.h"

namespace composer_gui
{
namespace console
{
namespace command
{

Unknown::Unknown(QObject *parent) : AbstractCommand(parent)
{

}

void Unknown::run()
{
    emit started(getCommand());
    emit message("Unknown command: '" + getCommand().getFullname()+ "'");
    emit finished(getCommand());
}

} // command
} // console
} // composer_gui
