#pragma once

#include <functional>
#include <QThreadPool>

namespace composer_gui
{
namespace concurrent
{
namespace thread
{

class Thread
{
public:
    Thread(std::function<void()> worker);

private:
    class ThreadPoolAccessor
    {
    public:
        static QThreadPool &getThreadPoolInstance();
    };
};

} // thread
} // concurrent
} // composer_gui
