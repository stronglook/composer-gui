#include <QtConcurrent>
#include <QSettings>

#include "globals.h"
#include "Thread.h"

namespace composer_gui
{
namespace concurrent
{
namespace thread
{

Thread::Thread(std::function<void()> worker)
{
    QtConcurrent::run(&ThreadPoolAccessor::getThreadPoolInstance(), worker);
}

QThreadPool &Thread::ThreadPoolAccessor::getThreadPoolInstance()
{
    static QThreadPool threadPool;

    QSettings settings(COMPOSERGUI_ORGANIZATION_NAME, COMPOSERGUI_SETTINGS_APPLICATION);
    threadPool.setMaxThreadCount(settings.value("worker_threads").toInt());

    return threadPool;
}


} // thread
} // concurrent
} // composer_gui
