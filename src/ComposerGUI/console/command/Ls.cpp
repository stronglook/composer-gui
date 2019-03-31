#include <QDirIterator>

#include "Ls.h"

namespace composer_gui
{
namespace console
{
namespace command
{

Ls::Ls(QObject *parent) : AbstractCommand(parent)
{

}

void Ls::run()
{
    emit started(getCommand());

    bool isRecursive = false;

    const QStringList &arguments = getCommand().getArguments();

    if (!arguments.empty()) {
        QString flag = arguments.first().trimmed();
        isRecursive = flag.endsWith("-r", Qt::CaseSensitivity::CaseInsensitive);
    }

    QDirIterator::IteratorFlag iteratorFlag = isRecursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags;

    QDirIterator it(getCommand().getExecutionDirectory(), iteratorFlag);

    while (true)
    {
        if (!it.fileName().isEmpty() && it.fileName() != "." && it.fileName() != "..") {
            emit message(it.filePath());
        }

        if (!it.hasNext()) {
            break;
        }

        it.next();
    }

    emit finished(getCommand());
}

} // command
} // console
} // composer_gui
