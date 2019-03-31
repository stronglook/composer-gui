#include "Clear.h"

namespace composer_gui
{
namespace console
{
namespace command
{

Clear::Clear(QObject *parent) : AbstractCommand(parent)
{

}

void Clear::run()
{
    emit finished(getCommand());
}

} // command
} // console
} // composer_gui
