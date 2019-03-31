#pragma once

#include <QMenu>
#include <QPoint>
#include <QAbstractItemView>

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace controls
{

class Context
{
public:
    Context(QAbstractItemView *view);
    void show(const QPoint &pos);

protected:
    void edit();
    void remove();

private:
    QMenu m_menu;
    QAbstractItemView *m_view;
};

} // controls
} // form
} // package
} // composer
} // composer_gui
