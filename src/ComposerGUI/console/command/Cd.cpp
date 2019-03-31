#include <QFileInfo>

#include "Cd.h"

namespace composer_gui
{
namespace console
{
namespace command
{

Cd::Cd(QObject *parent) : AbstractCommand(parent)
{

}

void Cd::run()
{
    emit started(getCommand());

    QString newDirectory = getCommand().getArguments().first();

    if (newDirectory.isEmpty()) {
        emit message("Path is empty");
    } else if (!QFileInfo::exists(newDirectory)) {
        emit message("No such directory");
    } else {
        emit directory(newDirectory);
    }

    emit finished(getCommand());
}

} // command
} // console
} // composer_gui
